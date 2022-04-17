#include <glew.h>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "Window.h"
#include "Scene0.h"
#include "MMath.h"
#include "VMath.h"
#include "Timer.h"

using namespace MATH;
using namespace std;



Scene0::Scene0(SDL_Window* sdlWindow_, GameManager* scene_)
{
	windowPtr = sdlWindow_;
	background = new Body(Vec3(-10.0f, 40.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 1.0f);
	spaceShip = new Body(Vec3(-8.0f, 7.8f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 1.0f);
	obstacle1 = new Body(Vec3(28.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 1.0f);
	obstacle2 = new Body(Vec3(28.0f, 10.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 1.0f);
	obstacle3 = new Body(Vec3(28.0f, 19.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 1.0f);
	obstacle4 = new Body(Vec3(28.0f, 28.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 1.0f);
	obstacle5 = new Body(Vec3(28.0f, 36.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 1.0f);
	bullets.push_back(new Body(Vec3(-20.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 1.0f));
	scene = scene_;
	spaceshipDirec = 0;
	totalTargetsDestroyed = 0;

}

Scene0::~Scene0()
{
	delete spaceShip;
	delete obstacle1, obstacle2, obstacle3, obstacle4, obstacle5;
	delete background;
	for (Body* body : bullets) {
		delete body;
	}
}

bool Scene0::OnCreate()
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

	SDL_Surface* backgroundImage = IMG_Load("textures/Space-Background-Tiled.png");
	SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundImage);
	if (backgroundImage == nullptr)
	{
		printf("Can't open textures/Space-Background-Tiled.png\n");
		return false;
	}

	SDL_Surface* obstacleImage = IMG_Load("textures/Obstacle.png");
	SDL_Texture* obstacleTexture = SDL_CreateTextureFromSurface(renderer, obstacleImage);
	if (obstacleImage == nullptr)
	{
		printf("Can't open textures/Obstacle.png\n");
		return false;
	}

	SDL_Surface* spaceShipImage = IMG_Load("textures/Spaceship.png");
	SDL_Texture* spaceShipTexture = SDL_CreateTextureFromSurface(renderer, spaceShipImage);
	if (spaceShipImage == nullptr)
	{
		printf("Can't open textures/Spaceship.png\n");
		return false;
		
	}

	SDL_Surface* bulletImage = IMG_Load("textures/bullet.png");
	SDL_Texture* bulletTexture = SDL_CreateTextureFromSurface(renderer, bulletImage);
	if (bulletImage == nullptr)
	{
		printf("Can't open textures/bullet.png\n");
		return false;
	}
		
	spaceShip->setTexture(spaceShipTexture);
	SDL_FreeSurface(spaceShipImage);

	background->setTexture(backgroundTexture);
	SDL_FreeSurface(backgroundImage);

	obstacle1->setTexture(obstacleTexture);
	SDL_FreeSurface(obstacleImage);
	obstacle2->setTexture(obstacleTexture);
	SDL_FreeSurface(obstacleImage);
	obstacle3->setTexture(obstacleTexture);
	SDL_FreeSurface(obstacleImage);
	obstacle4->setTexture(obstacleTexture);
	SDL_FreeSurface(obstacleImage);
	obstacle5->setTexture(obstacleTexture);
	SDL_FreeSurface(obstacleImage);

	for (int i = 0; i < bullets.size(); i++) {
		bullets[i]->setTexture(bulletTexture);
	}
	SDL_FreeSurface(bulletImage);



	
	return true;

}

void Scene0::HandleEvents(const SDL_Event& sdlEvent)
{
	if (sdlEvent.type == SDL_EventType::SDL_KEYDOWN)
	{
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_D)
		{
			Vec3 currpos = spaceShip->getPos();
			Vec3 newpos = currpos + Vec3(0.5, 0.0, 0.0);
			spaceShip->setPos(newpos);
			spaceShip->isMoving(true);
			spaceshipDirec = 0;
			
		}
	}
	else if (sdlEvent.type == SDL_EventType::SDL_KEYUP)
	{

		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_D)
		{
			
		}
	}
	if (sdlEvent.type == SDL_EventType::SDL_KEYDOWN)
	{
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_A)
		{
			Vec3 currpos = spaceShip->getPos();
			Vec3 newpos = currpos + Vec3(-0.5, 0.0, 0.0);
			spaceShip->setPos(newpos);
			spaceShip->isMoving(true);
			spaceshipDirec = 1;

		}
	}
	else if (sdlEvent.type == SDL_EventType::SDL_KEYUP)
	{
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_A)
		{
			
		}
	}

	if (sdlEvent.type == SDL_EventType::SDL_KEYDOWN)
	{
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_W)
		{
			Vec3 currpos = spaceShip->getPos();
			Vec3 newpos = currpos + Vec3(0.0, 0.5, 0.0);
			spaceShip->setPos(newpos);
			spaceShip->isMoving(true);

		}
	}
	else if (sdlEvent.type == SDL_EventType::SDL_KEYUP)
	{
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_W)
		{
			
		}
	}


	if (sdlEvent.type == SDL_EventType::SDL_KEYDOWN)
	{
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_S)
		{
			Vec3 currpos = spaceShip->getPos();
			Vec3 newpos = currpos + Vec3(0.0 ,-0.5, 0.0);
			spaceShip->setPos(newpos);
			spaceShip->isMoving(true);

		}
	}
	else if (sdlEvent.type == SDL_EventType::SDL_KEYUP)
	{
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_S)
		{
			
		}
	}  
	if (sdlEvent.type == SDL_EventType::SDL_KEYDOWN)
	{
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_SPACE)
		{
			Vec3 spaceshipPos = spaceShip->getPos();
			Vec3 bulletPos = spaceshipPos + Vec3(2.0f, -3.7f, 0.0f);
			Body* newBullet = new Body(bulletPos, Vec3(1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 1.0f);
			newBullet->setTexture(bullets[0]->getTexture());
			bullets.push_back(newBullet);
			
				newBullet->ApplyForce(Vec3(35.0f, 0.0f, 0.0f));
				newBullet->isMoving(true);
			

		}
	}
	
	else if (sdlEvent.type == SDL_EventType::SDL_KEYUP)
	{
	if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_SPACE)
	{

	}
	}

	else if (sdlEvent.type == SDL_USEREVENT)
	{
		printf("%d %s\n", sdlEvent.user.data1, sdlEvent.user.data2);
	}
}

