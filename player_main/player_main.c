#include "common/debug.h"
#include "common/player_type.h"
#include "player_audio/player_audio.h"

int main(int argc, char *argv[])
{
	char *file_path = "testfiles/01.flac";

	if (argc < 1)
	{
		printf("Usage %s audio_file\n", argv[0]);
	}

	player_audio_init();
	player_audio_open_file(file_path);
	player_audio_get_codec();
	player_audio_play();
	player_audio_quit();
	return 0;
}
