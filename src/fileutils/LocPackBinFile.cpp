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

    // 1. Critical existence check
    if (!std::filesystem::exists(testPath)) {
        throw std::runtime_error("CRITICAL: File not found at " + pathString);
    }

    if (std::filesystem::is_directory(testPath)) throw std::runtime_error("Path is a directory.");

    // Now it is safe to call metadata functions
    filePath = testPath;
    lastLoadTime = std::filesystem::last_write_time(filePath);
}

std::filesystem::path LocPackBinFile::getPath() const {
    return filePath;
}

void LocPackBinFile::setPath(const std::string &pathString)
{
    validateAndLoad(pathString);
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

// Helper to convert 'A' to 10, '1' to 1, etc.
auto hexCharToByte = [](char c) -> uint8_t {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return 0;
};

/**
 * Finds a hash in the stored .locpackbin file.
 * @param hash The CONVERTED hash to find in the .locpackbin file
 * @return The BlockInfo with all the information about the found entry
 */
BlockInfo LocPackBinFile::getTextByHash(const std::string &hash) const {
    auto hexToVal = [](char c) -> uint8_t {
        if (c >= '0' && c <= '9') return c - '0';
        if (c >= 'a' && c <= 'f') return c - 'a' + 10;
        if (c >= 'A' && c <= 'F') return c - 'A' + 10;
        return 0;
    };

    // 1. Convert Hash String to Bytes
    uint8_t hashBytes[16];
    for (int i = 0; i < 16; ++i) {
        hashBytes[i] = (hexToVal(hash[i * 2]) << 4) | hexToVal(hash[i * 2 + 1]);
    }

    // 2. Locate Hash in File
    auto searchRange = std::ranges::search(fileContents, hashBytes);
    if (searchRange.empty()) throw std::runtime_error("Hash not found: " + hash);
    const size_t findIdx = std::distance(fileContents.begin(), searchRange.begin());

    // 3. Extract 2-byte Length at Offset +24
    // We use memcpy or bit-shifting to ensure we respect Little Endian
    uint16_t contentLength = 0;
    contentLength = static_cast<uint16_t>(fileContents[findIdx + 24]) |
                    (static_cast<uint16_t>(fileContents[findIdx + 25]) << 8);

    // 4. Extract String starting at Offset +26
    std::string stringContent;
    stringContent.resize(contentLength);
    std::memcpy(stringContent.data(), &fileContents[findIdx + 26], contentLength);

    return BlockInfo{ static_cast<int>(findIdx), contentLength, stringContent };
}

void LocPackBinFile::updateEntry(const std::string& hexHash, int val1, int val2, const std::string& newText) const {
    // 1. Prepare the new binary hash (Little Endian as per your logic)
    // Assuming you use your existing conversion logic or a helper
    std::string convertedHex = convertHash(hexHash);
    uint8_t hashBytes[16];
    auto hexToVal = [](char c) -> uint8_t {
        if (c >= '0' && c <= '9') return c - '0';
        if (c >= 'a' && c <= 'f') return c - 'a' + 10;
        if (c >= 'A' && c <= 'F') return c - 'A' + 10;
        return 0;
    };
    for (int i = 0; i < 16; ++i) {
        hashBytes[i] = (hexToVal(convertedHex[i * 2]) << 4) | hexToVal(convertedHex[i * 2 + 1]);
    }

    // 2. Find the existing entry
    auto it = std::ranges::search(fileContents, hashBytes).begin();
    if (it == fileContents.end()) throw std::runtime_error("Hash not found for update.");

    size_t findIdx = std::distance(fileContents.begin(), it);

    // 3. Get old length to know how many bytes to remove
    uint16_t oldLength = static_cast<uint16_t>(fileContents[findIdx + 24]) |
                         (static_cast<uint16_t>(fileContents[findIdx + 25]) << 8);
    size_t oldTotalSize = 26 + oldLength;

    // 4. Construct the new entry buffer
    std::vector<uint8_t> newEntry;
    newEntry.reserve(26 + newText.length());

    // Add Hash
    newEntry.insert(newEntry.end(), hashBytes, hashBytes + 16);
    // Add Val1 (4 bytes)
    newEntry.insert(newEntry.end(), reinterpret_cast<uint8_t*>(&val1), reinterpret_cast<uint8_t*>(&val1) + 4);
    // Add Val2 (4 bytes)
    newEntry.insert(newEntry.end(), reinterpret_cast<uint8_t*>(&val2), reinterpret_cast<uint8_t*>(&val2) + 4);
    // Add Length (2 bytes)
    uint16_t newLen = static_cast<uint16_t>(newText.length());
    newEntry.insert(newEntry.end(), reinterpret_cast<uint8_t*>(&newLen), reinterpret_cast<uint8_t*>(&newLen) + 2);
    // Add String
    newEntry.insert(newEntry.end(), newText.begin(), newText.end());

    // 5. Atomic Update of the vector
    // This removes the old block and slides everything else over, then inserts the new block
    fileContents.erase(fileContents.begin() + findIdx, fileContents.begin() + findIdx + oldTotalSize);
    fileContents.insert(fileContents.begin() + findIdx, newEntry.begin(), newEntry.end());

    // 6. Save back to disk
    std::ofstream output(filePath, std::ios::binary);
    output.write(reinterpret_cast<const char*>(fileContents.data()), fileContents.size());
}

// TODO: Rewrite the documentation EVERYWHERE in the program!!!!!!!