//
// Created by elanda on 12/11/25.
//

#include "../../include/LocaleLine.h"

void LocaleLine::convertHash() {

}

void LocaleLine::convertContent() {

}

int LocaleLine::getCharacter() const {
    return m_character;
}

std::string LocaleLine::getContent() const {
    return m_content;
}

std::string LocaleLine::getConvertedContent() const {
    return m_convertedContent;
}

std::string LocaleLine::getConvertedHash() const {
    return m_convertedHash;
}

std::string LocaleLine::getHash() const {
    return m_hash;
}

int LocaleLine::getUnknown() const {
    return m_unknown;
}

void LocaleLine::setCharacter(int &character) {
    m_character = character;
}

void LocaleLine::setUnknown(const int &unknown) {
    m_unknown = unknown;
}

void LocaleLine::setContent(const std::string &content) {
    m_content = content;
}

void LocaleLine::setConvertedContent(const std::string &content) {
    m_convertedContent = content;
}

void LocaleLine::setHash(const std::string &hash) {
    m_hash = hash;
}

void LocaleLine::setConvertedHash(const std::string &convertedHash) {
    m_convertedHash = convertedHash;
}

LocaleLine::LocaleLine(const std::string &hash,
                       const std::string &convertedHash,
                       const std::string &content,
                       const int character,
                       const int unknown) {
    this->m_hash = hash;
    this->m_convertedHash = convertedHash;
    this->m_content = content;
    this->m_character = character;
    this->m_unknown = unknown;
}
