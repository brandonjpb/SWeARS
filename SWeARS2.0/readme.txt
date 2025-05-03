SWeARS 2.0

Simplified Weapons Assembler for Red Shift 2.0
(1.0 went out to get smokes and has not returned)

Weapon Crafting System for Lazy Geniuses
Compilation
bash

g++ -std=c++17 *.cpp -o swears && ./swears

OR if it whines about missing includes:
bash

g++ -std=c++17 main.cpp menu.cpp weaponCreator.cpp partdb.cpp save.cpp -o swears && ./swears

Commands

    1/search - Find parts

    2/list - Show all parts

    3/add - Add new parts (appends to parts.bin)

    4/delete - Delete parts

    5/create - Build weapons (saves as [name].txt)

    6/save - Manually append to parts.bin

    7/exit - Save & quit
