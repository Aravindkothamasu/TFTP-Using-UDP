#include"header.h"

void main(int argc,char **argv)
{
sno=0;	
	/*************   socket   ***********/
	if(argc!=2)
	{
		printf("./a.out Port no.\n");
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

	printf("sfd = %d\n",sfd);


	/*********   bind   *********/

	v.sin_family = AF_INET;
	v.sin_port  = htons(4567);
	v.sin_addr.s_addr = inet_addr("0.0.0.0");

	len = sizeof(v);

	r = bind(sfd,(struct sockaddr *)&v,len);
	if(r < 0)
	{
		perror("bind");
		return;
	}
	perror("bind");

	/*************************************/

	recvfrom(sfd,&a,sizeof(a),0,(struct sockaddr *)&v,&len);		//Recieving From client 1st
	printf("First time recieved\n");
	len = sizeof(v);
	qwe= open("log_s",O_CREAT|O_APPEND|O_WRONLY|O_TRUNC,0664);
	if(qwe < 0)
	{
		perror("open");
		return;
	}
	write(qwe,"\t _______________________________________________________________________ \n",75);
	write(qwe,"\t| S.no  Block     I.P Addr            Port.no                      Time |\n",75);
	write(qwe,"\t|-----------------------------------------------------------------------|\n",75);

	
	if(a.RRQ.opcode == 1)
	{
		printf("Requested RRQ %s\tFile\tFil_name = %s\n",a.RRQ.mode,a.RRQ.fil_name);
		writefil();
	}
	else	
	{
		printf("Requested WRQ %s\tFile\tFil_name = %s\n",a.WRQ.mode,a.WRQ.fil_name);
		readfil();
	}
	
}
