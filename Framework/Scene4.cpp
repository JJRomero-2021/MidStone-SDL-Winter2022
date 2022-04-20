#include <glew.h>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "Scene4.h"
#include "MMath.h"
#include "VMath.h"
#include "Timer.h"

using namespace std;

//test

Scene4::Scene4(SDL_Window* sdlWindow_, GameManager* scene_)
{
	windowPtr = sdlWindow_;
	controllsScene = new Body(Vec3(-16.0f, 40.0f, 0.0f), Vec3(11.3f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 1.0f);
	back = new Body(Vec3(15.0f, 0.0f, 0.0f), Vec3(11.3f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 1.0f);
	scene = scene_;
}

Scene4::~Scene4()
{
	delete controllsScene;
	delete back;

}

bool Scene4::OnCreate()
{
	renderer = SDL_GetRenderer(windowPtr);

	if (!renderer) {

		renderer = SDL_CreateRenderer(windowPtr, -1, SDL_RENDERER_ACCELERATED);

	}

	if (!renderer) {

		std::cout << "SDL_Error: " << SDL_GetError() << std::endl;

		return false;

	}
	int w, h;
	SDL_GetWindowSize(windowPtr, &w, &h);

	Matrix4 ndc = MMath::viewportNDC(w, h);
	Matrix4 ortho = MMath::orthographic(-10.0f, 30.0f, -10.0f, 40.0f, 0.0f, 1.0f);
	projectionMatrix = ndc * ortho;
	IMG_Init(IMG_INIT_PNG);

	SDL_Surface* conImage = IMG_Load("textures/ControllsScene.png");
	SDL_Texture* conTexture = SDL_CreateTextureFromSurface(renderer, conImage);
	if (conImage == nullptr)
	{
		printf("Can't open textures/ControllsScene.png\n");
		return false;
	}

	SDL_Surface* backImage = IMG_Load("textures/BackButt.png");
	SDL_Texture* backTexture = SDL_CreateTextureFromSurface(renderer, backImage);
	if (backImage == nullptr)
	{
		printf("Can't open textures/BackButt.png\n");
		return false;
	}

	
	controllsScene->setTexture(conTexture);
	SDL_FreeSurface(conImage);
	back->setTexture(backTexture);
	SDL_FreeSurface(backImage);
	
	return true;
}

void Scene4::HandleEvents(const SDL_Event& sdlEvent)
{
	int x, y;    // mouse position in screen coords



	// convert mouse's screen coords to world coords

	Uint32 buttons;

	buttons = SDL_GetMouseState(&x, &y);



	Vec3 mouseCoords = Vec3(float(x), float(y), 0.0f);

	Matrix4 inverseProjection = MMath::inverse(projectionMatrix);

	Vec3 mousePos = inverseProjection * (mouseCoords);
	int w, h, ww, wh;

	if (sdlEvent.button.type == SDL_MOUSEBUTTONUP) {
		// if the mouse is on the play button when clicked change to scene4
		Vec3 playButPos = back->getPos();
		SDL_QueryTexture(back->getTexture(), nullptr, nullptr, &w, &h);
		Vec3 ButtonDimensions = inverseProjection * Vec3(float(w), float(h), 0.0f);
		SDL_GetWindowSize(windowPtr, &ww, &wh);
		float buttonWidth = float(w) * 40.0f / float(ww);
		float buttonHeight = float(h) * 50.0f / float(wh);

		printf("mouse pos %f %f\n", mousePos.x, mousePos.y);
		printf("backbutpos %f %f %f %f\n", playButPos.x, playButPos.y, buttonWidth, buttonHeight);

		if (mousePos.x >= 15.0f & mousePos.x <= 24.5f & mousePos.y >= -2.5f & mousePos.y <= 0.0f) {
			SDL_Event event;
			SDL_memset(&event, 0, sizeof(event));
			event.type = scene->getChangeScene();
			event.user.code = 4;
			event.user.data1 = nullptr;
			event.user.data2 = nullptr;
			SDL_PushEvent(&event);

		}

		
	}
}

void Scene4::Update(const float deltaTime)
{
}

void Scene4::Render()
{
	/// Clear the screen
	SDL_SetRenderDrawColor(renderer, 0, 0, 50, 0);
	SDL_RenderClear(renderer);

	SDL_Rect square;
	Vec3 screenCoords;
	int w, h;

	/// Draw your scene here
	screenCoords = projectionMatrix * controllsScene->getPos();
	SDL_QueryTexture(controllsScene->getTexture(), nullptr, nullptr, &w, &h);
	square.x = static_cast<int>(screenCoords.x);
	square.y = static_cast<int>(screenCoords.y);
	square.w = w * 3;
	square.h = h * 3;
	SDL_RenderCopyEx(renderer, controllsScene->getTexture(), nullptr, &square, 0.0, nullptr, SDL_FLIP_NONE);

	screenCoords = projectionMatrix * back->getPos();
	SDL_QueryTexture(back->getTexture(), nullptr, nullptr, &w, &h);
	square.x = static_cast<int>(screenCoords.x);
	square.y = static_cast<int>(screenCoords.y);
	square.w = w;
	square.h = h;
	SDL_RenderCopyEx(renderer, back->getTexture(), nullptr, &square, 0.0, nullptr, SDL_FLIP_NONE);

	
	SDL_RenderPresent(renderer);
}

void Scene4::OnDestroy()
{

}