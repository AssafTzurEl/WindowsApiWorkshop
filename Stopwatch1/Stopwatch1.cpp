#include <Windows.h>
#include <iostream>

// See https://docs.microsoft.com/en-us/windows/win32/api/sysinfoapi/nf-sysinfoapi-gettickcount
class Stopwatch
{
public:
    Stopwatch()
        : m_started(false)
    {}

    void Start()
    {
        // In this implementation, we allow double-starting
        m_lastTick = GetTickCount();
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
        return (GetTickCount() - m_lastTick) / 1000.0;
    }

    void Reset()
    {
        m_started = false;
    }

private:
    bool m_started;
    DWORD m_lastTick;
};


int main()
{
    Stopwatch sw;

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
