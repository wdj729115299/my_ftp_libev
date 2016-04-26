#include <stdlib.h>
#include <stdio.h>
#include "server_ftp.h"
#include "common.h"

static struct ftp_server server;
static int g_connection_id = 0;

static void write_cb(struct ev_loop *loop, struct ev_io *w, int revents)
{
	
}

static void read_cb(struct ev_loop *loop, struct ev_io *w, int revents)
{
	struct ftp_client *ftp_client = (struct ftp_client*)((char*)w - offsetof(struct ftp_client, ev_read));
	struct packet data;
	struct packet *hp;
	
	while(1){
		if(recv(ftp_client->sd, &data, sizeof(struct packet), 0) < 0){
			dprintf("error ocuuring,closing connections\n");
			break;
		}

		hp = &data;
		ntoh_packet(hp);

		if(hp->type == TERMINAL)
			break;

		if(hp->conid == -1)
			hp->conid = ftp_client->connection_id;

		if(hp->type == REQUEST){
			switch(hp->comid){
				case GET:
					server_command_get(&data, ftp_client->sd);
					break;
			}
		}
	}

	close(ftp_client->sd);
}

static void accept_cb(struct ev_loop *loop, struct ev_io *w, int revents)
{
	struct ftp_client *accept_loop = (struct ftp_client*)
							((char*)w - offsetof(struct ftp_client, ev_accept));

	struct sockaddr_in client_addr;
	int client_sd;
	socklen_t client_len = sizeof(struct sockaddr_in);
	
	 client_sd = accept(accept_loop->sd, (struct sockaddr*)&client_addr, &client_len);
	 if(client_sd < 0)
	 	return;
	 
	struct ftp_client *client = (struct ftp_client*)malloc(sizeof(struct ftp_client));
	if(!client){
		dprintf("no memory for client.\n");
		exit(-1);
	}
	client->connection_id = g_connection_id++;
	client->sd = client_sd;
	
	ev_io_init(&client->ev_read, read_cb, client->sd, EV_READ);
	ev_io_start(loop, &client->ev_read);
}

static void ftp_server_loop(struct ftp_server *server)
{
	server->loop = ev_default_loop(0);
	struct ftp_client *accept_loop = malloc(sizeof(struct ftp_client));
	if(!accept_loop){
		dprintf("no memory for accept_loop.\n");
		exit(-1);
	}

	ev_io_init(&accept_loop->ev_accept, accept_cb, server->listen_sd, EV_READ);
	ev_io_start(server->loop, &accept_loop->ev_accept);

	ev_loop(server->loop, 0);
}

static int init_server(void)
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

	if(bind(server.listen_sd, (struct sockaddr*)&server.listen_addr, sizeof(server.listen_addr)) < 0){
		dprintf("bind socket failed.\n");
		return -1;
	}

	if(listen(server.listen_sd, 5) < 0){
		dprintf("listen on socket failed.\n");
		return -1;
	}

	if(setnonblock(server.listen_sd) < 0){
		dprintf("setnonblock failed.\n");
		return -1;
	}

	return 0;

}

void main(void){

	if(init_server() < 0){
		return;
	}
	
	return ftp_server_loop(&server);
}
