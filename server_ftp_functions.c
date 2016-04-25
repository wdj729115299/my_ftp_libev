#include "common.h"

void server_send_file(struct packet *hp, int sd, FILE *fp)
{
	int i = 0, j = 0;
	struct packet *data;
	
	while(!feof(fp)){
		memset(hp->buffer, '\0', sizeof(char) * LENBUFFER);
		hp->datalen = fread(hp->buffer, 1, LENBUFFER - 1, fp);
		i += hp->datalen;
		data = hp;
		hton_packet(data);
		if(send(sd, data, sizeof(struct packet), 0) < 0){
			dprintf("send packet failed\n");
			return;
		}
		j++;
	}
	fprintf(stdin, "\t%d byte(s) read.\n", i);
	fprintf(stdin, "\t%d data packet(s) sent.\n", j);
	fflush(stdin);
}

void server_send_EOT(struct packet *hp, int sd)
{
	int i;
	struct packet *data;
	
	hp->type = EOT;
	data = hp;
	hton_packet(data);
	if(send(sd, hp, sizeof(struct packet), 0) < 0){
		dprintf("send packet failed\n");
		return;
	}
}

void server_command_get(struct packet *hp, int sd)
{
	int n;
	struct packet *data;
	
	FILE *fp = fopen(hp->buffer, "rb");
	if(!fp){
		dprintf("fopen file failed\n");
		return;
	}

	hp->type = INFOMATION;
	hp->comid = GET;
	strcpy(hp->buffer, "file found.processing");
	data = hp;
	hton_packet(hp);

	if(send(sd, data, sizeof(struct packet), 0) < 0){
		dprintf("send packet failed\n");
		return;
	}

	hp->type = DATA;
	server_send_file(hp, sd, fp);
	fclose(fp);

	server_send_EOT(hp, sd);
}
