#ifndef PNGPARSEERROR_HPP
#define PNGPARSEERROR_HPP

#include <stdexcept>
#include <string>

class PingParseError : public std::runtime_error
{
public:
	PingParseError(std::string errorDesc);
};

#endif // PNGPARSEERROR_HPP
