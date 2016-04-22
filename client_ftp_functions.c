#include "client_ftp.h"
#include "common.h"

const char* command_list[NCOMMANDS] = 
{
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

void recv_file(int sd, FILE *fp)
{
	struct packet data;
	struct packet *chp;
	int i = 0, j = 0;

	if(recv(sd, &data, sizeof(struct packet), 0) <= 0){
		dprintf("error while receiving file\n");
		return;
	}
	
	j++;
	chp = &data;
	ntoh_packet(chp);
	
	while(chp->type == DATA){
		i += fwrite(chp->buffer, sizeof(char), chp->datalen, fp);
			
		if(recv(sd, &data, sizeof(struct packet), 0) <= 0){
			dprintf("error while receiving file\n");
			return;
		}
		j++;
		chp = &data;
		ntoh_packet(chp);
	}
	fprintf(stdin, "\t%d data packet(s) received.\n", --j);	// j decremented because the last packet is EOT.
	fprintf(stdin, "\t%d byte(s) written.\n", i);

	if(chp->type == EOT)
		return;
	else
	{
		fprintf(stderr, "Error occured while downloading remote file.\n");
		exit(2);
	}
	fflush(stderr);
}

void append_path(struct command *cmd, char *token)
{
	cmd->npaths++;
	char **temppaths = (char **)malloc(cmd->npaths * sizeof(char*));
	if(!temppaths){
		dprintf("no memory for temppaths\n");
		exit(-2);
	}
	memcpy(temppaths, cmd->paths, (cmd->npaths-1) * sizeof(char*));

	char *temps = (char*)malloc(strlen(token) * sizeof(char));
	if(!temps){
		dprintf("no memory for temps\n");
		exit(-2);
	}
	memcpy(temps, token, strlen(token));

	*(temppaths + cmd->npaths -1) = temps;
	cmd->paths = temppaths;
}

void client_command_get(struct packet *chp, int sd, char *filename)
{
	int ret;
	struct packet *data;
	
	FILE *fp = fopen(filename, "wb");
	if(!fp){
		dprintf("fopen file failed\n");
		exit(-2);
	}

	chp->type = REQUEST;
	chp->conid = -1;
	chp->comid = GET;
	strcpy(chp->buffer, filename);
	data = chp;
	hton_packet(data);
	if((ret = send(sd, data, sizeof(struct packet), 0)) != sizeof(struct packet)){
		dprintf("send packet failed\n");
		return;
	}
	if((ret = recv(sd, data, sizeof(struct packet), 0)) <= 0){
		dprintf("recv packet failed\n");
		return;
	}
	chp = data;
	ntoh_packet(chp);
	if(chp->type == INFOMATION && chp->comid == GET && strlen(chp->buffer) ){
		printf("\t%s\n", chp->buffer);
		recv_file(sd, fp);
	}else{
		dprintf("Error getting remote file : <%s>\n", filename);
	}

	fclose(fp);
}
