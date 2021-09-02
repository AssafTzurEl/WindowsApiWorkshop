#include <iostream>
#include <Windows.h>

int main()
{
    int button = MessageBoxA(NULL, "This is the message", "This is the title", MB_OKCANCEL);
}
