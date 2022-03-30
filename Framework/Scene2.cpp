#include <glew.h>
#include <SDL.h>
#include "Scene2.h"
#include "MMath.h"


Scene2::Scene2(SDL_Window* sdlWindow_, GameManager* scene_)
{
	window = sdlWindow_;
	background = new Body(Vec3(-10.0f, 40.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 1.0f);
	boss = new Body(Vec3(10.0f, 20.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 1.0f);
	minion = new Body(Vec3(16.0f, 22.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 1.0f);
	minion1 = new Body(Vec3(6.0f, 22.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 1.0f);
	spaceShip = new Body(Vec3(0.0f, 7.8f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 1.0f);
	obstacle = new Body(Vec3(22.0f, 30.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 1.0f);
	scene = scene_;
}

Scene2::~Scene2() 
{
	delete spaceShip;
	delete boss;
	delete background;
	delete obstacle;
}

bool Scene2::OnCreate() {

	renderer = SDL_GetRenderer(window);

	if (!renderer) {

		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	}

	if (!renderer) {

		std::cout << "SDL_Error: " << SDL_GetError() << std::endl;

		return false;

	}
	int w, h;
	SDL_GetWindowSize(window, &w, &h);

	Matrix4 ndc = MMath::viewportNDC(w, h);
	Matrix4 ortho = MMath::orthographic(-10.0f, 30.0f, -10.0f, 40.0f, 0.0f, 1.0f);
	projectionMatrix = ndc * ortho;
	IMG_Init(IMG_INIT_PNG);

	SDL_Surface* backgroundImage = IMG_Load("textures/darkPurple.png");
	SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundImage);
	if (backgroundImage == nullptr)
	{
		printf("Can't open textures/darkPurple.png\n");
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
	/// Draw your scene here

	screenCoords = projectionMatrix * background->getPos();
	SDL_QueryTexture(background->getTexture(), nullptr, nullptr, &w, &h);
	square.x = static_cast<int>(screenCoords.x);
	square.y = static_cast<int>(screenCoords.y);
	square.w = w * 5;
	square.h = h * 5;
	SDL_RenderCopyEx(renderer, background->getTexture(), nullptr, &square, 0.0, nullptr, SDL_FLIP_NONE);

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