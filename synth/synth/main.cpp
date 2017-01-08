#include <iostream>
#include "synth.h"

int main()
{
	try {
		Synth s;
		s.Run();
		return 0;
	}
	catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}
}