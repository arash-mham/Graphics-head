#pragma once
#include<SDL/SDL.h>
#include<GL/glew.h>
#include"GLSLProgram.h"
#include"Volumetric_Data.h"
enum  RenderState{RENDER, EXIT};
enum VolumeRenderState{SLICE, VOLUME2D, VOLUME3D};
class MainRenderer
{
public:
	MainRenderer();
	~MainRenderer();
	void run();
private:
	SDL_Window* _window;
	int _screenWdith;
	int _screenHeight;
	void  initSystems();
	void processInput();
	void renderLoop();
	RenderState _renderState;
	VolumeRenderState _volRenderState;
	void draw();
	void initShaders();
	Volumetric_Data volume;
	GLSLProgram GLSLpro;
	float _time;
	int current_texture;
	void volumeDrawSelection();
	bool animation;
};

