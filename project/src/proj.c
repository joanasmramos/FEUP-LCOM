#include <minix/drivers.h>
#include <minix/driver.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include "game.h"
#include "video_gr.h"
#include "time.h"


int main(){

	srand(time(NULL));

	sef_startup();

	Game* game = (Game*) load_game_bmp();

	runGame(game);

	//defplay
	//mouseinit
	//interrupithand

	vg_exit();
	return 0;
}
