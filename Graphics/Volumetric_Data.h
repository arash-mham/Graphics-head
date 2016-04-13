#pragma once

#ifdef _WIN32 
#define _CRT_SECURE_NO_DEPRECATE 
#endif 
#include <stdio.h>
#include<GL/glew.h>
#include <iostream>
#include "Vertex.h"
#include"IOManager.h"
#include"Error.h"

using namespace std;
class Volumetric_Data
{
public:
	Volumetric_Data();
	~Volumetric_Data();
	void LoadVolumeFromFile(const char* fileName, int width, int height, int level);
	void genereate_texture(int width, int height, int level);
	void generate_texture_RGBA(int wifth,int height,int level);
	void genereate_texture3D(int width, int height, int level);
	void init();
	void draw();
	GLuint get_textureID(int i);
	GLuint get_textureIDThreeD();
	void draw(int i);
	void setUp1D();
	void setUp2D();
	void setUp3D();
	int getLevel();
	void draw3D(int i);
	GLuint get_texture3D();
private:
	GLuint* textureID;
	GLuint _vboid;
	GLuint _vboid3D;
	GLuint* _vboidLayer;
	GLubyte *data;
	int level_stack;
	GLuint ThreeDtextureID;
	Vertex vertexData[6];
	
	
};

