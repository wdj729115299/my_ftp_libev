#ifndef _SERVER_FTP_H
#define _SERVER_FTP_H

#include <ev.h>
#include "common.h"

struct ftp_client{
	int sd;
	ev_io ev_accept;
	ev_io ev_read;
	ev_io ev_write;
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
