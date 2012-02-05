#ifndef __PLAYER_AUDIO_H__
#define __PLAYER_AUDIO_H__

#include "common/player_type.h"

void player_audio_init(void);
void player_audio_quit(void);
bool player_audio_open_file(char *s_file_path);
bool player_audio_get_codec(void);
bool player_audio_play(void);
#endif /* __PLAYER_AUDIO_H__ */
