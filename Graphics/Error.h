#pragma once
#include <string>
#include<iostream>
#include<SDL/SDL.h>

class Error
{
public:
	Error();
	~Error();
	void Error::fatalError(std::string errorString);
	
};

