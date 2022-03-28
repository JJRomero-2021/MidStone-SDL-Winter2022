#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>
#include <iostream>
#include <string>
#include "GameManager.h"

// test git
int main(int argc, char* args[]) 
{ /// Standard C-style entry point, you need to use it

	static_assert(sizeof(void*) == 4, "This program is not ready for 64-bit build");
	std::string name = { "GAMES" };
	GameManager *ptr = new GameManager();

	if (ptr->Initialize(name, 1280, 720) == true) {
		ptr->Run();
	}
	delete ptr;

	_CrtDumpMemoryLeaks();
	exit(0);

}