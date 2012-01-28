#ifndef __PLAYER_AUDIO_H__
#define __PLAYER_AUDIO_H__

#include "common/player_type.h"

struct st_audio_properties {
	
	char *ps_filepath;
};


void player_audio_init(void);
void player_audio_quit(void);

#endif /* __PLAYER_AUDIO_H__ */
