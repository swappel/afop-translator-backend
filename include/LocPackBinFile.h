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
    std::string text;

    BlockInfo() = default;

    explicit BlockInfo(int size, uint16_t length, std::string text);
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

    //[[nodiscard]] uint16_t read_u16_le(const uint8_t* ptr);
    //[[nodiscard]] uint16_t read_u16_be(const uint8_t* ptr);
    //[[nodiscard]] bool isPrintable(const uint8_t *start_ptr, const uint8_t *end_ptr);
    //[[nodiscard]] std::map<std::string, std::vector<uint8_t>> getHashVariationsBytes(const std::string &hex_hash);

    //[[nodiscard]] std::vector<size_t> findBytePattern(std::vector<byte> &pattern);
public:
    // Constructors
    [[nodiscard]] LocPackBinFile();
    [[nodiscard]] explicit LocPackBinFile(const std::string &pathString);

    bool loadBinaryFile(const std::string &pathString);

    [[nodiscard]] static std::string convertHash(const std::string &hash);
    [[nodiscard]] BlockInfo getTextByHash(const std::string &hash) const;

    void updateEntry(const std::string &hexHash, int val1, int val2, const std::string &newText) const;

    std::filesystem::path getPath() const;
    void setPath(const std::string &pathString);
};


#endif //AFOP_TRANSLATOR_BACKEND_LOCPACKBINFILE_H