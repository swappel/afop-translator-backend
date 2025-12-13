#include "libraries/rapidcsv.h"
#include <iostream>

#include "LocPackFile.h"

int main()
{
    const auto locPack = LocPackFile{"./menus.locpack"};

    const LocaleLine inventoryFull = locPack.findHashComplete("BFB129F6593159310000650DEC6EFD72");

    std::cout << inventoryFull.getContent();

    return 0;
}
