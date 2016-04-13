#include <stdlib.h>
#include <stdio.h>
#include "server_ftp.h"
#include "common.h"

static struct ftp_server server;

static void write_cb(struct ev_loop *loop, struct ev_io *w, int revents)
{
	
}

static void read_cb(struct ev_loop *loop, struct ev_io *w, int revents)
{
	struct ftp_client *ftp_client = (struct client*)((char*)w - offset(struct client, ev_read));
	
}

static void accept_cb(struct ev_loop *loop, struct ev_io *w, int revents)
{
	struct ftp_client *accept_loop = (struct ftp_client*)((char*)w - offset(struct client, ev_accept))

	struct sockaddr_in client_addr;
	sockaddr_t client_len = sizeof(struct sockaddr_in);
	int client_sd = accept(w->listen_sd, (struct sockaddr*)&client_addr, &client_len);
	struct ftp_client *client = malloc(sizeof(struct ftp_client));
	if(!client){
		dprintf("no memory for client.\n");
		exit(-1);
	}

	client->sd = client_sd;
	ev_io_init(&client->ev_read, read_cb, client->sd, EV_READ);
	ev_io_start(loop, &client->ev_read);
}

static void ftp_server_loop(struct ftp_server *server)
{
	server->loop = ev_default_loop(0);
	struct client *accept_loop = malloc(sizeof(struct client));
	if(!accept_loop){
		dprintf("no memory for accept_loop.\n");
		exit(-1);
	}

	ev_io_init(&accept_loop->ev_accept, accept_cb, server->listen_sd, EV_READ);
	ev_io_start(server->loop, accept_loop->ev_accept);

	ev_loop(server->loop, 0);
}

static void init_server(void)
{
	server.listen_addr.sin_family = AF_INET;
	server.listen_addr.sin_port = htons(55667);
	server.listen_addr.sin_addr.s_addr = INADDR_ANY;

	server.listen_sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(server.listen_sd < 0){
		dprintf("create socket failed\n");
		return -1;
	}

	int reuseaddr_on = 1;
	if(setsockopt(server.listen_sd, 
				SOL_SOCKET,
				SO_REUSEADDR,
				&reuseaddr_on,
				sizeof(reuseaddr_on)) < 0){
		dprintf("setsockopt failed.\n");
		return -1;
	}

	if(bind(server.listen_sd, &server.listen_addr, sizeof(server.listen_addr)) < 0){
		dprintf("bind socket failed.\n");
		return -1;
	}

	if(listen(server.listen_sd, 5) < 0){
		dprintf("listen on socket failed.\n");
		return -1;
	}

	if(setnonblock(server.listen_fd) < 0){
		dprintf("setnonblock failed.\n");
		return -1;
	}

}

int main(void){

	init_server();
	
	return ftp_server_loop(&server);
}