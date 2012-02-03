#include "common/debug.h"
#include "common/player_type.h"
#include "player_audio/player_audio.h"

int main(int argc, char *argv[])
{
	char *def_path = "testfiles/01.flac";
	char *file_path = NULL;

	file_path = def_path;

	player_audio_init();
	player_audio_open_file(file_path);
	player_audio_get_codec();
	player_audio_play();
	player_audio_quit();
	return 0;
}
