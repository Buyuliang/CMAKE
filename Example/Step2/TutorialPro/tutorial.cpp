#include <iostream>
#include <TutorialConfig.h>
#include <MathFunctions.h>
#include <math.h>

 int main(int argc, char* argv[])
{
	int inputValue = 2;
	if (argc < 2) {
		// report version
		std::cout << argv[0] << " Version " << Tutorial_VERSION_MAJOR << "."
				  << Tutorial_VERSION_MINOR << std::endl;
		std::cout << "Usage: " << argv[0] << " number" << std::endl;
		
	#ifdef USE_MYMATH
		const double outputValue = mysqrt(inputValue);
		std::cout << "mysqrt: " << outputValue << std::endl;
	#else
		const double outputValue = sqrt(inputValue);
		std::cout << "sqrt: " << outputValue << std::endl;
	#endif
		return 1;
	}
}