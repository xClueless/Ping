#ifndef PINGIMAGEBUFFER_HPP
#define PINGIMAGEBUFFER_HPP

#include "PingIHDR.hpp"

#include <QImage>
#include <vector>

class PingImageBuffer
{
private:
	std::vector<uint8_t> mDecompressedImageData;
	void zlibDeflate(const std::vector<std::vector<char>*>& compressedData);
	uint8_t* getRowPointer(size_t pos);
	const PingIHDR* mIHDR;
public:
	PingImageBuffer();
	void setIHDR(const PingIHDR* ihdr);
	void readCompressedData(const std::vector<std::vector<char>*>& compressedDataSets);
	QImage buildQImage();
};

#endif // PINGIMAGEBUFFER_HPP
