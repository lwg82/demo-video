#include "main.h"

/*
https://blog.csdn.net/leixiaohua1020/article/details/38868499
* 本程序是基于FFmpeg函数的最简单的程序。它可以打印出FFmpeg类库的下列信息：
* Protocol:  FFmpeg类库支持的协议
* AVFormat : FFmpeg类库支持的封装格式
* AVCodec : FFmpeg类库支持的编解码器
* AVFilter : FFmpeg类库支持的滤镜
* Configure : FFmpeg类库的配置信息
*/

#include <cstdio>
#include <exception>

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
#include "SDL.h"
};

#undef main 


//Refresh Event
#define SFM_REFRESH_EVENT  (SDL_USEREVENT + 1)
#define SFM_BREAK_EVENT    (SDL_USEREVENT + 2)

int thread_exit = 0;
int thread_pause = 0;

bool generate_rgb24_colorbar(int width, int height, unsigned char *buf, int buf_size)
{
	int bar_width = 0;

	int malloc_size = width * height * 3;

	if (buf_size < malloc_size)
		return false;

	memset(buf, 0, malloc_size);

	bar_width = width / 8;

	for (int j = 0; j < height; ++j)
	{
		for (int i = 0; i < width; ++i)
		{
			int bar_number = i / bar_width;

			switch (bar_number)
			{
				case 0: 
				{
					buf[(j*width + i) * 3 + 0] = 255;
					buf[(j*width + i) * 3 + 1] = 255;
					buf[(j*width + i) * 3 + 2] = 255;

					break;
				}

				case 1: 
				{
					buf[(j*width + i) * 3 + 0] = 255;
					buf[(j*width + i) * 3 + 1] = 255;
					buf[(j*width + i) * 3 + 2] = 0;

					break;
				}

				case 2: 
				{
					buf[(j*width + i) * 3 + 0] = 0;
					buf[(j*width + i) * 3 + 1] = 255;
					buf[(j*width + i) * 3 + 2] = 255;

					break;
				}

				case 3: 
				{
					buf[(j*width + i) * 3 + 0] = 0;
					buf[(j*width + i) * 3 + 1] = 255;
					buf[(j*width + i) * 3 + 2] = 0;

					break;
				}

				case 4: 
				{
					buf[(j*width + i) * 3 + 0] = 255;
					buf[(j*width + i) * 3 + 1] = 0;
					buf[(j*width + i) * 3 + 2] = 255;

					break;
				}

				case 5: 
				{
					buf[(j*width + i) * 3 + 0] = 255;
					buf[(j*width + i) * 3 + 1] = 0;
					buf[(j*width + i) * 3 + 2] = 0;

					break;
				}

				case 6: 
				{
					buf[(j*width + i) * 3 + 0] = 0;
					buf[(j*width + i) * 3 + 1] = 0;
					buf[(j*width + i) * 3 + 2] = 255;

					break;
				}

				case 7: 
				{
					buf[(j*width + i) * 3 + 0] = 0;
					buf[(j*width + i) * 3 + 1] = 0;
					buf[(j*width + i) * 3 + 2] = 0;

					break;
				}
			}
		}
	}

	return true;
}



int SDLCALL thread_function(void *data)
{
	thread_exit = 0;
	thread_pause = 0;

	while (!thread_exit) {
		if (!thread_pause) {
			SDL_Event event;
			event.type = SFM_REFRESH_EVENT;
			SDL_PushEvent(&event);
		}
		SDL_Delay(40);
	}
	thread_exit = 0;
	thread_pause = 0;
	//Break
	SDL_Event event;
	event.type = SFM_BREAK_EVENT;
	SDL_PushEvent(&event);

	return 0;
}


int main(int argc, char *argv[])
{
	int screen_width = 400;
	int screen_height = 300;
	int line_size = screen_width * 3;
	int buf_size =  screen_width * screen_height * 3;

	unsigned char *buf = (unsigned char *)malloc(buf_size);

	generate_rgb24_colorbar(screen_width, screen_height, buf, buf_size);

	SDL_Window   *window;
	SDL_Renderer *renderer;
	SDL_Texture  *texture;
	SDL_Rect      rect;

	SDL_Thread    *thread_video;
	SDL_Event      event;
	



	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER))
	{
		printf("Could not initialize SDL - %s\n", SDL_GetError());
		return -1;
	}

	window = SDL_CreateWindow("Simple ffmpeg player's Window",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		screen_width,
		screen_height,
		SDL_WINDOW_OPENGL);

	if (!window) {
		printf("SDL: could not create window - exiting:%s\n", SDL_GetError());
		return -1;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);

	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING,
		screen_width, screen_height);

	thread_video = SDL_CreateThread(thread_function, NULL, NULL);

	while (true)
	{
		//Wait
		SDL_WaitEvent(&event);

		if (event.type == SFM_REFRESH_EVENT) {
			
			//SDL---------------------------
			SDL_UpdateTexture(texture, NULL, buf, line_size);
			SDL_RenderClear(renderer);
			//SDL_RenderCopy( sdlRenderer, sdlTexture, &sdlRect, &sdlRect );  
			SDL_RenderCopy(renderer, texture, NULL, NULL);
			SDL_RenderPresent(renderer);
			//SDL End-----------------------
			
			
		}
		else if (event.type == SDL_KEYDOWN) {
			//Pause
			if (event.key.keysym.sym == SDLK_SPACE)
				thread_pause = !thread_pause;
		}
		else if (event.type == SDL_QUIT) {
			thread_exit = 1;
		}
		else if (event.type == SFM_BREAK_EVENT) {
			break;
		}
	}

	SDL_Quit();

	return 0;
}