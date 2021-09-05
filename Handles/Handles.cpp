#include <Windows.h>
#include <iostream>
#include <string>

std::string GetLastWinApiError()
{
    DWORD err = ::GetLastError();

    //return std::to_string(err);

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
        std::string error = GetLastWinApiError();

        std::cout << "Event closed with error: " << error << std::endl;
    }
}

void CreateEvent_NoErrors()
{
    std::cout << "\nCreating a normal event." << std::endl;
    HANDLE eventHandle = CreateEvent(nullptr, false, false, nullptr);

    if (eventHandle != NULL)
    {
        std::cout << "Event created successfully" << std::endl;

        CloseEventHandle(eventHandle);
    }
    else
    {
        DWORD error = GetLastError();
        std::cout << "Error #" << error << ": Event not created" << std::endl;
    }
}

void CreateEvent_AlreadyExists()
{
    std::cout << "\nCreating two named events." << std::endl;
    const TCHAR *eventName = TEXT("myevent");

    HANDLE event1 = CreateEvent(nullptr, false, false, eventName);
    std::string error = GetLastWinApiError();

    std::cout << "Last error: " << error << std::endl;

    HANDLE event2 = CreateEvent(nullptr, false, false, eventName);

    error = GetLastWinApiError();
    std::cout << "Last error: " << error << std::endl;

    CloseEventHandle(event2);

    CloseEventHandle(event1);
}

void CloseNullHandle_ShouldFail()
{
    std::cout << "\nClosing a null event." << std::endl;
    CloseEventHandle(NULL);
}

void CloseHandleTwice_ShouldFail()
{
    std::cout << "\nCreating an event and closing it twice." << std::endl;
    HANDLE eventHandle = CreateEvent(nullptr, false, false, nullptr);

    if (eventHandle != NULL)
    {
        std::cout << "Event created successfully" << std::endl;

        std::cout << "Closing once:" << std::endl;
        CloseEventHandle(eventHandle);
        std::cout << "Closing twice:" << std::endl;
        CloseEventHandle(eventHandle); // Crash!? RTFM!
    }
    else
    {
        std::string error = GetLastWinApiError();

        std::cout << "Last error: " << error << std::endl;
    }
}


int main()
{
    CreateEvent_NoErrors();
    CreateEvent_AlreadyExists();
    CloseNullHandle_ShouldFail();
    CloseHandleTwice_ShouldFail();

    return 0;
}
