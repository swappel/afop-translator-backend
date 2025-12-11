//
// Created by swappel on 12/11/25.
//

#include "../../include/LocPackFile.h"

LocPackFile::LocPackFile() {}

LocPackFile::LocPackFile(const std::string &path) {
    filePath = std::filesystem::path(path);
}

std::filesystem::path LocPackFile::getPath() const {
    return filePath;
}

void LocPackFile::setPath(const std::string &path)
{
    filePath = std::filesystem::path(path);
}

// Parses the .locpack files to a 500-long array of LocaleLine objects
std::vector<LocaleLine> LocPackFile::parseLocPack() {

}

void LocPackFile::findHash(const std::string&)
{
}

void LocPackFile::findByHash(const std::string&)
{
}