void Scene0::Update(const float deltaTime)
{
	spaceShip->Update(deltaTime);


	obstacle1->ApplyForce(Vec3(-0.4, 0.0,0.0));
	obstacle1->isMoving(true);
	obstacle2->ApplyForce(Vec3(-0.5, 0.0, 0.0));
	obstacle2->isMoving(true);
	obstacle3->ApplyForce(Vec3(-0.8, 0.0, 0.0));
	obstacle3->isMoving(true);
	obstacle4->ApplyForce(Vec3(-0.6, 0.0, 0.0));
	obstacle4->isMoving(true);
	obstacle5->ApplyForce(Vec3(-0.3, 0.0, 0.0));
	obstacle5->isMoving(true);


	obstacle1->Update(deltaTime);
	obstacle2->Update(deltaTime);
	obstacle3->Update(deltaTime);
	obstacle4->Update(deltaTime);
	obstacle5->Update(deltaTime);

	for (int i = 0; i < bullets.size(); i++) {
		bullets[i]->Update(deltaTime);
	}

	for (int i = 0; i < bullets.size(); i++) {
		Vec3 bullPos = bullets[i]->getPos();
		Vec3 obstacPos1 = obstacle1->getPos();
		float dist1 = VMath::distance(bullPos, obstacPos1);
		if (dist1 < 1.0) {
			SDL_Surface* blastImage = IMG_Load("textures/blast.png");
			SDL_Texture* blastTexture = SDL_CreateTextureFromSurface(renderer, blastImage);
			obstacle1->setTexture(blastTexture);
			SDL_FreeSurface(blastImage);
			Vec3 obstacCurrPos = obstacle1->getPos();
			obstacle1->setPos(obstacCurrPos + Vec3(-0.5, 0.5, 0.0));
			obstacle1->targetDestroyed(true);
		}

		Vec3 obstacPos2 = obstacle2->getPos();
		float dist2 = VMath::distance(bullPos, obstacPos2);
		if (dist2 < 1.0) {
			SDL_Surface* blastImage = IMG_Load("textures/blast.png");
			SDL_Texture* blastTexture = SDL_CreateTextureFromSurface(renderer, blastImage);
			obstacle2->setTexture(blastTexture);
			SDL_FreeSurface(blastImage);
			Vec3 obstacCurrPos = obstacle2->getPos();
			obstacle2->setPos(obstacCurrPos + Vec3(-0.5, 0.5, 0.0));
			obstacle2->targetDestroyed(true);
		}

		Vec3 obstacPos3 = obstacle3->getPos();
		float dist3 = VMath::distance(bullPos, obstacPos3);
		if (dist3 < 1.0) {
			SDL_Surface* blastImage = IMG_Load("textures/blast.png");
			SDL_Texture* blastTexture = SDL_CreateTextureFromSurface(renderer, blastImage);
			obstacle3->setTexture(blastTexture);
			SDL_FreeSurface(blastImage);
			Vec3 obstacCurrPos = obstacle3->getPos();
			obstacle3->setPos(obstacCurrPos + Vec3(-0.5, 0.5, 0.0));
			obstacle3->targetDestroyed(true);
		}

		Vec3 obstacPos4 = obstacle4->getPos();
		float dist4 = VMath::distance(bullPos, obstacPos4);
		if (dist4 < 1.0) {
			SDL_Surface* blastImage = IMG_Load("textures/blast.png");
			SDL_Texture* blastTexture = SDL_CreateTextureFromSurface(renderer, blastImage);
			obstacle4->setTexture(blastTexture);
			SDL_FreeSurface(blastImage);
			Vec3 obstacCurrPos = obstacle4->getPos();
			obstacle4->setPos(obstacCurrPos + Vec3(-0.5, 0.5, 0.0));
			obstacle4->targetDestroyed(true);
		}

		Vec3 obstacPos5 = obstacle5->getPos();
		float dist5 = VMath::distance(bullPos, obstacPos5);
		if (dist5 < 1.0) {
			SDL_Surface* blastImage = IMG_Load("textures/blast.png");
			SDL_Texture* blastTexture = SDL_CreateTextureFromSurface(renderer, blastImage);
			obstacle5->setTexture(blastTexture);
			SDL_FreeSurface(blastImage);
			Vec3 obstacCurrPos = obstacle5->getPos();
			obstacle5->setPos(obstacCurrPos + Vec3(-0.5, 0.5, 0.0));
			obstacle5->targetDestroyed(true);
		}
		


		
	}

	if (obstacle1->getTargetDestroyed() == true) {
		totalTargetsDestroyed++;
		obstacle1->targetDestroyed(false);
	}
	if (obstacle2->getTargetDestroyed() == true) {
		totalTargetsDestroyed++;
		obstacle2->targetDestroyed(false);
	}
	if (obstacle3-> getTargetDestroyed() == true) {
		totalTargetsDestroyed++;
		obstacle3->targetDestroyed(false);
	}
	if (obstacle4->getTargetDestroyed() == true) {
		totalTargetsDestroyed++;
		obstacle4->targetDestroyed(false);
	}
	if (obstacle5->getTargetDestroyed() == true) {
		totalTargetsDestroyed++;
		obstacle5->targetDestroyed(false);
	}
	
	printf("%d", totalTargetsDestroyed);

	if (totalTargetsDestroyed == 5) {
		printf("all destroyed" );
		
	}

	Vec3 origin(0.0f, 0.0f, 0.0f);
	//printf("distance from origin %.2f \n", VMath::distance(spaceShip->getPos(), origin));
	if (VMath::distance(spaceShip->getPos(), origin) < 1.5f)
	{
		SDL_Event event;
		SDL_memset(&event, 0, sizeof(event));
		event.type = scene->getChangeScene();
		event.user.code = 1;
		event.user.data1 = nullptr;
		event.user.data2 = nullptr;
		SDL_PushEvent(&event);
	}
}

