#include<iostream>
#include<string>
#include "MainRenderer.h"
#include "Error.h"



MainRenderer::MainRenderer()
{
	_window = nullptr;
	_screenHeight = 768;
	_screenWdith = 1024;
	_renderState = RenderState::RENDER;
	_time = 0;
	current_texture=0;
	animation=false;
}


MainRenderer::~MainRenderer()
{
	
}
void MainRenderer::run()
{
	initSystems();
	volume.init();
	renderLoop();
}
void MainRenderer::initSystems()
{
	Error er;
	SDL_Init(SDL_INIT_EVERYTHING);
	_window = SDL_CreateWindow("Graphics Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _screenWdith, _screenHeight, SDL_WINDOW_OPENGL);

	if (_window == nullptr)
	{
		er.fatalError("SDL Window could not be created"); 
		SDL_Quit();
	}
	SDL_GLContext glContex = SDL_GL_CreateContext(_window);
	if (glContex == nullptr)
	{ 
		er.fatalError("SDL_GL context could not be created!");
		SDL_Quit();
	}
	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		er.fatalError("Could not be initialized glew!");
		SDL_Quit();
	}
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);//we are goning to have two buffer, no flickering
	glClearColor(0.0f, 0.0f, 0.0f, 1.0);
	initShaders();
}
void MainRenderer::renderLoop()
{
	while (_renderState != RenderState::EXIT)
	{
		processInput();
				draw();
				if(animation)
					_time+=0.05;
				if(_time>INT_MAX)//to avoid making large numbers
					_time=0;
	}

}
void MainRenderer::processInput()
{
	//Input key, mouse,... evenets using SDL
	SDL_Event eve;
	while (SDL_PollEvent(&eve))
	{
		switch (eve.type)
		{
		case SDL_QUIT:
			_renderState = RenderState::EXIT;
			break;
		case SDL_KEYDOWN:
				switch (eve.key.keysym.sym)
				{
						case SDLK_DOWN:
							current_texture--;
							break;
						case SDLK_UP:
							current_texture++;
							break;
						case SDLK_1:
							_volRenderState=VolumeRenderState::SLICE;
							break;
						case SDLK_2:
							_volRenderState=VolumeRenderState::VOLUME2D;
							break;
						case SDLK_F1:
							animation=!animation;
						if(!animation){
						case SDLK_LEFT:
								if(_time>INT_MAX)
									_time=0;
								_time+=0.05;
								break;
						case SDLK_RIGHT:
								if(_time<INT_MIN)
									_time=0;
								_time-=0.05;
								break;
							}
				}
		break;
		}
	}
}
void MainRenderer::draw()
{
	 
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GLSLpro.use();
	glActiveTexture(GL_TEXTURE0);
	glClientActiveTexture(GL_TEXTURE0);
	 volumeDrawSelection();
	GLSLpro.unuse();
	SDL_GL_SwapWindow(_window);

}
void MainRenderer::initShaders()
{
	GLSLpro.compileShaders("Shaders/Vertexshader.txt", "Shaders/Fragmentshader.txt");
	GLSLpro.addAttribute("vertexPosition");//vertex position is coming from the vertex shader that we just wrote
	GLSLpro.addAttribute("vertexColor");
	GLSLpro.addAttribute("vertexuvw");
	GLSLpro.linkShaders();
}
void MainRenderer::volumeDrawSelection()
{
	if(_volRenderState==VolumeRenderState::SLICE)//If user wants to see each slice separately. We have just one
	{
		/*If user wants to see each slice separately.
		We have just one quad rendered with differnt textures*/
		glClear( GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT );
		glBindTexture(GL_TEXTURE_2D, volume.get_textureID(current_texture));

		volume.draw();
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	if(_volRenderState==VolumeRenderState::VOLUME2D)
	{
		/*Id the user wants to have a sense of the 3D object.
		We have created quad with the same number of slices 
		and blended the textures together*/
		GLint timeLocation = GLSLpro.getUniformLocation("time");//access the time variable in the vertex shader
		glUniform1f(timeLocation,_time);//animate the model
		glClear( GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT );
		glAlphaFunc( GL_GREATER, 0.5 );
		glEnable(GL_BLEND);//Blend nd all the textures
		glBlendFunc( GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR );
		 glEnable(GL_TEXTURE_2D);
		for(int i=0;i<volume.getLevel();i++)
		{
			volume.draw(i);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		glDisable(GL_BLEND);
	}
	
}


