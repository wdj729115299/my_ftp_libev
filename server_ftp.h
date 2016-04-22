#ifndef _SERVER_FTP_H
#define _SERVER_FTP_H

#include <ev.h>

struct ftp_client{
	int sd;
	ev_io accept;
	ev_io read;
	ev_io write;
	void *data;
	int connection_id;
};

struct ftp_server{
	int listen_sd;
	struct ev_loop *loop;
	struct sockaddr_in listen_addr;
	struct ev_io *ev_accept;
	void *data;
};

#endif
