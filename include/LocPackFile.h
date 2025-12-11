//
// Created by elanda on 12/11/25.
//

#ifndef AFOP_TRANSLATOR_BACKEND_FILEREADER_H
#define AFOP_TRANSLATOR_BACKEND_FILEREADER_H
#include <filesystem>
#include <string>


class LocPackFile
{

public:
    LocPackFile();
    explicit LocPackFile(std::string);
    [[nodiscard]] std::filesystem::path getPath() const;
    void setPath(std::string);
    void findHash(std::string);
    void findByHash(std::string);

private:
    std::filesystem::path filePath;

};


#endif //AFOP_TRANSLATOR_BACKEND_FILEREADER_H