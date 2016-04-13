#ifdef _WIN32 
	#define _CRT_SECURE_NO_DEPRECATE 
#endif 
#include "Sprite.h"


Sprite::Sprite()
{
	_vboid = 0;
}


Sprite::~Sprite()
{
	//we should free the buffer
	if (_vboid != 0)
		glDeleteBuffers(1, &_vboid);
}

void Sprite::init(int x, int y, int width, int height)
{
	_x = x;
	_y = y;
	_width = width;
	_height = height;
	if (_vboid == 0)
	{
		glGenBuffers(1, &_vboid);
	}
	Vertex vertexData[6];


	vertexData[0].setXPosition(x + height);
	vertexData[0].setYPosition(y + width);
	vertexData[0].setZPosition(0);


	vertexData[1].setXPosition(x);
	vertexData[1].setYPosition(y + width);
	vertexData[1].setZPosition(0);
	

	vertexData[2].setXPosition(x);
	vertexData[2].setYPosition(y);
	vertexData[2].setZPosition(0);
	
	
	vertexData[3].setXPosition(x);
	vertexData[3].setYPosition(y);
	vertexData[3].setZPosition(0);


	vertexData[4].setXPosition(x + width);
	vertexData[4].setYPosition(y);
	vertexData[4].setZPosition(0);
	
	vertexData[5].setXPosition(x + width);
	vertexData[5].setYPosition(y + height);
	vertexData[5].setZPosition(0);


	for (int i = 0; i < 6; i++)
	{
			vertexData[i].setRed(255);
			vertexData[i].setGreen(0);
			vertexData[i].setBlue(0);
			vertexData[i].setAlpha(255);
	
	}
	vertexData[1].setRed(255);
	vertexData[1].setGreen(244);
	vertexData[1].setBlue(0);
	vertexData[1].setAlpha(255);
	vertexData[4].setRed(122);
	vertexData[4].setGreen(244);
	vertexData[4].setBlue(123);
	vertexData[4].setAlpha(255);

	
		//makes the buffer active
	glBindBuffer(GL_ARRAY_BUFFER, _vboid);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);


	//unbind buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void Sprite::draw()
{
	Vertex v;
	glBindBuffer(GL_ARRAY_BUFFER, _vboid);

	glEnableVertexAttribArray(0);

	
	//3 here because we only have x,y,z
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)v.offsetof_position());
	//color attribure
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)v.offsetof_color());

	//actually draw the data

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(0);

	//unbind buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
}
