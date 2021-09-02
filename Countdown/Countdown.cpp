#include <Windows.h>
#include <iostream>

int main()
{
	for (size_t counter = 10; counter > 0; --counter)
	{
		std::cout << counter << std::endl;
		Sleep(INFINITE);
	}

	std::cout << "Liftoff!" << std::endl;

	return 0;
}