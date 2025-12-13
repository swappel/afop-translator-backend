//
// Created by swappel on 12/11/25.
//

#ifndef AFOP_TRANSLATOR_BACKEND_FILEREADER_H
#define AFOP_TRANSLATOR_BACKEND_FILEREADER_H
#include <filesystem>
#include <string>
#include <vector>

#include "LocaleLine.h"


class LocPackFile
{

public:
    LocPackFile();
    explicit LocPackFile(const std::string &path);
    [[nodiscard]] std::filesystem::path getPath() const;
    void setPath(const std::string &path);

    [[nodiscard]] std::vector<LocaleLine> parseLocPack() const;
    [[nodiscard]] int findHashIndex(const std::string& hash) const;

    [[nodiscard]] LocaleLine findHashComplete(const std::string& hash) const;

private:
    std::filesystem::path filePath;

};


#endif //AFOP_TRANSLATOR_BACKEND_FILEREADER_H