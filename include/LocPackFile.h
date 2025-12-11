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

    LocaleLine parseLocPack();

    void findHash(const std::string&);
    void findByHash(const std::string&);

private:
    std::filesystem::path filePath;

};


#endif //AFOP_TRANSLATOR_BACKEND_FILEREADER_H