#include "main.h"

/*
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


#define MALLOC_SIZE 40000


char * getConfiguration()
{
	char *info = (char*)malloc(MALLOC_SIZE);

	if (!info)
		std::terminate();

	memset(info, 0, MALLOC_SIZE);

	av_register_all();


	return info;
}

char *getAvFormatInfo()
{
	char *info = (char*)malloc(MALLOC_SIZE);

	if (!info)
		std::terminate();

	memset(info, 0, MALLOC_SIZE);

	av_register_all();

	AVInputFormat *pAVInputFormat = av_iformat_next(NULL);
	AVOutputFormat *pAVOutFormat = av_oformat_next(NULL);

	while (pAVInputFormat)
	{
		sprintf(info, "%s[In ] %10s\n", info, pAVInputFormat->name);

		pAVInputFormat = pAVInputFormat->next;
	}

	while (pAVOutFormat)
	{
		sprintf(info, "%s[Out] %10s\n", info, pAVOutFormat->name);
		pAVOutFormat = pAVOutFormat->next;
	}

	return info;
}


char *getURLProtocolInfo()
{
	char *info = (char*)malloc(MALLOC_SIZE);

	if (!info)
		std::terminate();

	memset(info, 0, MALLOC_SIZE);

	av_register_all();

	struct URLProtocol *pURLProtocol = NULL;
	struct URLProtocol **pURLProtocolOut = &pURLProtocol;

	avio_enum_protocols((void **)pURLProtocolOut, 0);

	while ((*pURLProtocolOut) != NULL)
	{
		sprintf(info, "%s[In ][%10s]\n", info, avio_enum_protocols((void **)pURLProtocolOut, 0));
	}

	// output
	pURLProtocol = NULL;

	avio_enum_protocols((void **)pURLProtocolOut, 1);

	while ((*pURLProtocolOut) != NULL)
	{
		sprintf(info, "%s[Out ][%10s]\n", info, avio_enum_protocols((void **)pURLProtocolOut, 1));
	}

	return info;
}

int main(int argc, char *argv[])
{
	char *info = nullptr;

	info = getAvFormatInfo();
	printf("\n<<AVFormat>>\n%s", info);
	free(info);

	info = getURLProtocolInfo();
	printf("\n<<URLProtocol>>\n%s", info);
	free(info);
	
	getchar();

	return 0;
}