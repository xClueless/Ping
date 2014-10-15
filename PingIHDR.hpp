#ifndef PINGIHDR_HPP
#define PINGIHDR_HPP

#include <vector>
#include "stdint.h"
#include <string>

enum ColourType
{
	GRAYSCALE = 0,
	RGB = 2,
	INDEXED = 3,
	GRAYSCALE_ALPHA = 4,
	RGB_ALPHA = 6
};

enum CompressionMethod
{
	DEFLATE = 0
};

enum FilterMethod
{
	ADAPTIVE_FIVE = 0
};

enum InterlaceMethod
{
	NO_INTERLACE = 0,
	ADAM7 = 1
};

class PingIHDR
{
private:
	const size_t WIDTH_INDEX = 0; //0-3
	const size_t HEIGHT_INDEX = 4; //4-7
	const size_t BIT_DEPTH_INDEX = 8; //8
	const size_t COLOUR_TYPE_INDEX = 9; //9
	const size_t COMPRESSION_METHOD_INDEX = 10; //10
	const size_t FILTER_METHOD_INDEX = 11; //11
	const size_t INTERLACE_METHOD_INDEX = 12; //12

	std::vector<char>& mData;

public:
	PingIHDR(std::vector<char>& data);

	uint32_t width() const;
	uint32_t height() const;
	uint8_t bitDepth() const;

	ColourType colourType() const;
	std::string colourTypeString() const;

	CompressionMethod compressionMethod() const;
	std::string compressionMethodString() const;

	FilterMethod filterMethod() const;
	std::string filterMethodString() const;

	InterlaceMethod interlaceMethod() const;
	std::string interlaceMethodString() const;

	void print() const;
};

#endif // PINGIHDR_HPP
