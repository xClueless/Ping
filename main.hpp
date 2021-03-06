#ifndef MAIN_HPP
#define MAIN_HPP

#include <string>

struct options
{
	std::string filename;
	bool printIHDR;
	bool printHeaders;
	bool printData;
	bool displayImage;
};

options processArguments(int argc, char* argv[]);

#endif // MAIN_HPP