void Scene0::Render()
{
	/// Clear the screen
	SDL_SetRenderDrawColor(renderer, 0, 0, 50, 0);
	SDL_RenderClear(renderer);

	SDL_Rect square;
	Vec3 screenCoords;
	int w, h;

	/// Draw your scene here

	screenCoords = projectionMatrix * background->getPos();
	SDL_QueryTexture(background->getTexture(), nullptr, nullptr, &w, &h);
	square.x = static_cast<int>(screenCoords.x);
	square.y = static_cast<int>(screenCoords.y);
	square.w = w * 4;
	square.h = h * 4;
	SDL_RenderCopyEx(renderer, background->getTexture(), nullptr, &square, 0.0, nullptr, SDL_FLIP_NONE);

	for (int i = 0; i < bullets.size(); i++) {
		screenCoords = projectionMatrix * bullets[i]->getPos();
		SDL_QueryTexture(bullets[i]->getTexture(), nullptr, nullptr, &w, &h);
		square.x = static_cast<int>(screenCoords.x) - (w / 2);
		square.y = static_cast<int>(screenCoords.y) - (h / 2);
		square.w = w;
		square.h = h;
		SDL_RenderCopyEx(renderer, bullets[i]->getTexture(), nullptr, &square, 0.0, nullptr, SDL_FLIP_NONE);
	}

	screenCoords = projectionMatrix * obstacle1->getPos();
	SDL_QueryTexture(obstacle1->getTexture(), nullptr, nullptr, &w, &h);
	square.x = static_cast<int>(screenCoords.x);
	square.y = static_cast<int>(screenCoords.y);
	square.w = w / 4;
	square.h = h / 4;
	SDL_RenderCopyEx(renderer, obstacle1->getTexture(), nullptr, &square, 0.0, nullptr, SDL_FLIP_NONE);

	screenCoords = projectionMatrix * obstacle2->getPos();
	SDL_QueryTexture(obstacle2->getTexture(), nullptr, nullptr, &w, &h);
	square.x = static_cast<int>(screenCoords.x);
	square.y = static_cast<int>(screenCoords.y);
	square.w = w / 4;
	square.h = h / 4;
	SDL_RenderCopyEx(renderer, obstacle2->getTexture(), nullptr, &square, 0.0, nullptr, SDL_FLIP_NONE);

	screenCoords = projectionMatrix * obstacle3->getPos();
	SDL_QueryTexture(obstacle3->getTexture(), nullptr, nullptr, &w, &h);
	square.x = static_cast<int>(screenCoords.x);
	square.y = static_cast<int>(screenCoords.y);
	square.w = w / 4;
	square.h = h / 4;
	SDL_RenderCopyEx(renderer, obstacle3->getTexture(), nullptr, &square, 0.0, nullptr, SDL_FLIP_NONE);

	screenCoords = projectionMatrix * obstacle4->getPos();
	SDL_QueryTexture(obstacle4->getTexture(), nullptr, nullptr, &w, &h);
	square.x = static_cast<int>(screenCoords.x);
	square.y = static_cast<int>(screenCoords.y);
	square.w = w / 4;
	square.h = h / 4;
	SDL_RenderCopyEx(renderer, obstacle4->getTexture(), nullptr, &square, 0.0, nullptr, SDL_FLIP_NONE);

	screenCoords = projectionMatrix * obstacle5->getPos();
	SDL_QueryTexture(obstacle5->getTexture(), nullptr, nullptr, &w, &h);
	square.x = static_cast<int>(screenCoords.x);
	square.y = static_cast<int>(screenCoords.y);
	square.w = w / 4;
	square.h = h / 4;
	SDL_RenderCopyEx(renderer, obstacle5->getTexture(), nullptr, &square, 0.0, nullptr, SDL_FLIP_NONE);


	screenCoords = projectionMatrix * spaceShip->getPos();
	SDL_QueryTexture(spaceShip->getTexture(), nullptr, nullptr, &w, &h);
	square.x = static_cast<int>(screenCoords.x);
	square.y = static_cast<int>(screenCoords.y);
	square.w = w;
	square.h = h;
	if (spaceshipDirec == 0)
	{
		SDL_RenderCopyEx(renderer, spaceShip->getTexture(), nullptr, &square, 0.0, nullptr, SDL_FLIP_NONE);
	} else
	{
		SDL_RenderCopyEx(renderer, spaceShip->getTexture(), nullptr, &square, 180.0, nullptr, SDL_FLIP_NONE);
	}
	SDL_RenderPresent(renderer);
}


void Scene0::OnDestroy()
{

}