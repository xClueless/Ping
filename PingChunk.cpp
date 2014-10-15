#include "PingChunk.hpp"
#include <iomanip>

using namespace std;

PingChunk::PingChunk()
{

}

int PingChunk::dataLength() const
{
	return mDataLength;
}

size_t PingChunk::dataOffset() const
{
	return LENGTH_FIELD_LENGTH + NAME_FIELD_LENGTH;
}

string PingChunk::name() const
{
	return mName;
}
vector<char> PingChunk::CRC() const
{
	return mCRC;
}

std::vector<char>& PingChunk::data()
{
	return mData;
}

const std::vector<char>& PingChunk::constData() const
{
	return mData;
}

size_t PingChunk::chunkLength() const
{
	return LENGTH_FIELD_LENGTH + NAME_FIELD_LENGTH + mDataLength + CRC_FIELD_LENGTH;
}

bool PingChunk::isCritical() const
{
	return isupper(mName.at(NAME_INDEX_CRITICAL));
}

bool PingChunk::isPublic() const
{
	return isupper(mName.at(NAME_INDEX_PUBLIC));
}

bool PingChunk::isConforming() const
{
	return isupper(mName.at(NAME_INDEX_CONFORM));
}

bool PingChunk::isSafelyCopyable() const
{
	return isupper(mName.at(NAME_INDEX_SAFE_COPY));
}

void PingChunk::printHeader() const
{
	cout << "Length:" << mDataLength << " Type/Name:" << mName << endl;
	cout << "CRC:" << read_png_uint32(&mCRC[0]) << endl;

	std::string criticalStr = isCritical() ? "Yes" : "No";
	std::string publicStr = isPublic() ? "Yes" : "No";
	std::string conformingStr = isConforming() ? "Yes" : "No";
	std::string safeCopyStr = isSafelyCopyable() ? "Yes" : "No";
	cout << "Critical:" << criticalStr << " Public:" << publicStr;
	cout << " Conforming:" << conformingStr << "SafeCopy: " << safeCopyStr << endl;
}

void PingChunk::printData() const
{
	cout << "DATA" << endl;
	for(char c : mData)
	{
		cout << c;
//		cout << setw(2) << setfill('0') << hex << (int) c;
	}
	cout << endl;
}
