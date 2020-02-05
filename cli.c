#include"header.h"

void main(int argc,char **argv)
{
	sno=qwe=0;	
//char ab[6][25];
	/***********     socket    **********/
	if(argc!=6)
	{
	        printf("./a.out Port.no I.P Addr  WRQ/RRQ   File_name  Mode\n");
		return;
	}

	for(i=0;i<argc;i++)
		strcpy(ab[i],argv[i]);
	
	sfd = socket(AF_INET,SOCK_DGRAM,0);

	if(sfd < 0)
	{
		perror("socket");
		return;	
	}
	perror("socket");

	/************  sendto   ***********/

	v.sin_family = AF_INET;
	v.sin_port = htons( atoi( argv[1]));			//Port.no
	v.sin_addr.s_addr = inet_addr(argv[2]);			//I.P addr
	
	/*****************************************/

	len = sizeof(v);

	client_lo();
	
	if(trans == 1)
	{
		printf("\n\tRequested RRQ %s File\t Fil_name = %s\n",a.RRQ.mode,a.RRQ.fil_name);
		readfil();
	}
	else
	{
		printf("\n\tRequested WRQ %s File\t Fil_name = %s\n",a.WRQ.mode,a.WRQ.fil_name);
		writefil();
	}
}
