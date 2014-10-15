#include "PingImageBuffer.hpp"
#include "PingParseError.hpp"

#include <iostream>
#include <sstream>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/zlib.hpp>

using namespace std;

PingImageBuffer::PingImageBuffer()
{
}

void PingImageBuffer::readCompressedData(const vector<vector<char>*>& compressedDataSets, CompressionMethod cm)
{
	if(cm == DEFLATE)
	{
		zlibDeflate(compressedDataSets);
	}
	else
	{
		throw PingParseError("Cannot decompress image data is the compression method is not understood.");
	}
}

QImage PingImageBuffer::buildQImage(PingIHDR* ihdr)
{
	QImage::Format fmt = QImage::Format_ARGB32;
	switch(ihdr->colourType())
	{
		case RGB: fmt = QImage::Format_RGB32; break;
		case RGB_ALPHA: fmt = QImage::Format_ARGB32; break;
		case GRAYSCALE: fmt = QImage::Format_Mono; break;
		case INDEXED: fmt = QImage::Format_Indexed8; break;
	}

//	QImage img(ihdr->width(), ihdr->height(), fmt);
	QImage img(&mDecompressedImageData[0], ihdr->width(), ihdr->height(), fmt);
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
