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

int thread_refresh(void *opaque)
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
	char video_file[] = "E:\\study_workspace\\demo-video\\build32\\src\\simple-app\\Debug\\test.mp4";

	AVFormatContext *pAVFormatContext = nullptr;

	int i, video_index;

	AVCodecContext *pAVCodecContext = nullptr;
	AVCodec        *pAVCodec = nullptr;

	AVFrame        *pAVFrame;
	AVFrame        *pAVFrameYUV;

	int   ret, got_picture;

	unsigned char *output_buffer = nullptr;
	AVPacket      *packet = nullptr;

	struct SwsContext *pSwsContext = nullptr;

	int screen_width;
	int screen_height;

	SDL_Window   *window;
	SDL_Renderer *renderer;
	SDL_Texture  *texture;
	SDL_Rect      rect;

	SDL_Thread    *thread_video;
	SDL_Event      event;


	av_register_all();

	//avformat_network_init();

	pAVFormatContext = avformat_alloc_context();

	if (avformat_open_input(&pAVFormatContext, video_file, NULL, NULL) )
	{
		printf("Couldn't open input stream. \n");
		return -1;
	}

	if (avformat_find_stream_info(pAVFormatContext, NULL))
	{
		printf("Couldn't find stream information. \n");
		return -1;
	}

	for (i = 0; i < pAVFormatContext->nb_streams; i++)
	{
		if (pAVFormatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			video_index = i;
			break;
		}
	}

	if (video_index == -1)
	{
		printf("Didn't find a video stream. \n");
		return -1;
	}


	 // 解码器
	pAVCodecContext = pAVFormatContext->streams[video_index]->codec;

	pAVCodec = avcodec_find_decoder(pAVCodecContext->codec_id);

	if (avcodec_open2(pAVCodecContext, pAVCodec, NULL) < 0)
	{
		printf("Could not open codec.\n");
		return -1;
	}

	pAVFrame = av_frame_alloc();
	pAVFrameYUV = av_frame_alloc();

	size_t buffer_size = av_image_get_buffer_size(AV_PIX_FMT_YUV420P, pAVCodecContext->width, pAVCodecContext->height, 1);
	output_buffer = (unsigned char *)av_malloc(buffer_size);

	av_image_fill_arrays(pAVFrameYUV->data, pAVFrameYUV->linesize, output_buffer, AV_PIX_FMT_YUV420P, pAVCodecContext->width, pAVCodecContext->height, 1);

	//Output Info-----------------------------
	printf("---------------- File Information ---------------\n");
	av_dump_format(pAVFormatContext, 0, video_file, 0);
	printf("-------------------------------------------------\n");

	pSwsContext = sws_getContext(pAVCodecContext->width,
		pAVCodecContext->height,
		pAVCodecContext->pix_fmt,
		pAVCodecContext->width,
		pAVCodecContext->height,
		AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER))
	{
		printf("Could not initialize SDL - %s\n", SDL_GetError());
		return -1;
	}

	screen_width = pAVCodecContext->width;
	screen_height = pAVCodecContext->height;

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

	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING,
		pAVCodecContext->width, pAVCodecContext->height);

	rect.x = 0;
	rect.y = 0;
	rect.w = screen_width;
	rect.h = screen_height;

	packet = (AVPacket *)av_malloc(sizeof(AVPacket));

	thread_video = SDL_CreateThread(thread_refresh, NULL, NULL);

	for (;;)
	{
		//Wait
		SDL_WaitEvent(&event);

		if (event.type == SFM_REFRESH_EVENT) {
			while (1) {
				if (av_read_frame(pAVFormatContext, packet) < 0)
					thread_exit = 1;

				if (packet->stream_index == video_index)
					break;
			}
			ret = avcodec_decode_video2(pAVCodecContext, pAVFrame, &got_picture, packet);
			if (ret < 0) {
				printf("Decode Error.\n");
				return -1;
			}
			if (got_picture) {
				sws_scale(pSwsContext, (const unsigned char* const*)pAVFrame->data, pAVFrame->linesize, 0, pAVCodecContext->height, pAVFrameYUV->data, pAVFrameYUV->linesize);
				//SDL---------------------------
				SDL_UpdateTexture(texture, NULL, pAVFrameYUV->data[0], pAVFrameYUV->linesize[0]);
				SDL_RenderClear(renderer);
				//SDL_RenderCopy( sdlRenderer, sdlTexture, &sdlRect, &sdlRect );  
				SDL_RenderCopy(renderer, texture, NULL, NULL);
				SDL_RenderPresent(renderer);
				//SDL End-----------------------
			}
			av_free_packet(packet);
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

	/////////////////
	sws_freeContext(pSwsContext);
	
	SDL_Quit();


	av_frame_free(&pAVFrame);
	av_frame_free(&pAVFrameYUV);

	avcodec_close(pAVCodecContext);

	avformat_close_input(&pAVFormatContext);

	
	return 0;
}