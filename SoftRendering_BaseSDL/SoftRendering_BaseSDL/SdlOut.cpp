/*#include <stdio.h>
#include <stdlib.h>

#include "SDL.h"
#include "SDL_revision.h"

int main(int argc, char* argv[])
{
	SDL_version compiled;
	SDL_version linked;

	/* Enable standard application logging 
	SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);

#if SDL_VERSION_ATLEAST(2, 0, 0)
	SDL_Log("Compiled with SDL 2.0 or newer\n");
#else
	SDL_Log("Compiled with SDL older than 2.0\n");
#endif
	SDL_VERSION(&compiled);
	SDL_Log("Compiled version: %d.%d.%d.%d (%s)\n",
		compiled.major, compiled.minor, compiled.patch,
		SDL_REVISION_NUMBER, SDL_REVISION);
	SDL_GetVersion(&linked);
	SDL_Log("Linked version: %d.%d.%d.%d (%s)\n",
		linked.major, linked.minor, linked.patch,
		SDL_GetRevisionNumber(), SDL_GetRevision());
	SDL_Quit();
	system("pause");
	return (0);
}
*/

#include <SDL.h>
#include <stdio.h>
#include <string>
#include <cmath>

//Screen dimension constants
const int SCREEN_WIDTH = 512;
const int SCREEN_HEIGHT = 512;

//Starts up SDL and creates window
bool init();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		gWindow = SDL_CreateWindow("SoftRenderingOutput", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_SOFTWARE);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
		}
	}

	return success;
}

void close()
{
	//Destroy window    
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

void SDLDrawPixel(int x, int y)
{
	SDL_RenderDrawPoint(gRenderer, x, SCREEN_HEIGHT - 1 - y);
}

int main(int argc, char* args[])
{
	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//Main loop flag
		bool quit = false;
		//Event handler
		SDL_Event e;

		//While application is running
		while (!quit)
		{
			//Handle events on queue
			while (SDL_PollEvent(&e) != 0)
			{
				//User requests quit
				if (e.type == SDL_QUIT)
				{
					quit = true;
				}
			}

			//Clear screen
			SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0xFF);
			SDL_RenderClear(gRenderer);

			//Draw vertical line of yellow dots
			SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
			//for (int i = 0; i < SCREEN_HEIGHT; i++)
			//{
			//	SDLDrawPixel(SCREEN_WIDTH / 2, i);
			//}


			SDLDrawPixel(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

			//Update screen
			SDL_RenderPresent(gRenderer);
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}


