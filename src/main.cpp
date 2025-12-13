#include "libraries/rapidcsv.h"
#include <iostream>

#include "LocPackFile.h"

int main()
{
    const auto locPack = LocPackFile{"/mnt/massive/Coding/CPP/afop-translator-backend/cmake-build-release/src/menus.locpack"};

    for (const std::vector<LocaleLine> testRange= locPack.parseLocPackRange(3, 200); auto line : testRange)
    {
        std::cout << line.getContent() << "\n_________________\n";
    }

    return 0;
}
