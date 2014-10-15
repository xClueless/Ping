#ifndef PINGIMAGEBUFFER_HPP
#define PINGIMAGEBUFFER_HPP

#include "PingIHDR.hpp"

#include <QImage>
#include <vector>

class PingImageBuffer
{
private:
	std::vector<unsigned char> mDecompressedImageData;
	void zlibDeflate(const std::vector<std::vector<char>*>& compressedData);

public:
	PingImageBuffer();
	void readCompressedData(const std::vector<std::vector<char>*>& compressedDataSets, CompressionMethod cm);
	QImage buildQImage(PingIHDR* ihdr);
};

#endif // PINGIMAGEBUFFER_HPP
