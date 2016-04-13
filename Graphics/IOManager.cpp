#include "IOManager.h"
bool IOManager::readFiletoBuffer(string filePath,vector<unsigned char>& buffer)
{
	ifstream file(filePath, ios::binary);
	if (file.fail())
	{
		perror(filePath.c_str());
		return false;
	}

	//seek to the end
	file.seekg(0, ios::end);

	//Get the file size
	int fileSize = file.tellg();

	// seek to the begining
	file.seekg(0, ios::beg);

	//skip the header

	fileSize -= file.tellg();
	buffer.resize(fileSize);
	file.read((char *)&(buffer[0]), fileSize);
	file.close();
	return true;

}


