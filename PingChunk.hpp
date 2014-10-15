#ifndef PNGCHUNK_HPP
#define PNGCHUNK_HPP

#include <vector>
#include <stdint.h>
#include <string>
#include <iostream>
#include <iterator>
#include <algorithm>
#include "PingParseError.hpp"
#include "PingUtil.hpp"

class PingChunk
{
protected:
	static const size_t LENGTH_FIELD_LENGTH = 4;

	static const size_t NAME_FIELD_LENGTH = 4;
	static const size_t NAME_INDEX_CRITICAL = 0;
	static const size_t NAME_INDEX_PUBLIC = 1;
	static const size_t NAME_INDEX_CONFORM = 2;
	static const size_t NAME_INDEX_SAFE_COPY = 3;
	static const size_t CRC_FIELD_LENGTH = 4;
public:
	static const size_t MINIMUM_CHUNK_LENGTH = LENGTH_FIELD_LENGTH + NAME_FIELD_LENGTH + CRC_FIELD_LENGTH;

protected:
	size_t mDataLength = 0;
	std::string mName;
	std::vector<char> mCRC;
	std::vector<char> mData;
public:
	PingChunk();
	~PingChunk();

	template<class It>
	void readHeaderLeft(It chunkStart, It chunkMinimumEnd)
	{
		if(std::distance(chunkStart, chunkMinimumEnd) < MINIMUM_CHUNK_LENGTH)
		{
			throw PingParseError("Chunk is too small to have a valid header.");
		}

		std::vector<char>::iterator currentField = chunkStart;
		mDataLength = read_png_uint32((char*) &(*currentField));
		std::advance(currentField, 4);
		mName = std::string(currentField, currentField+NAME_FIELD_LENGTH);
	}

	template<class It>
	void readData(It chunkStart, It chunkEnd)
	{
		if(mDataLength == 0)
		{
			throw PingParseError("Chunk Data Length has not been set.");
		}

		std::vector<char>::iterator dataStart = chunkStart + LENGTH_FIELD_LENGTH + NAME_FIELD_LENGTH;
		std::vector<char>::iterator dataEnd = chunkEnd - CRC_FIELD_LENGTH;

		size_t iteratorDataLength = std::distance(dataStart, dataEnd);
		if(iteratorDataLength != mDataLength)
		{
			throw PingParseError("Data length mismatch.");
		}
		mData.resize(mDataLength);
		std::copy(dataStart, dataEnd, mData.begin());
	}

	template<class It>
	void readCRC(It chunkStart, It chunkEnd)
	{
		std::vector<char>::iterator crcStart = chunkStart + LENGTH_FIELD_LENGTH + NAME_FIELD_LENGTH + mDataLength;
		mCRC = std::vector<char>(crcStart, chunkEnd);
	}

	int dataLength() const;
	size_t dataOffset() const;
	std::string name() const;
	std::vector<char> CRC() const;
	std::vector<char>& data();
	const std::vector<char>& constData() const;
	size_t chunkLength() const;
	bool isCritical() const;
	bool isPublic() const;
	bool isConforming() const;
	bool isSafelyCopyable() const;

	void printHeader() const;
	void printData() const;
};

#endif // PNGCHUNK_HPP
