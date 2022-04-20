#include <glew.h>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "Scene3.h"
#include "MMath.h"
#include "VMath.h"
#include "Timer.h"


using namespace std;

//test

Scene3::Scene3(SDL_Window* sdlWindow_, GameManager* scene_)
{
	windowPtr = sdlWindow_;
	scene = scene_;
	title = new Body(Vec3(-4.0f, 30.0f, 0.0f), Vec3(11.3f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 1.0f);
	play = new Body(Vec3(10.0f, 20.0f, 0.0f), Vec3(11.3f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 1.0f);
	controlls = new Body(Vec3(10.0f, 15.0f, 0.0f), Vec3(11.3f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 1.0f);
	credits = new Body(Vec3(10.0f, 10.0f, 0.0f), Vec3(11.3f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 1.0f);
	quit = new Body(Vec3(10.0f, 5.0f, 0.0f), Vec3(11.3f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 1.0f);
	
}

Scene3::~Scene3()
{
	delete title;
	delete play;
	delete controlls;
	delete credits;
	delete quit;
}

bool Scene3::OnCreate()
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

	SDL_Surface* titleImage = IMG_Load("textures/title.png");
	SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleImage);
	if (titleImage == nullptr)
	{
		printf("Can't open textures/title.png\n");
		return false;
	}

	SDL_Surface* playImage = IMG_Load("textures/playButton1.png");
	SDL_Texture* playTexture = SDL_CreateTextureFromSurface(renderer, playImage);
	if (playImage == nullptr)
	{
		printf("Can't open textures/playButton1.png\n");
		return false;
	}

	SDL_Surface* controllsImage = IMG_Load("textures/ConButton.png");
	SDL_Texture* controllsTexture = SDL_CreateTextureFromSurface(renderer, controllsImage);
	if (controllsImage == nullptr)
	{
		printf("Can't open textures/ConButton.png\n");
		return false;
	}

	SDL_Surface* creditsImage = IMG_Load("textures/CredButton.png");
	SDL_Texture* creditsTexture = SDL_CreateTextureFromSurface(renderer, creditsImage);
	if (creditsImage == nullptr)
	{
		printf("Can't open textures/CredButton.png\n");
		return false;
	}

	SDL_Surface* quitImage = IMG_Load("textures/QuitButton1.png");
	SDL_Texture* quitTexture = SDL_CreateTextureFromSurface(renderer, quitImage);
	if (quitImage == nullptr)
	{
		printf("Can't open textures/QuitButton1.png\n");
		return false;
	}
	title->setTexture(titleTexture);
	SDL_FreeSurface(titleImage);
	play->setTexture(playTexture);
	SDL_FreeSurface(playImage);
	controlls->setTexture(controllsTexture);
	SDL_FreeSurface(controllsImage);
	credits->setTexture(creditsTexture);
	SDL_FreeSurface(creditsImage);
	quit->setTexture(quitTexture);
	SDL_FreeSurface(quitImage);
	return true;
}

void Scene3::HandleEvents(const SDL_Event& sdlEvent)
{
	int x, y;    // mouse position in screen coords



	// convert mouse's screen coords to world coords

	Uint32 buttons;

	buttons = SDL_GetMouseState(&x, &y);



	Vec3 mouseCoords = Vec3(float(x), float(y), 0.0f);
	
	Matrix4 inverseProjection = MMath::inverse(projectionMatrix);
	
	Vec3 mousePos = inverseProjection * (mouseCoords);
	int w,h,ww,wh;

	if (sdlEvent.button.type == SDL_MOUSEBUTTONUP) {
		// if the mouse is on the play button when clicked change to scene0
		Vec3 playButPos = play->getPos();
		SDL_QueryTexture(play->getTexture(), nullptr, nullptr, &w, &h);
		Vec3 ButtonDimensions = inverseProjection * Vec3(float(w), float(h), 0.0f);
		SDL_GetWindowSize(windowPtr, &ww, &wh);
		float buttonWidth = float(w) * 40.0f / float(ww);
		float buttonHeight = float(h) * 50.0f / float(wh);
		
		printf("mouse pos %f %f\n", mousePos.x, mousePos.y);
		printf("playbutpos %f %f %f %f\n", playButPos.x, playButPos.y, buttonWidth, buttonHeight);
		
		if (mousePos.x >= 10.0f && mousePos.x <= 20.0f && mousePos.y >= 17.0f && mousePos.y <= 20.0f) {
			SDL_Event event;
			SDL_memset(&event, 0, sizeof(event));
			event.type = scene->getChangeScene();
			event.user.code = 1;
			event.user.data1 = nullptr;
			event.user.data2 = nullptr;
			SDL_PushEvent(&event);
			
		}
		
		// if the mouse is on the contorlls when clicked switch to controlls menu
		if (mousePos.x >= 10.0f && mousePos.x <= 20.0f && mousePos.y >= 12.5f && mousePos.y <= 15.0f) {
			SDL_Event event;
			SDL_memset(&event, 0, sizeof(event));
			event.type = scene->getChangeScene();
			event.user.code = 6;
			event.user.data1 = nullptr;
			event.user.data2 = nullptr;
			SDL_PushEvent(&event);

		}

		//if the mouse is on the credits button when clicked switch to credits menu
		if (mousePos.x >= 10.0f && mousePos.x <= 20.0f && mousePos.y >= 7.5f && mousePos.y <= 10.0f) {
			SDL_Event event;
			SDL_memset(&event, 0, sizeof(event));
			event.type = scene->getChangeScene();
			event.user.code = 7;
			event.user.data1 = nullptr;
			event.user.data2 = nullptr;
			SDL_PushEvent(&event);

		}

		//if the mouse is on the quit button when clicked close game
		if (mousePos.x >= 10.0f && mousePos.x <= 20.0f && mousePos.y >= 2.4f && mousePos.y <= 5.0f) {
			SDL_Event event;
			SDL_memset(&event, 0, sizeof(event));
			event.type = scene->getChangeScene();
			event.user.code = 5;
			event.user.data1 = nullptr;
			event.user.data2 = nullptr;
			SDL_PushEvent(&event);
		}
	}}

void Scene3::Update(const float deltaTime)
{
}

void Scene3::Render()
{
	/// Clear the screen
	SDL_SetRenderDrawColor(renderer, 0, 0, 50, 0);
	SDL_RenderClear(renderer);

	SDL_Rect square;
	Vec3 screenCoords;
	int w, h;

	/// Draw your scene here
	screenCoords = projectionMatrix * title->getPos();
	SDL_QueryTexture(title->getTexture(), nullptr, nullptr, &w, &h);
	square.x = static_cast<int>(screenCoords.x);
	square.y = static_cast<int>(screenCoords.y);
	square.w = w * 3;
	square.h = h * 3;
	SDL_RenderCopyEx(renderer, title->getTexture(), nullptr, &square, 0.0, nullptr, SDL_FLIP_NONE);

	screenCoords = projectionMatrix * play->getPos();
	SDL_QueryTexture(play->getTexture(), nullptr, nullptr, &w, &h);
	square.x = static_cast<int>(screenCoords.x);
	square.y = static_cast<int>(screenCoords.y);
	square.w = w;
	square.h = h;
	SDL_RenderCopyEx(renderer, play->getTexture(), nullptr, &square, 0.0, nullptr, SDL_FLIP_NONE);

	screenCoords = projectionMatrix * controlls->getPos();
	SDL_QueryTexture(controlls->getTexture(), nullptr, nullptr, &w, &h);
	square.x = static_cast<int>(screenCoords.x);
	square.y = static_cast<int>(screenCoords.y);
	square.w = w;
	square.h = h;
	SDL_RenderCopyEx(renderer, controlls->getTexture(), nullptr, &square, 0.0, nullptr, SDL_FLIP_NONE);

	screenCoords = projectionMatrix * credits->getPos();
	SDL_QueryTexture(credits->getTexture(), nullptr, nullptr, &w, &h);
	square.x = static_cast<int>(screenCoords.x);
	square.y = static_cast<int>(screenCoords.y);
	square.w = w;
	square.h = h;
	SDL_RenderCopyEx(renderer, credits->getTexture(), nullptr, &square, 0.0, nullptr, SDL_FLIP_NONE);

	screenCoords = projectionMatrix * quit->getPos();
	SDL_QueryTexture(quit->getTexture(), nullptr, nullptr, &w, &h);
	square.x = static_cast<int>(screenCoords.x);
	square.y = static_cast<int>(screenCoords.y);
	square.w = w;
	square.h = h;
	SDL_RenderCopyEx(renderer, quit->getTexture(), nullptr, &square, 0.0, nullptr, SDL_FLIP_NONE);

	SDL_RenderPresent(renderer);
}

void Scene3::OnDestroy()
{

}