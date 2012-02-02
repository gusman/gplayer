#include <SDL.h>
#include "common/debug.h"
#include "common/player_type.h"
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"

#include "player_audio_pcm.h"

#define SDL_AUDIO_BUFFER_SIZE 1024

AVFormatContext	*gp_avformat	= NULL;
AVCodecContext	*gp_avcodec	= NULL;
AVCodec		*gp_codec	= NULL;
AVFrame		*gp_decoded_frame = NULL;
AVPacket	 g_avpacket;
unsigned char	 g_datalen;
unsigned char	 g_isdecoding;

void player_audio_init(void)
{
	printf("[%s]\n", __FUNCTION__);
	SDL_Init(SDL_INIT_AUDIO);
	av_register_all();
	avformat_network_init(); // <-- avoid warning, I don't know what this for?
}

bool player_audio_open_file(char *s_file_path)
{
	gp_avformat = NULL;

	if(NULL == s_file_path)
	{
		printf("ERR: [%s] File path is not valid\n", __FUNCTION__);
		return FALSE;
	}
	
	if(0 != avformat_open_input(&gp_avformat, s_file_path, NULL, NULL))
	{
		printf("ERR: [%s] Can't open file\n", __FUNCTION__);
		return FALSE;
	}
	return TRUE;
}

bool player_audio_get_codec(void)
{
	int idx_stream;
	int i;

	if(0 > avformat_find_stream_info(gp_avformat, NULL))
	{
		printf("ERR: [%s] Can't find stream information\n", __FUNCTION__);
		return FALSE;
	}

	idx_stream = -1;
	for(i = 0; i < gp_avformat->nb_streams; i++)
	{
		if(AVMEDIA_TYPE_AUDIO == gp_avformat->streams[i]->codec->codec_type)
		{
			idx_stream = i;
			printf("Audio streams is founded\n");
			break;
		}
	}

	if(-1 == idx_stream)
	{
		printf("ERR: [%s] Can't find audio stream\n", __FUNCTION__);
		return FALSE;
	}

	/* Get codec from stream */
	gp_avcodec = gp_avformat->streams[idx_stream]->codec;
	gp_codec   = avcodec_find_decoder(gp_avcodec->codec_id);
	if(NULL == gp_codec)
	{
		printf("ERR: [%s] Can't find valid codec\n", __FUNCTION__);
		return FALSE;
	}	       

	/* Open codec */
	if(0 > avcodec_open2(gp_avcodec, gp_codec, NULL))
	{
		printf("ERR: [%s] Can't open codec\n", __FUNCTION__);
		return FALSE;
	}

	printf(">> CODEC ID: %x\n", gp_codec->id);

	return TRUE;
}

unsigned long data_len = 0; 
AVFrame frame;
AVPacket packet;
unsigned char audio_buff[44112 * 2] = {0, };
unsigned int  audio_buff_idx = 0;

void player_audio_callback(void *userdata, unsigned char *stream, int len)
{
#if 0
	printf("data_len: %d, len: %d\n", data_len, len);
	memcpy(stream, (unsigned char*) g_TEST_MODE_SINE5SEC_NORMAL + data_len, len);
	data_len += len;
	if (data_len > SINE_5SEC_NORMAL_SIZE)
		data_len = 0;
#endif
#if 1
	int buffer_size = len;
	int8_t buffer[len];
	int get_frame = 0;

	while (1)
	{	
		if(0 > av_read_frame(gp_avformat, &packet))
		{
			g_isdecoding = 0;
			printf("Finish decoding\n");
			return;
		}    

		if(0 > avcodec_decode_audio4(gp_avcodec, &frame, &get_frame, &packet))
		{
			printf("Decoding error\n");
			return;
		}
	   
		if (get_frame)
		{
			int data_size;

			data_size = av_samples_get_buffer_size(NULL, gp_avcodec->channels, frame.nb_samples, gp_avcodec->sample_fmt, 1);
			
			memcpy(&audio_buff[audio_buff_idx], (unsigned char *) frame.data[0], data_size);
			audio_buff_idx += data_size;	
		//	printf("Data size: %d\n", audio_buff_idx);
		}

		if (audio_buff_idx > len)
		{
			memcpy(stream, (unsigned char *) &audio_buff[0], len);
			memmove(&audio_buff[0], &audio_buff[len], audio_buff_idx - len);
			audio_buff_idx = audio_buff_idx - len;
			break;
		}
	}
#endif
}

bool player_audio_play(void)
{
	SDL_AudioSpec	desired;
	SDL_AudioSpec	obtained;

	if (NULL == gp_avcodec)
	{
		printf("ERR: [%s] gp_avcodec is NULL \n", __FUNCTION__);
		return FALSE;
	}

	desired.freq = gp_avcodec->sample_rate;
	desired.format = AUDIO_S16SYS;
	desired.channels = gp_avcodec->channels;
	desired.silence = 1;
	desired.samples = SDL_AUDIO_BUFFER_SIZE;
	desired.callback = player_audio_callback;
	desired.userdata = g_TEST_MODE_SINE5SEC_NORMAL;

	printf("Open audio\n");
	if(0 > SDL_OpenAudio(&desired, &obtained))
	{
		printf("ERR, Couldn't open audio: %s\n", SDL_GetError());
		return FALSE;
	}

	printf("Information:\n");
	printf("obtained.freq: %d\n", obtained.freq);
	printf("obtained.channels: %d\n", obtained.channels);
	printf("obtained.samples: %d\n", obtained.samples);

	printf("Audio playback starts\n");
	SDL_PauseAudio(0);

	g_datalen = 0xFF;
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
	printf("[%s]\n", __FUNCTION__);
}
	
