#include "libraries/rapidcsv.h"
#include <iostream>

#include "LocPackBinFile.h"
#include "LocPackFile.h"
#include "Server.h"

void performFullUpdate(LocPackFile& csvFile, LocPackBinFile& binFile,
                       const std::string& hash, int val1, int val2, const std::string& text) {

    std::cout << "Updating entry: " << hash << "..." << std::endl;

    // Update the Binary file (rebuilds the buffer and handles variable length)
    binFile.updateEntry(hash, val1, val2, text);

    // Update the CSV file
    csvFile.updateEntry(hash, val1, val2, text);

    std::cout << "Update successful!" << std::endl;
}

int main()
{
    try {
        LocPackServer server("menus.locpack", "menus.locpackbin");
        std::cout << "Starting server on port 18080..." << std::endl;
        server.run(18080);
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
