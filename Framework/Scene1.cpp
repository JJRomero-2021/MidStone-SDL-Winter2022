#include <glew.h>
#include <SDL.h>
#include "Scene1.h"
#include "MMath.h"

Scene1::Scene1(SDL_Window* sdlWindow_, GameManager* scene_)
{
	window = sdlWindow_;
}

Scene1::~Scene1() {}

bool Scene1::OnCreate() {
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
	Matrix4 ortho = MMath::orthographic(0.0f, 30.0f, 0.0f, 15.0f, 0.0f, 1.0f);
	projectionMatrix = ndc * ortho;

	return true;
}

void Scene1::HandleEvents(const SDL_Event& sdlEvent) {

}

void Scene1::Update(const float deltaTime) {

}

void Scene1::Render()
{
	/// Clear the screen
	SDL_RenderPresent(renderer);
	SDL_SetRenderDrawColor(renderer, 50, 50, 50, 0);
	SDL_RenderClear(renderer);

	/// Draw your scene here

}


void Scene1::OnDestroy() {

}