#ifndef PINGIMAGE_HPP
#define PINGIMAGE_HPP

#include <string>
#include <set>

#include "PingChunk.hpp"
#include "PingIHDR.hpp"
#include "PingParseError.hpp"

class PingImage
{
private:
	std::string mFilePath;
	std::vector<char> mData;
	std::vector<PingChunk*> mChunks;
	PingIHDR* mIHDR = NULL;
public:
	const static size_t PNG_HEADER_LENGTH = 8;
	const static size_t MINIMUM_PNG_SIZE = PNG_HEADER_LENGTH + PingChunk::MINIMUM_CHUNK_LENGTH;
	const std::vector<char> PNG_SIGNATURE = {-119, 80, 78, 71, 13, 10, 26, 10};

	PingImage(std::string filePath);
	void readFile();
	void readChunks();
	void verifySignature();

	const std::vector<PingChunk*>& chunks();
	PingIHDR* ihdr();
};

#endif // PINGIMAGE_HPP
