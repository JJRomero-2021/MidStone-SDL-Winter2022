#include <glew.h>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "Window.h"
#include "Scene.h"
#include "Scene1.h"
#include "MMath.h"
#include "VMath.h"
#include "Timer.h"
#include "GameManager.h"

using namespace MATH;
using namespace std;



Scene1::Scene1(SDL_Window* sdlWindow_, GameManager* scene_)
{
	windowPtr = sdlWindow_;
	scene = scene_;

	background = new Body(Vec3(-100.0f, 100.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 1.0f);
	sun = new Body(Vec3(50.0f, 20.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 1.0f);
	neptune = new Body(Vec3(60.0f, 40.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 1.0f);
	uranus = new Body(Vec3(110.0f, 40.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 1.0f);
	jupiter = new Body(Vec3(-20.0f, 40.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 1.0f);

	spaceShip = new Body(Vec3(-8.0f, 7.8f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 1.0f);
	enemy1 = new Body(Vec3(28.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 1.0f);
	enemy2 = new Body(Vec3(28.0f, 10.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 1.0f);
	enemy3 = new Body(Vec3(28.0f, 19.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 1.0f);
	enemy4 = new Body(Vec3(28.0f, 28.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 1.0f);
	enemy5 = new Body(Vec3(28.0f, 36.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 1.0f);
	bullets.push_back(new Body(Vec3(-20.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 1.0f));
	
	enemyAttack1 = new Body(Vec3(26.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 1.0f);
	enemyAttack2 = new Body(Vec3(26.0f, 10.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 1.0f);
	enemyAttack3 = new Body(Vec3(26.0f, 19.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 1.0f);
	enemyAttack4 = new Body(Vec3(26.0f, 28.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 1.0f);
	enemyAttack5 = new Body(Vec3(26.0f, 36.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 1.0f);

	
	spaceshipDirec = 0;
	totalTargetsDestroyed = 0;
	spaceShipDestroyed = false;

}

Scene1::~Scene1()
{
	delete jupiter;
	delete neptune;
	delete uranus;
	delete sun;
	delete spaceShip;
	delete enemy1, enemy2, enemy3, enemy4, enemy5, enemyAttack1, enemyAttack2, enemyAttack3, enemyAttack4, enemyAttack5;
	delete background;
	for (Body* body : bullets) {
		delete body;
	}
}

bool Scene1::OnCreate()
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

	SDL_Surface* neptuneImage = IMG_Load("textures/Neptune.png");
	SDL_Texture* neptuneTexture = SDL_CreateTextureFromSurface(renderer, neptuneImage);
	if (neptuneImage == nullptr)
	{
		printf("Can't open textures/Neptune.png\n");
		return false;
	}

	SDL_Surface* uranusImage = IMG_Load("textures/Uranus.png");
	SDL_Texture* uranusTexture = SDL_CreateTextureFromSurface(renderer, uranusImage);
	if (uranusImage == nullptr)
	{
		printf("Can't open textures/Uranus.png\n");
		return false;
	}

	SDL_Surface* jupiterImage = IMG_Load("textures/Jupiter.png");
	SDL_Texture* jupiterTexture = SDL_CreateTextureFromSurface(renderer, jupiterImage);
	if (jupiterImage == nullptr)
	{
		printf("Can't open textures/Jupiter.png\n");
		return false;
	}

	SDL_Surface* enemyImage = IMG_Load("textures/enemy-spaceship.png");
	SDL_Texture* enemyTexture = SDL_CreateTextureFromSurface(renderer, enemyImage);
	if (enemyImage == nullptr)
	{
		printf("Can't open textures/enemy.png\n");
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

	SDL_Surface* enemyAttackImage = IMG_Load("textures/enemyAttack.png");
	SDL_Texture* enemyAttackTexture = SDL_CreateTextureFromSurface(renderer, enemyAttackImage);
	if (enemyAttackImage == nullptr)
	{
		printf("Can't open textures/enemy attack.png\n");
		return false;
	}

	sun->setTexture(sunTexture);
	SDL_FreeSurface(sunImage);
	
	neptune->setTexture(neptuneTexture);
	SDL_FreeSurface(neptuneImage);
	
	uranus->setTexture(uranusTexture);
	SDL_FreeSurface(uranusImage);

	jupiter->setTexture(jupiterTexture);
	SDL_FreeSurface(jupiterImage);

	spaceShip->setTexture(spaceShipTexture);
	SDL_FreeSurface(spaceShipImage);

	background->setTexture(backgroundTexture);
	SDL_FreeSurface(backgroundImage);

	enemy1->setTexture(enemyTexture);	
	enemy1->obstacleOriginalTexture(true);
	enemy1->obstacleBlastTexture(false);

	enemy2->setTexture(enemyTexture);

	enemy2->obstacleOriginalTexture(true);
	enemy2->obstacleBlastTexture(false);

	enemy3->setTexture(enemyTexture);
	
	enemy3->obstacleOriginalTexture(true);
	enemy3->obstacleBlastTexture(false);

	enemy4->setTexture(enemyTexture);

	enemy4->obstacleOriginalTexture(true);
	enemy4->obstacleBlastTexture(false);

	enemy5->setTexture(enemyTexture);
	SDL_FreeSurface(enemyImage);
	enemy5->obstacleOriginalTexture(true);
	enemy5->obstacleBlastTexture(false);

	for (int i = 0; i < bullets.size(); i++) {
		bullets[i]->setTexture(bulletTexture);
	}
	SDL_FreeSurface(bulletImage);

	enemyAttack1->setTexture(enemyAttackTexture);
	
	enemyAttack2->setTexture(enemyAttackTexture);
	
	enemyAttack3->setTexture(enemyAttackTexture);
	
	enemyAttack4->setTexture(enemyAttackTexture);
	
	enemyAttack5->setTexture(enemyAttackTexture);
	SDL_FreeSurface(enemyAttackImage);



	return true;

}

void Scene1::HandleEvents(const SDL_Event& sdlEvent)
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

void Scene1::Update(const float deltaTime)
{
	spaceShip->Update(deltaTime);


	enemy1->ApplyForce(Vec3(-0.4, 0.0, 0.0));
	enemy1->isMoving(true);
	enemy2->ApplyForce(Vec3(-0.5, 0.0, 0.0));
	enemy2->isMoving(true);
	enemy3->ApplyForce(Vec3(-0.8, 0.0, 0.0));
	enemy3->isMoving(true);
	enemy4->ApplyForce(Vec3(-0.6, 0.0, 0.0));
	enemy4->isMoving(true);
	enemy5->ApplyForce(Vec3(-0.3, 0.0, 0.0));
	enemy5->isMoving(true);

	enemyAttack1->ApplyForce(Vec3(-4.0, 0.0, 0.0));
	enemyAttack1->isMoving(true);
	enemyAttack2->ApplyForce(Vec3(-5.0, 0.0, 0.0));
	enemyAttack2->isMoving(true);
	enemyAttack3->ApplyForce(Vec3(-5.8, 0.0, 0.0));
	enemyAttack3->isMoving(true);
	enemyAttack4->ApplyForce(Vec3(-6.0, 0.0, 0.0));
	enemyAttack4->isMoving(true);
	enemyAttack5->ApplyForce(Vec3(-4.5, 0.0, 0.0));
	enemyAttack5->isMoving(true);


	enemy1->Update(deltaTime);
	enemy2->Update(deltaTime);
	enemy3->Update(deltaTime);
	enemy4->Update(deltaTime);
	enemy5->Update(deltaTime);

	enemyAttack1->Update(deltaTime);
	enemyAttack2->Update(deltaTime);
	enemyAttack3->Update(deltaTime);
	enemyAttack4->Update(deltaTime);
	enemyAttack5->Update(deltaTime);


	for (int i = 0; i < bullets.size(); i++) {
		bullets[i]->Update(deltaTime);
	}

	SDL_Surface* blastImage = IMG_Load("textures/blast.png");
	SDL_Texture* blastTexture = SDL_CreateTextureFromSurface(renderer, blastImage);

	for (int i = 0; i < bullets.size(); i++) {
		Vec3 bullPos = bullets[i]->getPos();
		Vec3 obstacPos1 = enemy1->getPos() + Vec3(-0.5, 0.5, 0.0);
		float dist1 = VMath::distance(bullPos, obstacPos1);
		if (dist1 < 2.0 && enemy1->getObstactleOriginalTexture() == true) {
			enemy1->setTexture(blastTexture);
			
			Vec3 obstacCurrPos = enemy1->getPos();
			enemy1->setPos(obstacCurrPos + Vec3(-0.5, 0.5, 0.0));
			enemy1->targetDestroyed(true);
			enemy1->obstacleOriginalTexture(false);
			enemy1->obstacleBlastTexture(true);
		}

		Vec3 obstacPos2 = enemy2->getPos() + Vec3(-0.5, 0.5, 0.0);
		float dist2 = VMath::distance(bullPos, obstacPos2);
		if (dist2 < 2.0 && enemy2->getObstactleOriginalTexture() == true) {
			enemy2->setTexture(blastTexture);
			
			Vec3 obstacCurrPos = enemy2->getPos();
			enemy2->setPos(obstacCurrPos + Vec3(-0.5, 0.5, 0.0));
			enemy2->targetDestroyed(true);
			enemy2->obstacleOriginalTexture(false);
			enemy2->obstacleBlastTexture(true);
		}

		Vec3 obstacPos3 = enemy3->getPos() + Vec3(-0.5, 0.5, 0.0);
		float dist3 = VMath::distance(bullPos, obstacPos3);
		if (dist3 < 2.0 && enemy3->getObstactleOriginalTexture() == true) {
			enemy3->setTexture(blastTexture);
			
			Vec3 obstacCurrPos = enemy3->getPos();
			enemy3->setPos(obstacCurrPos + Vec3(-0.5, 0.5, 0.0));
			enemy3->targetDestroyed(true);
			enemy3->obstacleOriginalTexture(false);
			enemy3->obstacleBlastTexture(true);
		}

		Vec3 obstacPos4 = enemy4->getPos() + Vec3(-0.5, 0.5, 0.0);
		float dist4 = VMath::distance(bullPos, obstacPos4);
		if (dist4 < 2.0 && enemy4->getObstactleOriginalTexture() == true) {
			enemy4->setTexture(blastTexture);
			
			Vec3 obstacCurrPos = enemy4->getPos();
			enemy4->setPos(obstacCurrPos + Vec3(-0.5, 0.5, 0.0));
			enemy4->targetDestroyed(true);
			enemy4->obstacleOriginalTexture(false);
			enemy4->obstacleBlastTexture(true);
		}

		Vec3 obstacPos5 = enemy5->getPos() + Vec3(-0.5, 0.5, 0.0);
		float dist5 = VMath::distance(bullPos, obstacPos5);
		if (dist5 < 2.0 && enemy5->getObstactleOriginalTexture() == true) {
			enemy5->setTexture(blastTexture);
			
			Vec3 obstacCurrPos = enemy5->getPos();
			enemy5->setPos(obstacCurrPos + Vec3(-0.5, 0.5, 0.0));
			enemy5->targetDestroyed(true);
			enemy5->obstacleOriginalTexture(false);
			enemy5->obstacleBlastTexture(true);
		}

	}

	Vec3 rocketPos1 = enemyAttack1->getPos();
	Vec3 rocketPos2 = enemyAttack2->getPos();
	Vec3 rocketPos3 = enemyAttack3->getPos();
	Vec3 rocketPos4 = enemyAttack4->getPos();
	Vec3 rocketPos5 = enemyAttack5->getPos();
	Vec3 spaceshipPos = spaceShip->getPos();
	Vec3 enemyPos1 = enemy1->getPos();
	Vec3 enemyPos2 = enemy2->getPos();
	Vec3 enemyPos3 = enemy3->getPos();
	Vec3 enemyPos4 = enemy4->getPos();
	Vec3 enemyPos5 = enemy5->getPos();

	float SSDist1 = VMath::distance(rocketPos1, spaceshipPos);
	float SSDist2 = VMath::distance(rocketPos2, spaceshipPos);
	float SSDist3 = VMath::distance(rocketPos3, spaceshipPos);
	float SSDist4 = VMath::distance(rocketPos4, spaceshipPos);
	float SSDist5 = VMath::distance(rocketPos5, spaceshipPos);
	float SSDist6 = VMath::distance(enemyPos1, spaceshipPos);
	float SSDist7 = VMath::distance(enemyPos2, spaceshipPos);
	float SSDist8 = VMath::distance(enemyPos3, spaceshipPos);
	float SSDist9 = VMath::distance(enemyPos4, spaceshipPos);
	float SSDist10 = VMath::distance(enemyPos5, spaceshipPos);

	if (SSDist1 < 1.5f) {
		spaceShipDestroyed = true;
	}
	if (SSDist2 < 1.5f) {
		spaceShipDestroyed = true;
	}
	if (SSDist3 < 1.5f) {
		spaceShipDestroyed = true;
	}
	if (SSDist4 < 1.5f) {
		spaceShipDestroyed = true;
	}
	if (SSDist5 < 1.5f) {
		spaceShipDestroyed = true;
	}
	if (SSDist6 < 1.5f) {
		spaceShipDestroyed = true;
	}
	if (SSDist7 < 1.5f) {
		spaceShipDestroyed = true;
	}
	if (SSDist8 < 1.5f) {
		spaceShipDestroyed = true;
	}
	if (SSDist9 < 1.5f) {
		spaceShipDestroyed = true;
	}
	if (SSDist10 < 1.5f) {
		spaceShipDestroyed = true;
	}

	if (spaceShipDestroyed == true) {
		spaceShip->setTexture(blastTexture);
		SDL_FreeSurface(blastImage);
		printf("Spaceship destroyed");
	}


	if (enemy1->getTargetDestroyed() == true) {
		totalTargetsDestroyed++;
		enemy1->targetDestroyed(false);
	}
	if (enemy2->getTargetDestroyed() == true) {
		totalTargetsDestroyed++;
		enemy2->targetDestroyed(false);
	}
	if (enemy3->getTargetDestroyed() == true) {
		totalTargetsDestroyed++;
		enemy3->targetDestroyed(false);
	}
	if (enemy4->getTargetDestroyed() == true) {
		totalTargetsDestroyed++;
		enemy4->targetDestroyed(false);
	}
	if (enemy5->getTargetDestroyed() == true) {
		totalTargetsDestroyed++;
		enemy5->targetDestroyed(false);
	}

	printf("%d", totalTargetsDestroyed);

	if (totalTargetsDestroyed == 5) {
		printf("all destroyed");

	}

	Vec3 origin(0.0f, 0.0f, 0.0f);
	//printf("distance from origin %.2f \n", VMath::distance(spaceShip->getPos(), origin));
	if (VMath::distance(spaceShip->getPos(), origin) < 1.5f)
	{
		SDL_Event event;
		SDL_memset(&event, 0, sizeof(event));
		event.type = scene->getChangeScene();
		event.user.code = 3;
		event.user.data1 = nullptr;
		event.user.data2 = nullptr;
		SDL_PushEvent(&event);
	}
}

void Scene1::Render()
{
	/// Clear the screen
	SDL_SetRenderDrawColor(renderer, 0, 0, 50, 0);
	SDL_RenderClear(renderer);

	SDL_Rect square;
	Vec3 screenCoords;
	int w, h;

	// recalculate the projection matrix to focus on the player position
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
	square.w = w * 50;
	square.h = h * 50;
	SDL_RenderCopyEx(renderer, background->getTexture(), nullptr, &square, 0.0, nullptr, SDL_FLIP_NONE);

	screenCoords = projectionMatrix * sun->getPos();
	SDL_QueryTexture(sun->getTexture(), nullptr, nullptr, &w, &h);
	square.x = static_cast<int>(screenCoords.x);
	square.y = static_cast<int>(screenCoords.y);
	square.w = w;
	square.h = h;
	SDL_RenderCopyEx(renderer, sun->getTexture(), nullptr, &square, 0.0, nullptr, SDL_FLIP_NONE);

	screenCoords = projectionMatrix * neptune->getPos();
	SDL_QueryTexture(neptune->getTexture(), nullptr, nullptr, &w, &h);
	square.x = static_cast<int>(screenCoords.x);
	square.y = static_cast<int>(screenCoords.y);
	square.w = w / 2;
	square.h = h / 2;
	SDL_RenderCopyEx(renderer, neptune->getTexture(), nullptr, &square, 0.0, nullptr, SDL_FLIP_NONE);

	screenCoords = projectionMatrix * uranus->getPos();
	SDL_QueryTexture(uranus->getTexture(), nullptr, nullptr, &w, &h);
	square.x = static_cast<int>(screenCoords.x);
	square.y = static_cast<int>(screenCoords.y);
	square.w = w / 2;
	square.h = h / 2;
	SDL_RenderCopyEx(renderer, uranus->getTexture(), nullptr, &square, 0.0, nullptr, SDL_FLIP_NONE);

	screenCoords = projectionMatrix * jupiter->getPos();
	SDL_QueryTexture(jupiter->getTexture(), nullptr, nullptr, &w, &h);
	square.x = static_cast<int>(screenCoords.x);
	square.y = static_cast<int>(screenCoords.y);
	square.w = w * 2;
	square.h = h * 2;
	SDL_RenderCopyEx(renderer, jupiter->getTexture(), nullptr, &square, 0.0, nullptr, SDL_FLIP_NONE);

	for (int i = 0; i < bullets.size(); i++) {
		screenCoords = projectionMatrix * bullets[i]->getPos();
		SDL_QueryTexture(bullets[i]->getTexture(), nullptr, nullptr, &w, &h);
		square.x = static_cast<int>(screenCoords.x) - (w / 2);
		square.y = static_cast<int>(screenCoords.y) - (h / 2);
		square.w = w;
		square.h = h;
		SDL_RenderCopyEx(renderer, bullets[i]->getTexture(), nullptr, &square, 0.0, nullptr, SDL_FLIP_NONE);
	}

	screenCoords = projectionMatrix * enemy1->getPos();
	SDL_QueryTexture(enemy1->getTexture(), nullptr, nullptr, &w, &h);
	square.x = static_cast<int>(screenCoords.x);
	square.y = static_cast<int>(screenCoords.y);
	square.w = w / 4;
	square.h = h / 4;
	SDL_RenderCopyEx(renderer, enemy1->getTexture(), nullptr, &square, 0.0, nullptr, SDL_FLIP_NONE);

	screenCoords = projectionMatrix * enemy2->getPos();
	SDL_QueryTexture(enemy2->getTexture(), nullptr, nullptr, &w, &h);
	square.x = static_cast<int>(screenCoords.x);
	square.y = static_cast<int>(screenCoords.y);
	square.w = w / 4;
	square.h = h / 4;
	SDL_RenderCopyEx(renderer, enemy2->getTexture(), nullptr, &square, 0.0, nullptr, SDL_FLIP_NONE);

	screenCoords = projectionMatrix * enemy3->getPos();
	SDL_QueryTexture(enemy3->getTexture(), nullptr, nullptr, &w, &h);
	square.x = static_cast<int>(screenCoords.x);
	square.y = static_cast<int>(screenCoords.y);
	square.w = w / 4;
	square.h = h / 4;
	SDL_RenderCopyEx(renderer, enemy3->getTexture(), nullptr, &square, 0.0, nullptr, SDL_FLIP_NONE);

	screenCoords = projectionMatrix * enemy4->getPos();
	SDL_QueryTexture(enemy4->getTexture(), nullptr, nullptr, &w, &h);
	square.x = static_cast<int>(screenCoords.x);
	square.y = static_cast<int>(screenCoords.y);
	square.w = w / 4;
	square.h = h / 4;
	SDL_RenderCopyEx(renderer, enemy4->getTexture(), nullptr, &square, 0.0, nullptr, SDL_FLIP_NONE);

	screenCoords = projectionMatrix * enemy5->getPos();
	SDL_QueryTexture(enemy5->getTexture(), nullptr, nullptr, &w, &h);
	square.x = static_cast<int>(screenCoords.x);
	square.y = static_cast<int>(screenCoords.y);
	square.w = w / 4;
	square.h = h / 4;
	SDL_RenderCopyEx(renderer, enemy5->getTexture(), nullptr, &square, 0.0, nullptr, SDL_FLIP_NONE);


	screenCoords = projectionMatrix * spaceShip->getPos();
	SDL_QueryTexture(spaceShip->getTexture(), nullptr, nullptr, &w, &h);
	square.x = static_cast<int>(screenCoords.x);
	square.y = static_cast<int>(screenCoords.y);
	square.w = w;
	square.h = h;
	if (spaceshipDirec == 0)
	{
		SDL_RenderCopyEx(renderer, spaceShip->getTexture(), nullptr, &square, 0.0, nullptr, SDL_FLIP_NONE);
	}
	else
	{
		SDL_RenderCopyEx(renderer, spaceShip->getTexture(), nullptr, &square, 180.0, nullptr, SDL_FLIP_NONE);
	}


	screenCoords = projectionMatrix * enemyAttack1->getPos();
	SDL_QueryTexture(enemyAttack1->getTexture(), nullptr, nullptr, &w, &h);
	square.x = static_cast<int>(screenCoords.x);
	square.y = static_cast<int>(screenCoords.y);
	square.w = w / 4;
	square.h = h / 4;
	SDL_RenderCopyEx(renderer, enemyAttack1->getTexture(), nullptr, &square, 0.0, nullptr, SDL_FLIP_NONE);

	screenCoords = projectionMatrix * enemyAttack2->getPos();
	SDL_QueryTexture(enemyAttack2->getTexture(), nullptr, nullptr, &w, &h);
	square.x = static_cast<int>(screenCoords.x);
	square.y = static_cast<int>(screenCoords.y);
	square.w = w / 4;
	square.h = h / 4;
	SDL_RenderCopyEx(renderer, enemyAttack2->getTexture(), nullptr, &square, 0.0, nullptr, SDL_FLIP_NONE);

	screenCoords = projectionMatrix * enemyAttack3->getPos();
	SDL_QueryTexture(enemyAttack3->getTexture(), nullptr, nullptr, &w, &h);
	square.x = static_cast<int>(screenCoords.x);
	square.y = static_cast<int>(screenCoords.y);
	square.w = w / 4;
	square.h = h / 4;
	SDL_RenderCopyEx(renderer, enemyAttack3->getTexture(), nullptr, &square, 0.0, nullptr, SDL_FLIP_NONE);

	screenCoords = projectionMatrix * enemyAttack4->getPos();
	SDL_QueryTexture(enemyAttack4->getTexture(), nullptr, nullptr, &w, &h);
	square.x = static_cast<int>(screenCoords.x);
	square.y = static_cast<int>(screenCoords.y);
	square.w = w / 4;
	square.h = h / 4;
	SDL_RenderCopyEx(renderer, enemyAttack4->getTexture(), nullptr, &square, 0.0, nullptr, SDL_FLIP_NONE);

	screenCoords = projectionMatrix * enemyAttack5->getPos();
	SDL_QueryTexture(enemyAttack5->getTexture(), nullptr, nullptr, &w, &h);
	square.x = static_cast<int>(screenCoords.x);
	square.y = static_cast<int>(screenCoords.y);
	square.w = w / 4;
	square.h = h / 4;
	SDL_RenderCopyEx(renderer, enemyAttack5->getTexture(), nullptr, &square, 0.0, nullptr, SDL_FLIP_NONE);



	SDL_RenderPresent(renderer);
}


void Scene1::OnDestroy()
{

}