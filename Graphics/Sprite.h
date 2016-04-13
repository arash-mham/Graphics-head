#pragma once
#include<GL/glew.h>
#include <stdio.h>
#include"Vertex.h"
#include<cstddef>
#include<stdlib.h>
class Sprite
{
public:
	Sprite();
	~Sprite();
	void draw();
	void init(int x, int y, int width, int height);
	bool LoadVolumeFromFile(const char* fileName);
private:
	int _x;
	int _y;
	int _width;
	int _height;
	GLuint _vboid;

};

