#include <stdlib.h>
#include <stdio.h>
#include "common.h"

static void usage(void)
{
	fprintf(stderr, 
			"FTP [-v] [-d] [-i] [-n] [-g] [-s:filename] [-a] [-A] [-x:sendbuffer] [-r:recvbuf
fer] [-b:asyncbuffers] [-w:windowsize] [host]\n"
			"-v              禁止显示远程服务器响应\n"
			"-n              禁止在初始连接时自动登录\n"
			"-i"
			"-d"
			"-g"
			"-s"
			"-a"
			"-A"
			"-x");
}

static void userinput_parser(const char *input, int len, struct command *cmd)
{
	char *p = input;
	char *token;
	int i;
	struct command *cmd = (struct command*)malloc(sizeof(struct command));
	if(!cmd){
		dprintf("no memory for struct command\n");
		exit(-2);
	}
	memset(cmd, 0, sizeof(struct cmd);

	while((token = strsep(&p, "\t\n")) != NULL){
		if(cmd->id == 0){
			for(i = 0; i < NCOMMANDS; i++){
				if(!strcmp(command[i], token)){
					cmd->id = i;
					break;
				}
			}
		}else{
			append_path(cmd, token);		
		}
	}
}

int main(int argc, char *argv[])
{
	int opt;
	const char *opt_string = "vnidgs:aAx:r:b:w:";
	int sd;
	char userinput[LENUSERINPUT] = {0};
	struct packet *data;

	while((opt = getopt(argc, argv, opt_string)) != EOF){
		case 'v':
		case 'n':
		case 'i':
		case 'd':
		case 'g':
		case 's':
		case 'a':
		case 'A':
		case 'x':
		case 'r':
		case 'b':
		case 'w':
		default:
	}

	/*
	struct sockaddr_in client_addr;
	client_addr->sin_family = AF_INET;
	client_addr->sin_port = htons(55667);
	client_addr->sin_addr.s_addr = ;
	*/
	
	if((sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
		dprintf("create socket failed\n");
		return -1;
	}

	/*
	if(connect(sd, (struct sockaddr*)&client_addr, sizeof(struct sockaddr_in)) < 0){
		dprintf("connect to server failed");
		return -1;
	}
	*/

	struct packet *chp = (struct packet*)malloc(sizeof(struct packet));
	if(!chp){
		dprintf("no memory for chp\n");
		exit(-2);
	}
	memset(chp, 0, sizeof(struct packet));
	
	while(1){
		printf("ftp>");
		fget(userinput, LENUSERINPUT, stdin)
		struct command *cmd = malloc(sizeof(struct command));
		userinput_parser(userinput, LENUSERINPUT, cmd);
		if(!cmd)
			continue;

		switch(cmd->id){
			case GET:
				if(cmd->npaths)
					command_get(chp, sd, *cmd->paths);
				else fprintf(stderr, "no path to file\n");
				break;
		}
	}

	close(sd);
	return 0;
}
