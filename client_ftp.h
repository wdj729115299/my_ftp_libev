#ifndef _CLIENT_FTP_H
#define _CLIENT_FTP_H

struct command{
	short int id;
	int npaths;
	char** paths;
};

#define LENUSERINPUT 1024

#endif
