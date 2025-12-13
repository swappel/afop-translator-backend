//
// Created by elanda on 12/13/25.
//

#ifndef AFOP_TRANSLATOR_BACKEND_LOCPACKBINFILE_H
#define AFOP_TRANSLATOR_BACKEND_LOCPACKBINFILE_H
#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <set>
#include <iostream>
#include <iterator>


struct BlockInfo
{
    int offset;
    uint16_t length;
    bool is_big_endian;
    std::string text;
};

using byte = uint8_t;

class LocPackBinFile
{
private:
    void validateAndLoad(const std::string &pathString);

    //[[nodiscard]] const std::set<int> & getPrintableSet() const;

    std::filesystem::path filePath;

    mutable std::vector<byte> fileContents;
    mutable std::filesystem::file_time_type lastLoadTime;

    [[nodiscard]] uint16_t read_u16_le(const uint8_t* ptr);
    [[nodiscard]] uint16_t read_u16_be(const uint8_t* ptr);
    [[nodiscard]] bool isPrintable(const uint8_t *start_ptr, const uint8_t *end_ptr);
    [[nodiscard]] std::map<std::string, std::vector<uint8_t>> getHashVariationsBytes(const std::string &hex_hash);

    [[nodiscard]] std::vector<size_t> findBytePattern(std::vector<byte> &pattern);
public:
    // Constructors
    [[nodiscard]] LocPackBinFile();
    [[nodiscard]] explicit LocPackBinFile(std::string &pathString);

    bool loadBinaryFile(const std::string &pathString);

    [[nodiscard]] std::string getTextByHash(std::string &hash);
};


#endif //AFOP_TRANSLATOR_BACKEND_LOCPACKBINFILE_H