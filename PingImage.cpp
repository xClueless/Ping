#include "PingImage.hpp"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>

using namespace std;

PingImage::PingImage(string filePath) : mFilePath(filePath)
{

}

void PingImage::readFile()
{
	ifstream fileIn(mFilePath, ios::in | ios::binary | ios::ate);
	if(!fileIn.is_open())
	{
		throw PingParseError("Failed to open file.");
	}

	size_t fileSize = fileIn.tellg();

	if(fileSize < MINIMUM_PNG_SIZE)
	{
		fileIn.close();
		throw PingParseError("File is too small to be a valid PNG");
	}
	mData.resize(fileSize);

	fileIn.seekg(0, ios::beg);
	fileIn.read(&mData[0], fileSize);
	fileIn.close();
}

void PingImage::readChunks()
{
	vector<char>::iterator chunkStart = mData.begin() + PNG_SIGNATURE.size();

	while(distance(chunkStart, mData.end()) >= PingChunk::MINIMUM_CHUNK_LENGTH)
	{
		mChunks.push_back(new PingChunk);
		PingChunk* chunk = mChunks.back();

		chunk->readHeaderLeft(chunkStart, chunkStart+PingChunk::MINIMUM_CHUNK_LENGTH);
		vector<char>::iterator chunkEnd = chunkStart + chunk->chunkLength();

		//We know where the CRC is now, might as well read it even if we don't understand the chunk data.
		chunk->readCRC(chunkStart, chunkEnd);

		//Read and use the chunk data if we understand the chunk type. There's no point in copying the data if we don't understand it.
		if(chunk->name() == "IHDR" || chunk->name() == "IDAT")
		{
			chunk->readData(chunkStart, chunkEnd);

			if(chunk->name() == "IHDR")
			{
				if(mIHDR != NULL)
				{
					throw PingParseError("Found a second IHDR chunk. Ignoring it.");
				}
				mIHDR = new PingIHDR(chunk->data());
			}
			else if(chunk->name() == "IDAT")
			{
				mIDATData.push_back(&chunk->data());
			}
		}
		else if(chunk->name() == "IEND")
		{
			break;
		}
		else if(chunk->isCritical())
		{
			throw PingParseError("Cannot understand critical chunk '" + chunk->name() + "'");
		}
		else
		{
			cout << "Ignoring non-critical meaningless chunk '" << chunk->name() << "'" << endl;
		}

		advance(chunkStart, chunk->chunkLength());
	}
}

void PingImage::readImage()
{
	if(mIHDR == NULL)
	{
		throw PingParseError("Cannot read the image without an IHDR chunk. Make sure you call readChunks first.");
	}
	if(mIDATData.empty())
	{
		throw PingParseError("Cannot read the image without any IDAT chunks. Make sure you call readChunks first.");
	}
	mImageBuffer.readCompressedData(mIDATData, mIHDR->compressionMethod());
}

void PingImage::verifySignature()
{
	vector<char> fileSignature(mData.begin(), mData.begin() + PNG_SIGNATURE.size());

	if(fileSignature != PNG_SIGNATURE)
	{
		throw PingParseError("Invalid PNG header.");
	}
}

const std::vector<PingChunk*>&PingImage::chunks()
{
	return mChunks;
}

PingIHDR* PingImage::ihdr()
{
	return mIHDR;
}

PingImageBuffer& PingImage::imageBuffer()
{
	return mImageBuffer;
}
