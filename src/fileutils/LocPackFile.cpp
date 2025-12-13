//
// Created by swappel on 12/11/25.
//

#include "../../include/LocPackFile.h"

#include <fstream>
#include <unistd.h>

#include "libraries/rapidcsv.h"

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

// TODO
std::vector<LocaleLine> LocPackFile::parseLocPack() const
{
    std::vector<LocaleLine> lines;
    auto* locale_line = new LocaleLine();

    std::ifstream input(filePath);

    return lines;
}

/*
 * Tries to find the index of a line where the given hash is found
 *
 * Returns -1 if no line was found.
 */
int LocPackFile::findHashIndex(const std::string& hash) const
{
    LocaleLine foundLine;

    std::ifstream input(filePath);
    std::string line;
    auto doc = rapidcsv::Document{filePath,
                                    rapidcsv::LabelParams(-1,-1),
                                    rapidcsv::SeparatorParams(',',
                                                        false,
                                                        false,
                                                true,
                                                    false
                                    )
    };

    int foundIndex = -1;
    for (int i = 0; i < doc.GetRowCount(); ++i)
    {

        const std::vector<std::string> entryHash = doc.GetRow<std::string>(i);

        if (entryHash[0] == hash)
        {
            foundIndex = i;
            break;
        }
    }

    return foundIndex;
}

/*
 * Uses a hash as input and returns a LocalLine object with the data of the line.
 *
 * Throws an error if the hash could not be found.
 */
LocaleLine LocPackFile::findHashComplete(const std::string& hash) const
{
    const int foundIndex = findHashIndex(hash);

    if (foundIndex == -1)
    {
        throw std::runtime_error("Hash not found in the LocPack file: " + hash);
    }

    const auto doc = rapidcsv::Document{filePath,
                                    rapidcsv::LabelParams(-1,-1),
                                    rapidcsv::SeparatorParams(',',
                                                        false,
                                                        false,
                                                true,
                                                    false
                                    )
    };

    const std::vector<std::string> readStrings = doc.GetRow<std::string>(foundIndex);

    std::string reformatedContent = readStrings[3];
    reformatedContent.erase(std::ranges::remove(reformatedContent, '\r').begin(), reformatedContent.end());

    return LocaleLine{readStrings[0], reformatedContent, std::stoi(readStrings[1]), std::stoi(readStrings[2])};
}