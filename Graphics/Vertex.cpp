#include "Vertex.h"


Vertex::Vertex()
{
}


Vertex::~Vertex()
{
}
void Vertex::setXPosition(float x){ position[0] = x; }
float Vertex::getXPosition(){ return position[0]; }
void Vertex::setYPosition(float y){ position[1] = y; }
float Vertex::getYPosition(){ return position[1]; }
void Vertex::setZPosition(float z){ position[2] = z; }
float Vertex::getZPosition(){ return position[2]; }
void Vertex::setRed(GLubyte red){ color[0] = red; }
GLubyte Vertex::getRed(){ return color[0]; }
void Vertex::setGreen(GLubyte green){ color[1] = green; }
GLubyte Vertex::getGreen(){ return color[1]; }
void Vertex::setBlue(GLubyte blue){ color[2] = blue; }
GLubyte Vertex::getBlue(){ return color[2]; }
void Vertex::setAlpha(GLubyte alpha){ color[3] = alpha; }
GLubyte Vertex::getAlpha(){ return color[3]; }
size_t Vertex::offsetof_position(){ return offsetof(Vertex, position); }
size_t Vertex::offsetof_color(){ return offsetof(Vertex, color); }
size_t Vertex::offsetof_UVW(){ return offsetof(Vertex, Textures); }
void Vertex::setTexture(float u, float v, float w){ Textures[0] = u; Textures[1] = v; Textures[2]=w;}
float Vertex::getUTexture(){ return Textures[0]; }
float Vertex::getVTexture(){ return Textures[1]; }
float Vertex::getWTexture(){ return Textures[2]; }

