#ifndef _COMMON_H
#define _COMMON_H
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdint.h>
#include <getopt.h>
#include <netinet/in.h>
#include <string.h>

#define __DEBUG         1

#if __DEBUG
#define dprintf(fmt, args...) \
        do{ \
                printf("FILE %s, LINE %d, "fmt, __FILE__,__LINE__, ## args);    \
         }while(0)
#else
#define dprintf(fmt, arg...)
#endif

static int setnonblock(int fd)
{
	int flags = fcntl(fd, F_GETFL);
	if(flags < 0)
		return flags;
	flags |= O_NONBLOCK;
	if(fcntl(fd, F_SETFL, flags) < 0)
		return -1;

	return 0;
}

#define NCOMMANDS 20

enum COMMAND{
		OPEN,
		GET,
		PUT,
		MGET,
		MPUT,
		CD,
		LCD,
		MGETWILD,
		MPUTWILD,
		DIR_,		// _ to avoid conflict with directory pointer DIR
		LDIR,
		LS,
		LLS,
		MKDIR,
		LMKDIR,
		RGET,
		RPUT,
		PWD,
		LPWD,
		EXIT
};

#define	LENBUFFER	504		// so as to make the whole packet well-rounded ( = 512 bytes)

struct packet{
	short int conid;
	short int type;
	short int comid;
	short int datalen;
	char buffer[LENBUFFER];
};

enum message_type{
	REQUEST,
	DONE,
	INFOMATION,
	TERMINAL, 
	DATA,
	EOT
};

static void ntoh_packet(struct packet *hp)
{
	hp->comid = ntohs(hp->comid);
	hp->conid = ntohs(hp->conid);
	hp->datalen = ntohs(hp->datalen);
	hp->type = ntohs(hp->type);
}

static void hton_packet(struct packet *hp)
{
	hp->comid = htons(hp->comid);
	hp->conid = htons(hp->conid);
	hp->datalen = htons(hp->datalen);
	hp->type = htons(hp->type);
}

#endif
