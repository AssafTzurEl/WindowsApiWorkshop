#include <Windows.h>
#include <iostream>

void CreateEvent_NoErrors()
{
    std::cout << "\nCreating a normal event." << std::endl;
    HANDLE eventHandle = CreateEvent(nullptr, false, false, nullptr);

    std::cout << "Event created successfully? " << (eventHandle != NULL) << std::endl;

    CloseHandle(eventHandle);
}

std::string GetLastWinApiError()
{
    DWORD err = ::GetLastError();
    LPVOID lpMsgBuf;

    ::FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        err,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
        (LPSTR) (&lpMsgBuf),
        0,
        NULL);

    std::string result(static_cast<char *>(lpMsgBuf));

    // Free the buffer.
    LocalFree(lpMsgBuf);

    return result;
}

void CloseEventHandle(HANDLE eventHandle)
{
    auto closed = CloseHandle(eventHandle);

    if (closed)
    {
        std::cout << "Event closed successfully" << std::endl;
    }
    else
    {
        std::cout << "Event closed with error: " << GetLastWinApiError() << std::endl;
    }
}

void CreateEvent_AlreadyExists()
{
    std::cout << "\nCreating two named events." << std::endl;
    const TCHAR *eventName = TEXT("myevent");

    HANDLE event1 = CreateEvent(nullptr, false, false, eventName);
    std::cout << "Last error: " << GetLastWinApiError() << std::endl;

    HANDLE event2 = CreateEvent(nullptr, false, false, eventName);
    std::cout << "Last error: " << GetLastWinApiError() << std::endl;

    CloseEventHandle(event2);

    CloseEventHandle(event1);
}

void CloseEvent_ShouldFail()
{
    std::cout << "\nClosing a null event." << std::endl;
    CloseEventHandle(NULL);
}

int main()
{
    CreateEvent_NoErrors();
    CreateEvent_AlreadyExists();
    CloseEvent_ShouldFail();

    return 0;
}
