#include <iostream>
#include <Windows.h>
#include <shlwapi.h>

int main(size_t argc, char *argv[])
{
    const size_t BUFFER_SIZE = 512;

    if (argc != 3)
    {
        std::cout << "Usage:  FileCopy <src> <dest>" << std::endl;

        return 1;
    }

    const char *source = argv[1];
    const char *dest = argv[2];

    if (!PathFileExists(source))
    {
        std::cout << "Source file not found" << std::endl;

        return 2;
    }

    HANDLE hSource = CreateFile(source, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL, NULL);
    HANDLE hDest = CreateFile(dest, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL,
        NULL);

    // Check for errors: NULL handles, GetLastError, etc.

    char buffer[BUFFER_SIZE];
    DWORD bytesRead;
    DWORD bytesWritten;

    do
    {
        if (ReadFile(hSource, buffer, BUFFER_SIZE, &bytesRead, NULL))
        {
            if (!WriteFile(hDest, buffer, bytesRead, &bytesWritten, NULL) ||
                bytesRead != bytesWritten)
            {
                // error
            }
        }
        // else - error
    } while (BUFFER_SIZE == bytesRead);

    CloseHandle(hDest); // todo: check errors
    CloseHandle(hSource); // todo: check errors
    
    return 0;
}
