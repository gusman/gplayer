#include <SDL.h>
#include "common/debug.h"
#include "common/player_type.h"
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"

void player_audio_init(void)
{
	av_register_all();
}

void player_audio_quit(void)
{
	/* Shutdown all SDL subsystem */
	printf("%s, SDL Quit\n", __FUNCTION__);
	SDL_Quit();
}
	
