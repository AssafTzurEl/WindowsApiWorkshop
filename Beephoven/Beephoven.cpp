#include <Windows.h>
#include <iostream>

int main()
{
    Beep(100, 1); // "warm-up"

    for (size_t count = 0; count < 3; ++count)
    {
        Beep(391, 150); // G/Sol
        Sleep(50);
    }

    Beep(311, 1500); // D#/Re#

    // Next: F F F D (349 and 294 Hz)

    return 0;
}
