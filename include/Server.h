#pragma once
#include "libraries/crow_all.h"
#include "LocPackFile.h"
#include "LocPackBinFile.h"
#include <iostream>

class LocPackServer {
public:
    LocPackServer(const std::string& csvPath, const std::string& binPath)
        : csvHandler(csvPath), binHandler(binPath) {}

    void run(int port = 18080) {
        setupRoutes();
        app.port(port).multithreaded().run();
    }

private:
    crow::App<crow::CORSHandler> app;
    LocPackFile csvHandler;
    LocPackBinFile binHandler;

    void setupRoutes() {
        auto& cors = app.get_middleware<crow::CORSHandler>();

        cors
            .global()
            .origin("http://localhost:5173")
            .methods(crow::HTTPMethod::Post, crow::HTTPMethod::Get, crow::HTTPMethod::Options)
            .headers("Content-Type", "Authorization");

        // --- ROUTE: Update Entry ---
        // FIX: We explicitly allow POST and OPTIONS.
        // We also check the method inside to ensure Crow doesn't trip over itself.
        // 1. Remove .methods(...) from the route definition
        // 2. Add an explicit check inside the lambda
        CROW_ROUTE(app, "/api/update").methods(crow::HTTPMethod::Post, crow::HTTPMethod::Options)
        ([this](const crow::request& req) {
            // Manually handle the OPTIONS preflight
            if (req.method == crow::HTTPMethod::Options) {
                auto res = crow::response(204);
                res.add_header("Access-Control-Allow-Origin", "http://localhost:5173");
                res.add_header("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
                res.add_header("Access-Control-Allow-Headers", "Content-Type, Authorization");
                return res;
            }

            // Explicitly reject anything that isn't POST
            if (req.method != crow::HTTPMethod::Post) {
                return crow::response(405, "Method Not Allowed");
            }

            auto data = crow::json::load(req.body);
            if (!data) return crow::response(400, "Invalid JSON");

            try {
                std::string hash = data["hash"].s();
                int v1 = data["val1"].i();
                int v2 = data["val2"].i();
                std::string text = data["text"].s();

                binHandler.updateEntry(hash, v1, v2, text);
                csvHandler.updateEntry(hash, v1, v2, text);

                return crow::response(200, "Update Successful");
            } catch (const std::exception& e) {
                std::cerr << "Update Error: " << e.what() << std::endl;
                return crow::response(500, e.what());
            }
        });

        // --- ROUTE: Range (Pagination) ---
        CROW_ROUTE(app, "/api/range")
        ([this](const crow::request& req) {
            try {
                const char* offset_raw = req.url_params.get("offset");
                const char* limit_raw = req.url_params.get("limit");
                int offset = offset_raw ? std::stoi(offset_raw) : 0;
                int limit = limit_raw ? std::stoi(limit_raw) : 10;

                auto lines = csvHandler.parseLocPackRange(offset, limit);
                crow::json::wvalue res;
                int count = 0;
                for (auto& line : lines) {
                    if (line.getHash() == "Hash" || line.getHash().empty()) continue;
                    res["data"][count]["hash"] = line.getHash();
                    res["data"][count]["text"] = line.getContent();
                    res["data"][count]["val1"] = line.getCharacter();
                    res["data"][count]["val2"] = line.getUnknown();
                    count++;
                }
                return crow::response(res);
            } catch (const std::exception& e) {
                return crow::response(500, e.what());
            }
        });

        // --- ROUTE: Search ---
        CROW_ROUTE(app, "/api/search")
([this](const crow::request& req) {
    const char* q = req.url_params.get("q");
    if (!q) return crow::response(200, "[]");

        auto all = csvHandler.parseLocPackWhole();

        crow::json::wvalue res;
        res = crow::json::wvalue::list();


        // CRITICAL: Initialize as a list to prevent 500 errors
        std::vector<crow::json::wvalue> results;

        int count = 0;
     for (const auto& line : all) {
         if (line.getHash() == "Hash") continue;
         if (line.getHash().find(q) != std::string::npos ||
             line.getContent().find(q) != std::string::npos) {

             res[count]["hash"] = line.getHash();
             res[count]["text"] = line.getContent();
             res[count]["val1"] = line.getCharacter();
             res[count]["val2"] = line.getUnknown();
             count++;
         }
         if (count >= 50) break;
         return crow::response(crow::json::wvalue(results));
});
    }
};