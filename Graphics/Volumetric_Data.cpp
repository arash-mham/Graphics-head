

#include "Volumetric_Data.h"

Volumetric_Data::Volumetric_Data()
{
}
Volumetric_Data::~Volumetric_Data()
{
}
void Volumetric_Data::LoadVolumeFromFile(const char* fileName, int width, int height, int level) {
	level_stack=level;
	FILE *fp;
	size_t size = width *height * level;
	data = new GLubyte[size];
	Error er;
	if (!(fp = fopen(fileName, "rb")))
		er.fatalError("Error: opening.raw file failed");
	if (fread(data, sizeof(char), size, fp) != size)
		er.fatalError("Error: opening.raw file failed");
	fclose(fp);
	
}
void Volumetric_Data::genereate_texture(int width, int height, int level)
{
	textureID=new GLuint[level];
	glGenTextures(level, (GLuint*)textureID);
	for(int i=0;i<level;i++)
	{
	glBindTexture(GL_TEXTURE_2D, textureID[i]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, &data[width*height*i]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	}

	//Unbind the texture
	
	//delete data;
	
}
void Volumetric_Data::generate_texture_RGBA(int width,int height, int level)
{
	textureID=new GLuint[level];
	glGenTextures(level, (GLuint*)textureID);
	for(int i=0;i<level;i++)
	{
		int pass=i*width*height;
		GLubyte *dataRGBA;
		dataRGBA = new GLubyte[width*height*4];
		for( int nIndx = 0; nIndx < width*height; nIndx++ )
		{
			dataRGBA[nIndx*4] = data[pass+nIndx];
			dataRGBA[nIndx*4+1] = data[pass+nIndx];
			dataRGBA[nIndx*4+2] = data[pass+nIndx];
			dataRGBA[nIndx*4+3] = 255;
			if( data[pass+nIndx] < 20 )
			{
				dataRGBA[nIndx*4+3] = 0;
			}
		}
		glBindTexture( GL_TEXTURE_2D, textureID[i] );
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height , 0,GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid *)dataRGBA);
		glBindTexture(GL_TEXTURE_2D, 0);
		delete dataRGBA;
	}
}
void Volumetric_Data::genereate_texture3D(int width, int height, int level)
{
	GLubyte *dataRGBA;
	dataRGBA = new GLubyte[width*height*4];
	glGenTextures(1,(GLuint*)&ThreeDtextureID );
	glBindTexture( GL_TEXTURE_3D, ThreeDtextureID );
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	for( int nIndx = 0; nIndx < width*height*level; nIndx++ )
	{
		
		dataRGBA[nIndx*4] = data[nIndx];
		dataRGBA[nIndx*4+1] = data[nIndx];
		dataRGBA[nIndx*4+2] = data[nIndx];
		dataRGBA[nIndx*4+3] = 255;
	}
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA,width, height , level, 0,GL_RGBA, GL_UNSIGNED_BYTE,(GLvoid *) dataRGBA );
	glBindTexture( GL_TEXTURE_3D, 0 );
	
	//delete data;
	delete dataRGBA;
	delete data;
}
void Volumetric_Data::init()
{
	LoadVolumeFromFile("Files/head2", 256,256,109);
	generate_texture_RGBA(256,256,109);
	setUp1D();
	setUp2D();
	setUp3D();
}
void Volumetric_Data::draw()
{
	Vertex v;
	glBindBuffer(GL_ARRAY_BUFFER, _vboid);

	glEnableVertexAttribArray(0);

	//2 here because we only have x,y
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)v.offsetof_position());
	//color attribure
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)v.offsetof_color());

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)v.offsetof_UVW());

	//actually draw the data

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(0);

	//unbind buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
