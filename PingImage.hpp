#ifndef PINGIMAGE_HPP
#define PINGIMAGE_HPP

#include <string>
#include <set>

#include "PingChunk.hpp"
#include "PingIHDR.hpp"
#include "PingImageBuffer.hpp"
#include "PingParseError.hpp"

class PingImage
{
private:
	std::string mFilePath;
	std::vector<char> mData;
	std::vector<PingChunk*> mChunks;
	std::vector<std::vector<char>*> mIDATData;
	PingIHDR* mIHDR = NULL;
	PingImageBuffer mImageBuffer;
public:
	const static size_t PNG_HEADER_LENGTH = 8;
	const static size_t MINIMUM_PNG_SIZE = PNG_HEADER_LENGTH + PingChunk::MINIMUM_CHUNK_LENGTH;
	const std::vector<char> PNG_SIGNATURE = {-119, 80, 78, 71, 13, 10, 26, 10};

	PingImage(std::string filePath);
	void readFile();
	void readChunks();
	void readImage();
	void verifySignature();

	const std::vector<PingChunk*>& chunks();
	PingIHDR* ihdr();
	PingImageBuffer& imageBuffer();
};

#endif // PINGIMAGE_HPP
