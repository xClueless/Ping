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
		mChunks.push_back(PingChunk());
		PingChunk& chunk = mChunks.back();

		chunk.readHeaderLeft(chunkStart, chunkStart+PingChunk::MINIMUM_CHUNK_LENGTH);

		vector<char>::iterator chunkEnd = chunkStart + chunk.chunkLength();

		if(chunk.name() == "IHDR" || chunk.name() == "IDAT")
		{
			chunk.readData(chunkStart, chunkEnd);
			chunk.readCRC(chunkStart, chunkEnd);
		}
		else if(chunk.isCritical())
		{
			throw PingParseError("Cannot understand critical chunk '" + chunk.name() + "'");
		}
		else
		{
			cout << "Ignoring non-critical meaningless chunk '" << chunk.name() << "'" << endl;
		}

		advance(chunkStart, chunk.chunkLength());
	}
}

void PingImage::verifySignature()
{
	vector<char> fileSignature(mData.begin(), mData.begin() + PNG_SIGNATURE.size());

	if(fileSignature != PNG_SIGNATURE)
	{
		throw PingParseError("Invalid PNG header.");
	}
}