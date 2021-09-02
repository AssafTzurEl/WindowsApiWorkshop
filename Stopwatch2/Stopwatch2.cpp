#include <Windows.h>
#include <iostream>
#include <sstream>

class StopWatchException : public std::exception
{
public:
    StopWatchException(DWORD errorCode)
        : std::exception(GetErrorCodeString(errorCode).c_str()), m_errorCode(errorCode)
    {}

private:
    static std::string GetErrorCodeString(DWORD errorCode)
    {
        std::stringstream strm;

        strm << "Error #" << errorCode << " encountered";

        return strm.str();
    }

    DWORD m_errorCode;
};

// See https://docs.microsoft.com/en-us/windows/win32/sysinfo/acquiring-high-resolution-time-stamps
// and https://docs.microsoft.com/en-us/windows/win32/api/profileapi/nf-profileapi-queryperformancecounter
class StopWatch
{
public:
    StopWatch()
        : m_started(false)
    {
        if (!QueryPerformanceFrequency(&m_frequency))
        {
            throw StopWatchException(GetLastError());
        }
    }

    void Start()
    {
        // In this implementation, we allow double-starting
        QueryPerformanceCounter(&m_startTick); // Skip return value check
        m_started = true;
    }

    double Stop()
    {
        if (m_started)
        {
            m_started = false;
            return Elapsed();
        }

        // else
        return 0;
    }

    double Elapsed()
    {
        LARGE_INTEGER currentTick, elapsedTicks;
        QueryPerformanceCounter(&currentTick); // Skip return value check
        elapsedTicks.QuadPart = currentTick.QuadPart - m_startTick.QuadPart;
        double elapsedSeconds = static_cast<double>(elapsedTicks.QuadPart) / m_frequency.QuadPart;
        return elapsedSeconds;
    }

    void Reset()
    {
        m_started = false;
    }

private:
    bool m_started;
    LARGE_INTEGER m_frequency;
    LARGE_INTEGER m_startTick;
};

int main()
{
    StopWatch sw;

    sw.Start();
    for (size_t count = 0; count < 10'000'000; ++count)
        ;
    std::cout << "After for(10,000,000), elapsed = " << sw.Stop() << std::endl;

    sw.Start();
    Sleep(100);
    std::cout << "After Sleep(100), elapsed = " << sw.Stop() << std::endl;

    sw.Start();
    Sleep(0);
    std::cout << "After Sleep(0), elapsed = " << sw.Stop() << std::endl;

    sw.Start();
    Sleep(1);
    std::cout << "After Sleep(1), elapsed = " << sw.Stop() << std::endl;

    sw.Start();
    Sleep(1000);
    std::cout << "After Sleep(1000), elapsed = " << sw.Stop() << std::endl;

    return 0;
}
