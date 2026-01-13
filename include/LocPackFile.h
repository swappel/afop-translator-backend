//
// Created by swappel on 12/11/25.
//

#ifndef AFOP_TRANSLATOR_BACKEND_FILEREADER_H
#define AFOP_TRANSLATOR_BACKEND_FILEREADER_H
#include <filesystem>
#include <string>
#include <vector>

#include "LocaleLine.h"
#include "libraries/rapidcsv.h"

class LocPackFile
{
private:
    std::filesystem::path filePath;
    mutable std::unique_ptr<rapidcsv::Document> doc;
    mutable std::filesystem::file_time_type lastLoadTime;

    [[nodiscard]] bool refreshDocument() const;
    void validateAndLoad(const std::string &pathString);

public:
    LocPackFile();
    explicit LocPackFile(const std::string &pathString);
    [[nodiscard]] std::filesystem::path getPath() const;
    void setPath(const std::string &pathString);

    [[nodiscard]] std::vector<LocaleLine> parseLocPackRange(const int offset, const int amount) const;
    [[nodiscard]] std::vector<LocaleLine> parseLocPackWhole() const;
    [[nodiscard]] int findHashIndex(const std::string& hash) const;

    [[nodiscard]] LocaleLine findHashComplete(const std::string& hash) const;

    void updateEntry(const std::string &hash, int character, int unknown, const std::string &newContent);
};


#endif //AFOP_TRANSLATOR_BACKEND_FILEREADER_H