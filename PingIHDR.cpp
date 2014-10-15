#include "PingIHDR.hpp"
#include "PingUtil.hpp"
#include "PingParseError.hpp"
#include <iostream>
#include <sstream>

using namespace std;

PingIHDR::PingIHDR(vector<char>& data) : mData(data)
{

}

uint32_t PingIHDR::width() const
{
	return read_png_uint32(&mData[WIDTH_INDEX]);
}

uint32_t PingIHDR::height() const
{
	return read_png_uint32(&mData[HEIGHT_INDEX]);
}

uint8_t PingIHDR::bitDepth() const
{
	return mData[BIT_DEPTH_INDEX];
}

ColourType PingIHDR::colourType() const
{
	switch(mData[COLOUR_TYPE_INDEX])
	{
		case GRAYSCALE: return GRAYSCALE;
		case RGB: return RGB;
		case INDEXED: return INDEXED;
		case GRAYSCALE_ALPHA: return GRAYSCALE_ALPHA;
		case RGB_ALPHA: return RGB_ALPHA;
		default: throw PingParseError("Colour type is unknown.");
	}
}
string PingIHDR::colourTypeString() const
{
	switch(mData[COLOUR_TYPE_INDEX])
	{
		case GRAYSCALE: return "GRAYSCALE";
		case RGB: return "RGB";
		case INDEXED: return "INDEXED";
		case GRAYSCALE_ALPHA: return "GRAYSCALE_ALPHA";
		case RGB_ALPHA: return "RGB_ALPHA";
	}
	stringstream ss;
	ss << "Unknown colour type '" << (int) mData[COLOUR_TYPE_INDEX] << "'";
	return ss.str();
}

CompressionMethod PingIHDR::compressionMethod() const
{
	switch(mData[COMPRESSION_METHOD_INDEX])
	{
		case DEFLATE: return DEFLATE;
		default: throw PingParseError("Compression method is unknown.");
	}
}
string PingIHDR::compressionMethodString() const
{
	switch(mData[COMPRESSION_METHOD_INDEX])
	{
		case DEFLATE: return "DEFLATE";
	}
	stringstream ss;
	ss << "Unknown compression method '" << (int) mData[COMPRESSION_METHOD_INDEX] << "'";
	return ss.str();
}

FilterMethod PingIHDR::filterMethod() const
{
	switch(mData[FILTER_METHOD_INDEX])
	{
		case ADAPTIVE_FIVE: return ADAPTIVE_FIVE;
		default: throw PingParseError("Filter method is unknown.");
	}
}
string PingIHDR::filterMethodString() const
{
	switch(mData[FILTER_METHOD_INDEX])
	{
		case ADAPTIVE_FIVE: return "ADAPTIVE_FIVE";
	}
	stringstream ss;
	ss << "Unknown filter method '" << (int) mData[FILTER_METHOD_INDEX] << "'";
	return ss.str();
}

InterlaceMethod PingIHDR::interlaceMethod() const
{
	switch(mData[INTERLACE_METHOD_INDEX])
	{
		case NO_INTERLACE: return NO_INTERLACE;
		case ADAM7: return ADAM7;
		default: throw PingParseError("Interlace method is unknown.");
	}
}
string PingIHDR::interlaceMethodString() const
{
	switch(mData[INTERLACE_METHOD_INDEX])
	{
		case NO_INTERLACE: return "NO_INTERLACE";
		case ADAM7: return "ADAM7";
	}
	stringstream ss;
	ss << "Unknown interlace method '" << (int) mData[INTERLACE_METHOD_INDEX] << "'";
	return ss.str();
}

void PingIHDR::print() const
{
	cout << endl << "IHDR" << endl;
	cout << "===================================" << endl;
	cout << "W/H: " << width() << "x" << height() << endl;
	cout << "Bit Depth: " << static_cast<unsigned>(bitDepth()) << endl;
	cout << "Colour Type: " << colourTypeString() << endl;
	cout << "Compression Method: " << compressionMethodString() << endl;
	cout << "Filter Method: " << filterMethodString() << endl;
	cout << "Interlace Method: " << interlaceMethodString() << endl;
}
