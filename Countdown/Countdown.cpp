#include <Windows.h>
#include <iostream>

int main()
{
	for (size_t counter = 10; counter > 0; --counter)
	{
		std::cout << counter << std::endl;
		Sleep(1000);
	}

	std::cout << "Liftoff!" << std::endl;

	return 0;
}