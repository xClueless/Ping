#include "PingImageBuffer.hpp"
#include "PingParseError.hpp"

#include <iostream>
#include <sstream>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/zlib.hpp>

using namespace std;

enum FilterAlgorithms
{
	NO_FILTER = 0,
	SUB_FILTER = 1,
	UP_FILTER = 2,
	AVERAGE_FILTER = 3,
	PAETH_FILTER = 4
};

PingImageBuffer::PingImageBuffer()
{
}

void PingImageBuffer::setIHDR(const PingIHDR* ihdr)
{
	mIHDR = ihdr;
}

void PingImageBuffer::readCompressedData(const vector<vector<char>*>& compressedDataSets)
{
	if(mIHDR->compressionMethod() == DEFLATE)
	{
		zlibDeflate(compressedDataSets);
	}
	else
	{
		throw PingParseError("Cannot decompress image data is the compression method is not understood.");
	}
}

void PingImageBuffer::reverseFilters()
{
	for(size_t currentRowIndex=0;currentRowIndex<mIHDR->height();++currentRowIndex)
	{
		uint8_t* currentRowPtr = getRowPointer(currentRowIndex);

		uint8_t lineFilterAlgorithm = *currentRowPtr;

		switch(lineFilterAlgorithm)
		{
			case NO_FILTER: cout << "[" << currentRowIndex  << "] NO_FILTER." << endl; break;
			case SUB_FILTER: cout << "[" << currentRowIndex  << "] SUB_FILTER." << endl; break;
			case UP_FILTER: cout << "[" << currentRowIndex  << "] UP_FILTER." << endl; break;
			case AVERAGE_FILTER: cout << "[" << currentRowIndex  << "] AVERAGE_FILTER." << endl; break;
			case PAETH_FILTER: cout << "[" << currentRowIndex  << "] PAETH_FILTER." << endl; break;
		}
	}
}

QImage PingImageBuffer::buildQImage()
{
	if(mIHDR == NULL)
	{
		throw PingParseError("Cannot build QImage without an IHDR");
	}
	else if(mDecompressedImageData.empty())
	{
		throw PingParseError("Cannot build QImage without decompressed image data.");
	}

	QImage::Format fmt = QImage::Format_ARGB32;
	switch(mIHDR->colourType())
	{
		case RGB: fmt = QImage::Format_RGB32; break;
		case RGB_ALPHA: fmt = QImage::Format_ARGB32; break;
		case GRAYSCALE: fmt = QImage::Format_Mono; break;
		case INDEXED: fmt = QImage::Format_Indexed8; break;
	}

	QImage img(&mDecompressedImageData[0], mIHDR->width(), mIHDR->height(), fmt);
	return img;
}

void PingImageBuffer::zlibDeflate(const vector<vector<char>*>& compressedData)
{
	//See: http://stackoverflow.com/questions/9119688/boost-gzip-decompress-byte-array
	using namespace boost::iostreams;
	using namespace boost::iostreams::zlib;

	cout << endl << "ZLIB DEFLATE" << endl;
	cout << "===================================" << endl;

	filtering_ostream os;
	os.push(zlib_decompressor());
	os.push(back_inserter(mDecompressedImageData));

	cout << "Decompressing";
	for(vector<char>* compressedDataChunk : compressedData)
	{
		cout << " #";
		boost::iostreams::write(os, &(*compressedDataChunk)[0], compressedDataChunk->size());
	}

	cout << " Done" << endl;
}

uint8_t* PingImageBuffer::getRowPointer(size_t pos)
{
	if(mIHDR == NULL)
	{
		throw PingParseError("Cannot retrieve row pointer without an IHDR");
	}
	else if(mDecompressedImageData.empty())
	{
		throw PingParseError("Cannot retrieve row pointer without decompressed image data.");
	}

	uint8_t* rowPtr = &mDecompressedImageData[0];
	for(size_t i=0;i<pos;++i)
	{
		rowPtr+=mIHDR->width();
	}

	return rowPtr;
}
