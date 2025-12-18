#include "../../include/LocPackBinFile.h"

#include <algorithm>

LocPackBinFile::LocPackBinFile() = default;

LocPackBinFile::LocPackBinFile(const std::string &pathString)
{
    if (!loadBinaryFile(pathString))
    {
        throw std::runtime_error("Initialization failed: Could not load binary file at path '" + filePath.string() + "'.");
    }
}

BlockInfo::BlockInfo(int offset, uint16_t length, std::string text)
    : offset(offset), length(length), text(std::move(text)) { }

void LocPackBinFile::validateAndLoad(const std::string &pathString) {
    const auto testPath = std::filesystem::path(pathString);

    if (!std::filesystem::exists(testPath)) throw std::runtime_error("Path '" + pathString + "' does not exist.");
    if (std::filesystem::is_directory(testPath)) throw std::runtime_error("Path '" + pathString + "' is a directory. Should be a file.");
    if (testPath.extension() != ".locpackbin") throw std::runtime_error("File '" + pathString + "' has an incorrect extension. Should be '.locpack'");

    filePath = testPath;

    lastLoadTime = std::filesystem::last_write_time(filePath);
}

bool LocPackBinFile::loadBinaryFile(const std::string &pathString)
{
    try
    {
        validateAndLoad(pathString);
    } catch (std::runtime_error &e)
    {
        std::cerr << "An error occured while trying to find the file: \n" << e.what() << std::endl;
        return false;
    }

    std::ifstream input(filePath, std::ios::binary | std::ios::ate);

    if (!input.is_open())
    {
        std::cerr << "Could not open file at path '" + filePath.string() + "." << std::endl;
        return false;
    }

    const size_t fileSize = input.tellg();

    fileContents.resize(fileSize);
    input.seekg(0, std::ios::beg);

    if (input.read(reinterpret_cast<char*>(fileContents.data()), fileSize))
    {
        lastLoadTime = std::filesystem::last_write_time(filePath);
        return true;
    } else
    {
        std::cerr << "Failed reading file at path '" + filePath.string() << "'." << std::endl;
        return false;
    }
    // TODO: Implement the server error response
}

// uint16_t LocPackBinFile::read_u16_le(const uint8_t* ptr)
// {
//     return static_cast<uint16_t>(ptr[0]) | static_cast<uint16_t>(ptr[1]) << 8;
// }

// uint16_t LocPackBinFile::read_u16_be(const uint8_t* ptr)
// {
//     return static_cast<uint16_t>(ptr[1]) | static_cast<uint16_t>(ptr[0]) << 8;
// }

namespace
{
    // const std::set<int> &getPrintableSet()
    // {
    //     static const std::set<int> PRINTABLE_SET = []
    //     {
    //         std::set<int> s;
    //
    //         for (int i = 32; i < 126; ++i)
    //         {
    //             s.insert(i);
    //         }
    //
    //         s.insert(9);
    //         s.insert(10);
    //         s.insert(13);
    //         return s;
    //     }();
    //     return PRINTABLE_SET;
    // }

    uint16_t hexToWord(const std::string &bytes) {
        /*
         * Just for show, this is the structure:
         * HEX: 0x02 0x23
         */
    }
}

// bool LocPackBinFile::isPrintable(const uint8_t* start_ptr, const uint8_t* end_ptr)
// {
//     const std::set<int> &printableChars = getPrintableSet();
//
//     for (const uint8_t *ptr = start_ptr; ptr < end_ptr; ++ptr)
//     {
//         if (!printableChars.contains(*ptr))
//         {
//             return false;
//         }
//     }
//     return true;
// }

// std::map<std::string, std::vector<uint8_t>> LocPackBinFile::getHashVariationsBytes(const std::string& hex_hash)
// {
//     if (hex_hash.length() % 2 != 0)
//     {
//         throw std::runtime_error("Hash string has odd length and cannot be converted to bytes.");
//     }
//
//     std::vector<uint8_t> raw_bytes;
//
//     for (int i = 0; i < hex_hash.length(); i += 2)
//     {
//         raw_bytes.push_back(hexToByte(hex_hash.substr(i, 2)));
//     }
//
//     std::map<std::string, std::vector<uint8_t>> variants;
//
//     variants.emplace("raw", raw_bytes);
//
//     std::vector<uint8_t> reversed_bytes = raw_bytes;
//     std::ranges::reverse(reversed_bytes);
//     variants.emplace("little_endian", reversed_bytes);
//
//     return variants;
// }

/**
 * Converts a standard hash from .locpack file to hash from the .locpackbin file
 * @param hash The hash to convert
 * @return The converted hash
 */
std::string LocPackBinFile::convertHash(const std::string &hash){
    std::string firstHalf = hash.substr(0, hash.length()/2);
    std::string secondHalf = hash.substr(hash.length()/2);

    std::string convertedString;

    for (int i = 15; i > 0; i -= 2)
    {
        convertedString += firstHalf.substr(i - 1, 2);
    }

    for (int i = 15; i > 0; i -= 2)
    {
        convertedString += secondHalf.substr(i - 1, 2);
    }



    return convertedString;
}

/**
 * Finds a hash in the stored .locpackbin file.
 * @param hash The CONVERTED hash to find in the .locpackbin file
 * @return The BlockInfo with all the information about the found entry
 */
BlockInfo LocPackBinFile::getTextByHash(const std::string &hash) const {

    byte hashBytes[16];
    // Convert char to bytes
    for (int charIdx = 0; charIdx < 32; charIdx += 2) {
        hashBytes[charIdx] = (hash[charIdx] << 4) + hash[charIdx + 1];
    }

    // Search for the actual hash
    const auto search = std::ranges::search(fileContents, hashBytes
    ).begin();

    // Verify if the search was found
    if (search == fileContents.end()) throw std::runtime_error("No entry was found with hash '" + hash + "'");
    const int findIdx = std::distance(fileContents.begin(), search);

    const uint16_t contentLength = fileContents[findIdx + 32 + 9];
    char content[contentLength + 1];

    for (int i = 0; i < contentLength; ++i)
    {
        content[i] = fileContents[findIdx + 44 + i];
    }

    content[contentLength] = '\0';

    const std::string stringContent(content);

    return BlockInfo{
        findIdx,
        contentLength,
        stringContent
    };

}

// TODO: Rewrite the documentation EVERYWHERE in the program!!!!!!!