GLuint Volumetric_Data::get_textureID(int i)
{
	if(i<level_stack&&i>=0)
	return textureID[i];
	else
		return textureID[0];
}
GLuint Volumetric_Data::get_textureIDThreeD()
{
	return ThreeDtextureID ;
}
int Volumetric_Data::getLevel()
{
	return level_stack;
}
void Volumetric_Data::draw(int i)
{
	//none vertex object to acces the offset does not do anything
	Vertex v;
	glBindTexture(GL_TEXTURE_2D, textureID[i]);
	glBindBuffer(GL_ARRAY_BUFFER, _vboidLayer[i]);
	glEnableVertexAttribArray(0);
	//position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)v.offsetof_position());
	//color attribure
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)v.offsetof_color());
	//texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)v.offsetof_UVW());
	//actually draw the data
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(0);
	//unbind buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void Volumetric_Data::setUp1D()
{
	//double check if we have mave a vertex buffer before
	if (_vboid == 0)
	{
		glGenBuffers(1, &_vboid);
	}//end of if
	#pragma region vertex_values
			int x; int y; int width; int height;
			x = -1;
			y = -1;
			width = 2;
			height = 2;
			vertexData[0].setXPosition(x + height);
			vertexData[0].setYPosition(y + width);
			vertexData[0].setZPosition(0);
			vertexData[0].setTexture(0.0f, 0.0f,0.0f);

			vertexData[1].setXPosition(x);
			vertexData[1].setYPosition(y + width);
			vertexData[1].setZPosition(0);
			vertexData[1].setTexture(1.0f, 0.0f,0.0f);

			vertexData[2].setXPosition(x);
			vertexData[2].setYPosition(y);
			vertexData[2].setZPosition(0);
			vertexData[2].setTexture(1.0f, 1.0f,0.0f);

			vertexData[3].setXPosition(x);
			vertexData[3].setYPosition(y);
			vertexData[3].setZPosition(0);
			vertexData[3].setTexture(1.0f, 1.0f,0.0f);

			vertexData[4].setXPosition(x + width);
			vertexData[4].setYPosition(y);
			vertexData[4].setZPosition(0);
			vertexData[4].setTexture(0.0f, 1.0f,0.0f);

			vertexData[5].setXPosition(x + width);
			vertexData[5].setYPosition(y + height);
			vertexData[5].setZPosition(0);
			vertexData[5].setTexture(0.0f, 0.0f,0.0f);
	#pragma endregion vertex and texture values
	//makes the buffer active
	glBindBuffer(GL_ARRAY_BUFFER, _vboid);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
	//unbind buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void Volumetric_Data::setUp2D()
{
	
	_vboidLayer=new GLuint[level_stack];
	glGenBuffers(level_stack, (GLuint*)_vboidLayer);
	for(int i=0;i<level_stack;i++)
	{
		
		#pragma region vertex_values
			Vertex vertexDataLayer[6];
			int x; int y; int width; int height;
			x = -1;
			y = -1;
			width = 2;
			height = 2;
			vertexDataLayer[0].setXPosition(x + height);
			vertexDataLayer[0].setYPosition(y + width);
			vertexDataLayer[0].setZPosition((float)i/120.0f);
			vertexDataLayer[0].setTexture(0.0f, 0.0f,0.0f);

			vertexDataLayer[1].setXPosition(x);
			vertexDataLayer[1].setYPosition(y + width);
			vertexDataLayer[1].setZPosition((float)i/120.0f);
			vertexDataLayer[1].setTexture(1.0f, 0.0f,0.0f);

			vertexDataLayer[2].setXPosition(x);
			vertexDataLayer[2].setYPosition(y);
			vertexDataLayer[2].setZPosition((float)i/120.0f);
			vertexDataLayer[2].setTexture(1.0f, 1.0f,0.0f);

			vertexDataLayer[3].setXPosition(x);
			vertexDataLayer[3].setYPosition(y);
			vertexDataLayer[3].setZPosition((float)i/120.0f);
			vertexDataLayer[3].setTexture(1.0f, 1.0f,0.0f);

			vertexDataLayer[4].setXPosition(x + width);
			vertexDataLayer[4].setYPosition(y);
			vertexDataLayer[4].setZPosition((float)i/120.0f);
			vertexDataLayer[4].setTexture(0.0f, 1.0f,0.0f);

			vertexDataLayer[5].setXPosition(x + width);
			vertexDataLayer[5].setYPosition(y + height);
			vertexDataLayer[5].setZPosition((float)i/120.0f);
			vertexDataLayer[5].setTexture(0.0f, 0.0f,0.0f);
	#pragma endregion vertex and texture values
		glBindBuffer(GL_ARRAY_BUFFER, _vboidLayer[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexDataLayer, GL_STATIC_DRAW);
		//unbind buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}//end of for
	
}
void Volumetric_Data::setUp3D()
{
	Vertex vertexData3D[120];
	//double check if we have mave a vertex buffer before
		glGenBuffers(1, &_vboid3D);
	#pragma region vertex_values
		for(int i=0;i<20;i++)
		{
			int x; int y; int width; int height;
			x = -1;
			y = -1;
			width = 2;
			height = 2;
			vertexData3D[i].setXPosition(x + height);
			vertexData3D[i].setYPosition(y + width);
			vertexData3D[i].setZPosition(0);
			vertexData3D[i].setTexture(0.0f, 0.0f,0.0f);

			vertexData3D[i+1].setXPosition(x);
			vertexData3D[i+1].setYPosition(y + width);
			vertexData3D[i+1].setZPosition(0);
			vertexData3D[i+1].setTexture(1.0f, 0.0f,0.0f);

			vertexData3D[i+2].setXPosition(x);
			vertexData3D[i+2].setYPosition(y);
			vertexData3D[i+2].setZPosition(0);
			vertexData3D[i+2].setTexture(1.0f, 1.0f,0.0f);

			vertexData3D[i+3].setXPosition(x);
			vertexData3D[i+3].setYPosition(y);
			vertexData3D[i+3].setZPosition(0);
			vertexData3D[i+3].setTexture(1.0f, 1.0f,0.0f);

			vertexData3D[i+4].setXPosition(x + width);
			vertexData3D[i+4].setYPosition(y);
			vertexData3D[i+4].setZPosition(0);
			vertexData3D[i+4].setTexture(0.0f, 1.0f,0.0f);

			vertexData3D[i+5].setXPosition(x + width);
			vertexData3D[i+5].setYPosition(y + height);
			vertexData3D[i+5].setZPosition(0);
			vertexData3D[i+5].setTexture(0.0f, 0.0f,0.0f);
		}
	#pragma endregion vertex and texture values
	//makes the buffer active
	glBindBuffer(GL_ARRAY_BUFFER, _vboid3D);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData3D), vertexData3D, GL_STATIC_DRAW);
	//unbind buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void Volumetric_Data::draw3D(int i)
{
	Vertex v;
	glBindTexture(GL_TEXTURE_3D, ThreeDtextureID);
	glBindBuffer(GL_ARRAY_BUFFER, _vboid3D);

	glEnableVertexAttribArray(0);

	//2 here because we only have x,y
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)v.offsetof_position());
	//color attribure
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)v.offsetof_color());

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)v.offsetof_UVW());

	//actually draw the data

	glDrawArrays(GL_TRIANGLES, i, 6);
	

	glDisableVertexAttribArray(0);

	//unbind buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
GLuint Volumetric_Data::get_texture3D()
{
	return ThreeDtextureID;
}