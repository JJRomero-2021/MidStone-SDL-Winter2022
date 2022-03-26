#ifndef WINDOW_H
#define WINDOW_H

#include <SDL.h>
#include <glew.h>
#include <SDL_opengl.h>
#include <string>
#include <iostream>
class Window {
private:
	struct SDL_Window* window;   
	struct SDL_Surface* screenSurface;
	int width, height;
	
public:
	Window();
	~Window();
	bool OnCreate(std::string name_, int width_, int height_);
	void OnDestroy();
	SDL_Window* GetSDL_Window();
};
#endif
