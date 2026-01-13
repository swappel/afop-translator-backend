#include "../../include/LocPackFile.h"

/**
 * This is an empty constructor
 */
LocPackFile::LocPackFile() = default;

/**
 * A constructor that initializes the LocPackFile object
 *
 * @param pathString
 */
LocPackFile::LocPackFile(const std::string &pathString) {
    validateAndLoad(pathString);
}

void LocPackFile::validateAndLoad(const std::string &pathString) {
    const auto testPath = std::filesystem::path(pathString);

    // Checking if the path is valid
    if (!std::filesystem::exists(testPath)) throw std::runtime_error("Path '" + pathString + "' does not exist.");
    if (std::filesystem::is_directory(testPath)) throw std::runtime_error("Path '" + pathString + "' is a directory. Should be a file.");
    if (testPath.extension() != ".locpack") throw std::runtime_error("File '" + pathString + "' has an incorrect extension. Should be '.locpack'");

    filePath = testPath;

    // Loading the CSV lines into the Document object
    auto newDoc = std::make_unique<rapidcsv::Document>(
        filePath.string(),
        rapidcsv::LabelParams(-1, -1),
        rapidcsv::SeparatorParams(',', false, true, true, false)
    );
    doc = std::move(newDoc);
    lastLoadTime = std::filesystem::last_write_time(filePath);
}

/*
 Method used to check if the currently loaded document is still up to date

 Returns false if the document that tries ot be loaded does not exist
 */
bool LocPackFile::refreshDocument() const
{
    if (!std::filesystem::exists(filePath))
    {
        return false;
    }

    if (const auto currentModTime = std::filesystem::last_write_time(filePath); currentModTime > lastLoadTime)
    {
        std::cerr << "File at path '" << filePath.string() << "' modified. Reloading..." << std::endl;

        auto newDoc = std::make_unique<rapidcsv::Document>(
            filePath.string(),
            rapidcsv::LabelParams(-1, -1),
            rapidcsv::SeparatorParams(',', false, true, true, false)
        );

        this->doc = std::move(newDoc);
        this->lastLoadTime = currentModTime;
    }

    return true;
}

std::filesystem::path LocPackFile::getPath() const {
    return filePath;
}

void LocPackFile::setPath(const std::string &pathString)
{
    validateAndLoad(pathString);
}

/*
 Used to fetch multiple lines of the .locpack file

 Returns a vector with the requested information.
 If unsuccessful, throws a runtime error
 */
std::vector<LocaleLine> LocPackFile::parseLocPackRange(const int offset, const int amount) const
{
    // Making sure the document we read is up to date
    if (!refreshDocument())
    {
        throw std::runtime_error("Reloading file at path '" + filePath.string() + "' failed.");
    }

    std::vector<LocaleLine> lines;
    lines.reserve(amount);

    // Read the lines
    for (size_t i = 0; i < doc->GetRowCount(); ++i) {
        std::vector<std::string> readStrings = doc->GetRow<std::string>(i);

        // Skip empty or malformed rows
        if (readStrings.size() < 4) continue;

        try {
            // Remove \r
            readStrings[3].erase(std::ranges::remove(readStrings[3], '\r').begin(), readStrings[3].end());

            // This is where it crashes on the header ("Val1")
            // The try-catch will now catch it and just 'continue' to the next line
            int v1 = std::stoi(readStrings[1]);
            int v2 = std::stoi(readStrings[2]);

            lines.push_back(LocaleLine{ readStrings[0], readStrings[3], v1, v2 });
        }
        catch (...) {
            // If stoi fails (like on the header row), just skip this row and keep going
            continue;
        }
    }

    return lines;
}

/*
 Used to fetch all lines of the .locpack file

 Returns a vector with the requested information.
 If unsuccessful, throws a runtime error
 */
std::vector<LocaleLine> LocPackFile::parseLocPackWhole() const
{
    // Making sure the document we read is up to date
    if (!refreshDocument())
    {
        throw std::runtime_error("Reloading file at path '" + filePath.string() + "' failed.");
    }

    std::vector<LocaleLine> lines;
    const int lineCount = doc->GetRowCount();
    lines.reserve(lineCount);

    // Read the lines
    for (int i = 0; i < lineCount; ++i)
    {
        // Get the row
        std::vector<std::string> readStrings = doc->GetRow<std::string>(i);

        // Reformating string to remove \r from it
        readStrings[3].erase(std::ranges::remove(readStrings[3], '\r').begin(), readStrings[3].end());

        // Put the result at the end of the retrieve lines
        lines.emplace_back(readStrings[0], readStrings[3], std::stoi(readStrings[1]), std::stoi(readStrings[2]));
    }

    return lines;
}

/*
 * Tries to find the index of a line where the given hash is found
 *
 * Returns -1 if no line was found
 */
int LocPackFile::findHashIndex(const std::string& hash) const
{
    // Making sure the document we read is up to date
    if (!refreshDocument())
    {
        throw std::runtime_error("Reloading file at path '" + filePath.string() + "' failed.");
    }

    // Going through the entries to try and find the requested hash
    int foundIndex = -1;
    for (int i = 0; i < doc->GetRowCount(); ++i)
    {
        // If the first cell of the row equals the given hash, set the foundIndex to current index and exit loop
        if (const std::vector<std::string> entryHash = doc->GetRow<std::string>(i); entryHash[0] == hash)
        {
            foundIndex = i;
            break;
        }
    }

    return foundIndex;
}

/*
 * Uses a hash as input and returns a LocalLine object with the data of the line
 *
 * Throws an error if the hash could not be found
 */
LocaleLine LocPackFile::findHashComplete(const std::string& hash) const
{
    const int foundIndex = findHashIndex(hash);

    // If no hash was found throw runtime error
    if (foundIndex == -1)
    {
        throw std::runtime_error("Hash not found in the LocPack file: " + hash);
    }

    // Reading the row with the found index from the hash
    std::vector<std::string> readStrings = doc->GetRow<std::string>(foundIndex);

    // Reformating string to remove \r from it.
    readStrings[3].erase(std::ranges::remove(readStrings[3], '\r').begin(), readStrings[3].end());

    return LocaleLine{readStrings[0], readStrings[3], std::stoi(readStrings[1]), std::stoi(readStrings[2])};
}

void LocPackFile::updateEntry(const std::string& hash, int character, int unknown, const std::string& newContent) {
    // 1. Ensure the document is loaded and up to date
    if (!doc) {
        throw std::runtime_error("CSV document not initialized.");
    }
    refreshDocument();

    // 2. Find the row index for the given hash
    int rowIndex = findHashIndex(hash);
    if (rowIndex == -1) {
        throw std::runtime_error("Hash not found in .locpack: " + hash);
    }

    // 3. Prepare the new row data
    // Format: Hash, Value1, Value2, String
    std::vector<std::string> newRow;
    newRow.push_back(hash);
    newRow.push_back(std::to_string(character));
    newRow.push_back(std::to_string(unknown));
    newRow.push_back(newContent);

    // 4. Update the row in memory
    doc->SetRow(rowIndex, newRow);

    // 5. Save the changes to the file
    doc->Save();
}
