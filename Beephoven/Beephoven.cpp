#include <Windows.h>
#include <iostream>

int main()
{
    Beep(100, 1); // "warm-up"

    for (size_t count = 0; count < 3; ++count)
    {
        Beep(391, 150);
        Sleep(50);
    }

    Beep(311, 1500);

    return 0;
}
