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

static void append_path(struct command *cmd, char *token)
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

void command_get(struct packet *chp, int sd, char *filename)
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
	data = htonp(chp);

	if((ret = send(sd, data, sizeof(struct packet), 0)) != sizeof(struct packet)){
		dprintf("send packet failed\n");
		return;
	}
	if((ret = recv(sd, data, sizeof(struct packet), 0)) <= 0){
		dprintf("recv packet failed\n");
		return;
	}
	chp = ntohs(data);
	if(chp->type == INFOMATION && chp->comid == GET && strlen(chp->buffer) ){
		printf("\t%s\n", chp->buffer);
		recv_file(chp, sd, fp);
	}else{
		dprintf("Error getting remote file : <%s>\n", filename);
	}

	fclose(fp);
}
