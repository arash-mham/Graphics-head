#pragma once
#include<GL/glew.h>
#include<cstddef>
class Vertex
{
public:
	Vertex();
	~Vertex();
	void setXPosition(float x);
	float getXPosition();
	void setYPosition(float y);
	float getYPosition();
	void setZPosition(float z);
	float getZPosition();
	void setRed(GLubyte red);
	GLubyte getRed();
	void setGreen(GLubyte green);
	GLubyte getGreen();
	void setBlue(GLubyte blue);
	GLubyte getBlue();
	void setAlpha(GLubyte alpha);
	GLubyte getAlpha();
	void setTexture(float u, float v, float w);
	float getUTexture();
	float getVTexture();
	float getWTexture();
	size_t offsetof_position();
	size_t offsetof_color();
	size_t offsetof_UVW();

private:
	float position[3];
	GLubyte color[4];
	float Textures[3];
	
};

