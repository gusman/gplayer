#include <SDL.h>
#include "common/debug.h"
#include "common/player_type.h"
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"

#define SDL_AUDIO_BUFFER_SIZE 4096

AVFormatContext	*gp_avformat	= NULL;
AVCodecContext	*gp_avcodec	= NULL;
AVCodec		*gp_codec	= NULL;
uint8		 g_isdecoding;
uint8		 ga_audio_buff[44112 * 2] = {0, };
uint16		 g_audio_buff_idx = 0;

void player_audio_init(void)
{
	SDL_Init(SDL_INIT_AUDIO);
	
	av_register_all();
	avformat_network_init(); // <-- avoid warning, I don't know what this for?

	/* Make sure quit sdl and close audio when exit */
	atexit(SDL_Quit);
	atexit(SDL_CloseAudio);
}

bool player_audio_open_file(char *s_file_path)
{
	gp_avformat = NULL;

	if(NULL == s_file_path)
	{
		DBG_ERR("File path is not valid\n");
		return FALSE;
	}
	
	if(0 != avformat_open_input(&gp_avformat, s_file_path, NULL, NULL))
	{
		DBG_PRINT("AVFormat can't open file\n");
		return FALSE;
	}
	return TRUE;
}

bool player_audio_get_codec(void)
{
	int16  idx_stream;
	uint16 i;

	if(0 > avformat_find_stream_info(gp_avformat, NULL))
	{
		DBG_PRINT("AVFormat Can't find stream information\n");
		return FALSE;
	}

	idx_stream = -1;
	for(i = 0; i < gp_avformat->nb_streams; i++)
	{
		if(AVMEDIA_TYPE_AUDIO == gp_avformat->streams[i]->codec->codec_type)
		{
			idx_stream = i;
			break;
		}
	}

	if(-1 == idx_stream)
	{
		DBG_ERR("Can't find audio stream\n");
		return FALSE;
	}

	/* Get codec from stream */
	gp_avcodec = gp_avformat->streams[idx_stream]->codec;
	gp_codec   = avcodec_find_decoder(gp_avcodec->codec_id);
	if(NULL == gp_codec)
	{
		DBG_PRINT("Can't find valid codec\n");
		return FALSE;
	}	       

	/* Open codec */
	if(0 > avcodec_open2(gp_avcodec, gp_codec, NULL))
	{
		DBG_PRINT("Can't open codec\n");
		return FALSE;
	}

	return TRUE;
}



void player_audio_callback(void *userdata, unsigned char *stream, int len)
{
	AVFrame frame;
	AVPacket packet;
	
	int16 get_frame = 0;

	while (1)
	{	
		if(0 > av_read_frame(gp_avformat, &packet))
		{
			g_isdecoding = 0;
			DBG_PRINT("Audio decoding finishs");
			return;
		}    

		if(0 > avcodec_decode_audio4(gp_avcodec, &frame, &get_frame, &packet))
		{
			DBG_PRINT("Audio Decoding error\n");
			return;
		}
	   
		if (get_frame)
		{
			int data_size;

			data_size = av_samples_get_buffer_size(NULL, gp_avcodec->channels,\
				       	frame.nb_samples, gp_avcodec->sample_fmt, 1);
			
			memcpy(&ga_audio_buff[g_audio_buff_idx], (uint8 *) frame.data[0], data_size);
			g_audio_buff_idx += data_size;	
		}

		if (g_audio_buff_idx > len)
		{
			memcpy(stream, (uint8 *) &ga_audio_buff[0], len);
			memmove(&ga_audio_buff[0], &ga_audio_buff[len], g_audio_buff_idx - len);
			g_audio_buff_idx = g_audio_buff_idx - len;
			break;
		}
	}
}

bool player_audio_play(void)
{
	SDL_AudioSpec	desired;
	SDL_AudioSpec	obtained;

	if (NULL == gp_avcodec)
	{
		DBG_PRINT("Audio codec context is NULL\n");
		return FALSE;
	}

	desired.freq = gp_avcodec->sample_rate;
	desired.format = AUDIO_S16SYS;
	desired.channels = gp_avcodec->channels;
	desired.samples = SDL_AUDIO_BUFFER_SIZE;
	desired.callback = player_audio_callback;
	desired.userdata = NULL; /* Data is come from global variable */

	if(0 > SDL_OpenAudio(&desired, &obtained))
	{
		DBG_PRINT("Fail to open SDL Audio\n");
		return FALSE;
	}

	SDL_PauseAudio(0);
	g_isdecoding = 1;
	while (1 == g_isdecoding)
	{
		SDL_Delay(100);
	}

	SDL_CloseAudio();
	return TRUE;
}

void player_audio_quit(void)
{
	/* Shutdown all SDL subsystem */
	SDL_Quit();
}
	
