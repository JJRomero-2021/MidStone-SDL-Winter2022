#include <glew.h>
#include <SDL.h>
#include "Scene2.h"
#include "MMath.h"
#include "VMath.h"
#include "Timer.h"


Scene2::Scene2(SDL_Window* sdlWindow_, GameManager* scene_)
{
	windowPtr = sdlWindow_;
	background = new Body(Vec3(-100.0f, 200.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 1.0f);
	sun = new Body(Vec3(20.0f, 40.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 1.0f);
	planet = new Body(Vec3(10.0f, 30.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 1.0f);
	boss = new Body(Vec3(10.0f, 20.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 1.0f);
	minion = new Body(Vec3(16.0f, 22.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 1.0f);
	minion1 = new Body(Vec3(6.0f, 22.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 1.0f);
	spaceShip = new Body(Vec3(0.0f, 7.8f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 1.0f);
	obstacle = new Body(Vec3(32.0f, 30.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 1.0f);
	scene = scene_;
}

Scene2::~Scene2()
{
	delete spaceShip;
	delete boss;
	delete background;
	delete sun;
	delete planet;
	delete obstacle;
}

bool Scene2::OnCreate() {

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
	Matrix4 ortho = MMath::orthographic(0.0f, 40.0f, 0.0f, 50.0f, 0.0f, 1.0f);
	projectionMatrix = ndc * ortho;
	IMG_Init(IMG_INIT_PNG);

	SDL_Surface* backgroundImage = IMG_Load("textures/Space-Background-Tiled.png");
	SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundImage);
	if (backgroundImage == nullptr)
	{
		printf("Can't open textures/Space-Background-Tiled.png\n");
		return false;
	}

	SDL_Surface* sunImage = IMG_Load("textures/Sun.png");
	SDL_Texture* sunTexture = SDL_CreateTextureFromSurface(renderer, sunImage);
	if (sunImage == nullptr)
	{
		printf("Can't open textures/Sun.png\n");
		return false;
	}

	SDL_Surface* planetImage = IMG_Load("textures/earth.png");
	SDL_Texture* planetTexture = SDL_CreateTextureFromSurface(renderer, planetImage);
	if (planetImage == nullptr)
	{
		printf("Can't open textures/earth.png\n");
		return false;
	}

	SDL_Surface* bossImage = IMG_Load("textures/enemyBlack4.png");
	SDL_Texture* bossTexture = SDL_CreateTextureFromSurface(renderer, bossImage);
	if (bossImage == nullptr)
	{
		printf("Can't open textures/enemyBlack4.png\n");
		return false;
	}

	SDL_Surface* minionImage = IMG_Load("textures/enemyBlack1.png");
	SDL_Texture* minionTexture = SDL_CreateTextureFromSurface(renderer, minionImage);
	if (minionImage == nullptr)
	{
		printf("Can't open textures/enemyBlack1.png\n");
		return false;
	}

	SDL_Surface* obstacleImage = IMG_Load("textures/playerShip2_damage3.png");
	SDL_Texture* obstacleTexture = SDL_CreateTextureFromSurface(renderer, obstacleImage);
	if (obstacleImage == nullptr)
	{
		printf("Can't open textures/playerShip2_damage3.png\n");
		return false;
	}

	SDL_Surface* spaceShipImage = IMG_Load("textures/Spaceship.png");
	SDL_Texture* spaceShipTexture = SDL_CreateTextureFromSurface(renderer, spaceShipImage);
	if (spaceShipImage == nullptr)
	{
		printf("Can't open textures/Spaceship.png\n");
		return false;
	}
	background->setTexture(backgroundTexture);
	SDL_FreeSurface(backgroundImage);
	sun->setTexture(sunTexture);
	SDL_FreeSurface(sunImage);
	planet->setTexture(planetTexture);
	SDL_FreeSurface(planetImage);
	boss->setTexture(bossTexture);
	SDL_FreeSurface(bossImage);
	minion->setTexture(minionTexture);
	SDL_FreeSurface(minionImage);
	minion1->setTexture(minionTexture);
	SDL_FreeSurface(minionImage);
	spaceShip->setTexture(spaceShipTexture);
	SDL_FreeSurface(spaceShipImage);
	obstacle->setTexture(obstacleTexture);
	SDL_FreeSurface(obstacleImage);
	return true;
}

void Scene2::HandleEvents(const SDL_Event& sdlEvent)
{
	if (sdlEvent.type == SDL_EventType::SDL_KEYDOWN)
	{
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_D)
		{
			Vec3 currpos = spaceShip->getPos();
			Vec3 newpos = currpos + Vec3(0.5, 0.0, 0.0);
			spaceShip->setPos(newpos);
			spaceShip->isMoving(true);
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
			Vec3 newpos = currpos + Vec3(0.0, -0.5, 0.0);
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

	else if (sdlEvent.type == SDL_USEREVENT)
	{
		printf("%d %s\n", sdlEvent.user.data1, sdlEvent.user.data2);
	}

}

void Scene2::Update(const float deltaTime)
{
	spaceShip->Update(deltaTime);
	Vec3 origin(75.0f, 35.5f, 0.0f);
	printf("distance from origin %.2f \n", VMath::distance(spaceShip->getPos(), origin));
	if (VMath::distance(spaceShip->getPos(), origin) < 35.0f)
	{
		SDL_Event event;
		SDL_memset(&event, 0, sizeof(event));
		event.type = scene->getChangeScene();
		event.user.code = 7;
		event.user.data1 = nullptr;
		event.user.data2 = nullptr;
		SDL_PushEvent(&event);
	}
}

// Changes 

void Scene2::Render()
{
	/// Clear the screen
	SDL_RenderPresent(renderer);
	SDL_SetRenderDrawColor(renderer, 50, 0, 70, 0);
	SDL_RenderClear(renderer);

	SDL_Rect square;
	Vec3 screenCoords;
	int w, h;

	// Preetish this is the code that makes the camera follow the player// recalculate the projection matrix to focus on the player position
	SDL_GetWindowSize(windowPtr, &w, &h);
	Matrix4 ndc = MMath::viewportNDC(w, h);
	Vec3 playerPos = spaceShip->getPos();
	float left = playerPos.x - 0.2f * 40.0f;
	float right = playerPos.x + 0.8f * 40.0f;
	float bottom = playerPos.y - 0.5f * 50.0f;
	float top = playerPos.y + 0.5f * 50.0f;
	Matrix4 ortho = MMath::orthographic(left, right, bottom, top, 0.0f, 1.0f);
	projectionMatrix = ndc * ortho;
	inverseProjection = MMath::inverse(projectionMatrix);

	/// Draw your scene here

	screenCoords = projectionMatrix * background->getPos();
	SDL_QueryTexture(background->getTexture(), nullptr, nullptr, &w, &h);
	square.x = static_cast<int>(screenCoords.x);
	square.y = static_cast<int>(screenCoords.y);
	square.w = w * 10;
	square.h = h * 10;
	SDL_RenderCopyEx(renderer, background->getTexture(), nullptr, &square, 0.0, nullptr, SDL_FLIP_NONE);

	screenCoords = projectionMatrix * sun->getPos();
	SDL_QueryTexture(sun->getTexture(), nullptr, nullptr, &w, &h);
	square.x = static_cast<int>(screenCoords.x);
	square.y = static_cast<int>(screenCoords.y);
	square.w = w;
	square.h = h;
	SDL_RenderCopyEx(renderer, sun->getTexture(), nullptr, &square, 0.0, nullptr, SDL_FLIP_NONE);

	screenCoords = projectionMatrix * planet->getPos();
	SDL_QueryTexture(planet->getTexture(), nullptr, nullptr, &w, &h);
	square.x = static_cast<int>(screenCoords.x);
	square.y = static_cast<int>(screenCoords.y);
	square.w = w * 2;
	square.h = h * 2;
	SDL_RenderCopyEx(renderer, planet->getTexture(), nullptr, &square, 0.0, nullptr, SDL_FLIP_NONE);

	screenCoords = projectionMatrix * boss->getPos();
	SDL_QueryTexture(boss->getTexture(), nullptr, nullptr, &w, &h);
	square.x = static_cast<int>(screenCoords.x);
	square.y = static_cast<int>(screenCoords.y);
	square.w = w * 2;
	square.h = h * 2;
	SDL_RenderCopyEx(renderer, boss->getTexture(), nullptr, &square, 0.0, nullptr, SDL_FLIP_NONE);

	screenCoords = projectionMatrix * minion->getPos();
	SDL_QueryTexture(minion->getTexture(), nullptr, nullptr, &w, &h);
	square.x = static_cast<int>(screenCoords.x);
	square.y = static_cast<int>(screenCoords.y);
	square.w = w;
	square.h = h;
	SDL_RenderCopyEx(renderer, minion->getTexture(), nullptr, &square, 0.0, nullptr, SDL_FLIP_NONE);

	screenCoords = projectionMatrix * minion1->getPos();
	SDL_QueryTexture(minion1->getTexture(), nullptr, nullptr, &w, &h);
	square.x = static_cast<int>(screenCoords.x);
	square.y = static_cast<int>(screenCoords.y);
	square.w = w;
	square.h = h;
	SDL_RenderCopyEx(renderer, minion1->getTexture(), nullptr, &square, 0.0, nullptr, SDL_FLIP_NONE);

	screenCoords = projectionMatrix * obstacle->getPos();
	SDL_QueryTexture(obstacle->getTexture(), nullptr, nullptr, &w, &h);
	square.x = static_cast<int>(screenCoords.x);
	square.y = static_cast<int>(screenCoords.y);
	square.w = w * 2;
	square.h = h * 2;
	SDL_RenderCopyEx(renderer, obstacle->getTexture(), nullptr, &square, 0.0, nullptr, SDL_FLIP_NONE);


	screenCoords = projectionMatrix * spaceShip->getPos();
	SDL_QueryTexture(spaceShip->getTexture(), nullptr, nullptr, &w, &h);
	square.x = static_cast<int>(screenCoords.x);
	square.y = static_cast<int>(screenCoords.y);
	square.w = w;
	square.h = h;
	if (spaceShip->getVelocity().x >= 0.0)
	{
		SDL_RenderCopyEx(renderer, spaceShip->getTexture(), nullptr, &square, 0.0, nullptr, SDL_FLIP_NONE);
	}
	else {
		SDL_RenderCopyEx(renderer, spaceShip->getTexture(), nullptr, &square, 0.0, nullptr, SDL_FLIP_HORIZONTAL);
	}
	SDL_RenderPresent(renderer);

}


void Scene2::OnDestroy()
{

}