/*
*/

#include "Define.h"
#include "EbenezerDlg.h"

//-----------------------------------------------------------------------------
int SDL_main(int argc, char** argv) {

	CEbenezerDlg server;

	if(!server.OnInitDialog()) {
		system("pause");
		return -1;
	}

	system("pause"); // NOTE: keeps thread running

	return 0;
}
