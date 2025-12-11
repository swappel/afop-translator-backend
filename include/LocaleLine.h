//
// Created by swappel on 12/11/25.
//

#ifndef AFOP_TRANSLATOR_BACKEND_LOCALELINE_H
#define AFOP_TRANSLATOR_BACKEND_LOCALELINE_H
#include <string>


class LocaleLine {
private:
    std::string m_hash;
    std::string m_convertedHash;
    int m_character;
    int m_unknown;
    std::string m_content;
    std::string m_convertedContent;

    void convertHash();
    void convertContent();

public:
    LocaleLine(const std::string &hash, const std::string &convertedHash, const std::string &content, int character, int unknown);
    [[nodiscard]] std::string getHash() const;
    [[nodiscard]] std::string getConvertedHash() const;
    [[nodiscard]] std::string getContent() const;
    [[nodiscard]] std::string getConvertedContent() const;
    [[nodiscard]] int getCharacter() const;
    [[nodiscard]] int getUnknown() const;

    void setCharacter(int &character);
    void setUnknown(const int &unknown);
    void setContent(const std::string &content);
    void setConvertedContent(const std::string &content);
    void setHash(const std::string &hash);
    void setConvertedHash(const std::string &convertedHash);

};


#endif //AFOP_TRANSLATOR_BACKEND_LOCALELINE_H