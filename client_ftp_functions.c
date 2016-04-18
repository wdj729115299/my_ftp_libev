#include "client_ftp.h"
#include "common.h"

static const char* command[NCOMMANDS] = {
	"open",
	"get",
	"put",
	"mget",
	"mput",
		
	"cd",
	"lcd",
		
	"mgetwild",
	"mputwild",
	
	"dir",
	"ldir",

	"ls",
	"lls",
		
	"mkdir",
	"lmkdir",

	"rget",
	"rput",
		
	"pwd",
	"lpwd",
		
	"exit"
};
