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
#include "model.h"
#include "geometry.h"

//Screen dimension constants
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 1024;

//Starts up SDL and creates window
bool init();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;
Model *model = NULL;
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
	delete model;
}

void SDLDrawPixel(int x, int y)
{
	SDL_RenderDrawPoint(gRenderer, x, SCREEN_HEIGHT - 1 - y);
}

//绘制线段函数，处理翻转和过高逻辑。
void line(int x0, int y0, int x1, int y1) {
	bool steep = false;
	if (std::abs(x0-x1)<std::abs(y0-y1)){
	//如果线段在平面上长小于高，那么转置
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}
	if (x0>x1){
	//如果x0大于x1，那么左右翻转
		std::swap(x0, x1);
		std::swap(y0, y1);
	}
	int dx = x1 - x0;
	int dy = y1 - y0;
	int derror2 = std::abs(dy*2);
	int error2 = 0;
	int y = y0;
	for ( int x=x0;x<=x1;x++){
		if (steep){
		//如果是转置的图像，那么反转回来
			SDLDrawPixel(y, x);
		}
		else{
			SDLDrawPixel(x, y);
		}
		error2 += derror2;
		if (error2>dx){
			y += (y1 > y0 ? 1 : -1);
			error2 -= dx*2;
		}
	}
}

/*
void line(int x0, int y0, int x1, int y1) {
	bool steep = false;
	if (std::abs(x0 - x1) < std::abs(y0 - y1))
		//如果线段在平面上长小于高，那么转置
	{
		std::swap(x0, y0);
		std::swap(x0, y1);
		steep = true;
	}
	if (x0 > x1)
	{
		//如果x0小于x1，那么左右翻转
		std::swap(x0, x1);
		std::swap(y0, y1);

	}

	for (int x = x0; x <= x1; x++)
	{
		float t = (x - x0) / (float)(x1 - x0);
		int y = y0*(1. - t) + y1 * t;
		if (steep)
		{
			//如果是转置的图像，那么反转回来
			SDLDrawPixel(y, x);
		}
		else
		{
			SDLDrawPixel(x, y);
		}
	}
}
*/
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
			//--------------------主渲染代码--------------------
			//DrawLineTest
			/*
			for (int i = 0;i<=1;i++)
			{
				SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
				line(65, 100, 400, 200);
				SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
				line(100, 65, 200, 400);
				SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 255);
				line(400, 200, 65, 100);
			}*/
			
			SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 255);
			model = new Model("obj/african_head.obj");
			//model = new Model("obj/Testbox.obj");
			for (int i = 0; i < model->nfaces(); i++)
			{
				std::vector<int> face = model->face(i);
				for (int j = 0;j<3;j++)
				{
					Vec3f v0 = model->vert(face[j]);
					Vec3f v1 = model->vert(face[(j + 1)%3 ]);
					int x0 = (v0.x + 1.)*SCREEN_WIDTH / 2;
					int y0 = (v0.y + 1.)*SCREEN_HEIGHT /2;
					int x1 = (v1.x + 1.)*SCREEN_WIDTH / 2;
					int y1 = (v1.y + 1.)*SCREEN_HEIGHT /2;
					line(x0, y0, x1, y1);

				}
			}
			/*
			SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
			line(128, 128, 384, 128);
			
			SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
			line(384, 128, 384, 384);
			*/
	


			//Update screen
			SDL_RenderPresent(gRenderer);
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}


