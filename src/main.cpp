#include "libraries/rapidcsv.h"
#include <iostream>

#include "LocPackBinFile.h"
#include "LocPackFile.h"

int main()
{
    const auto locPack = LocPackFile{"./menu.locpack"};
    const auto locPackBin = LocPackBinFile("./menu.locpackbin");

    for (const std::vector<LocaleLine> testRange= locPack.parseLocPackRange(3, 200); auto line : testRange)
    {
        std::string convertedHash = LocPackBinFile::convertHash(line.getHash());

        std::cout << "Hash in .locpack: " << line.getHash() << "\nConverted hash:   " << convertedHash << std::endl;

        std::cout << "Content in .locpack:    " << line.getContent() << "\n"
                  << "Content in .locpackbin: " << locPackBin.getTextByHash(convertedHash).text;
    }

    return 0;
}
