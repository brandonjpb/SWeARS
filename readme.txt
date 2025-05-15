SWeARS 2.0

Simplified Weapons Assembler for Red Shift 2.0
(1.0 went out to get smokes and has not returned)

Weapon Crafting System for Lazy Geniuses
Compilation

To run the program, run the following from the root directory:

rm -r -force build; mkdir build; cd build; cmake .. -G "Visual Studio 17 2022" -A x64; cmake --build . --config Debug; .\Debug\SWeARS2.0.exe

Commands

    1/search - Find parts

    2/list - Show all parts

    3/add - Add new parts (appends to parts.bin)

    4/delete - Delete parts

    5/create - Build weapons (saves as [name].txt)

    6/save - Manually append to parts.bin

    7/exit - Save & quit

