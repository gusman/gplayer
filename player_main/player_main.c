#include "common/debug.h"
#include "common/player_type.h"
#include "player_audio/player_audio.h"

int main(int argc, char *argv[])
{
	if (argc < 1)
	{
		printf("Usage %s audio_file\n", argv[0]);
	}

	player_audio_init();
	player_audio_quit();
	return 0;
}
