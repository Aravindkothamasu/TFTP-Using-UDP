#include"header.h"
void readfil(void)
{
	i=1;
	open_file(4);									//Write Only
	printf("\nIn  Read file...\n");

	recvfrom(sfd,&a,sizeof(a),0,(struct sockaddr *)&v,&len);
	if(a.ACK.opcode == 4)
		printf("ACK Recieved\n");
	sleep(3);

	while(recvfrom(sfd,&a,sizeof(a),0,(struct sockaddr *)&v,&len)!=-1)			//reading from socket breaks recvform^()
	{
		if(a.DATA.opcode == 5)							//Error opcode 
			read_error();
		else if(a.DATA.block == 0)						//End of block
		{
			printf("\nTansmission Completed\n");
			exit(0);
		}
		else									//If it is sucessful
		{	
			if(i == a.DATA.block)
			{
				printf("%d Block Recieved\t",a.DATA.block);
				write(fd,a.DATA.data,strlen(a.DATA.data));			//Writing data into file
				t1 = time(0);	
				if(sno <9)						//Printing into log file
		sprintf(s,"\t  %d     %d        %s            %d     %s",++sno,i,inet_ntoa(v.sin_addr),ntohs(v.sin_port),ctime(&t1));
				else 
		sprintf(s,"         %d    %d        %s            %d     %s",++sno,i,inet_ntoa(v.sin_addr),ntohs(v.sin_port),ctime(&t1));

				if(qwe!=0 && qwe!=1)
					write(qwe,s,strlen(s));
				if(errno == 9)
					error_fr(4);					//Bad File Des(can't able to write  4

				bzero(&a,sizeof(a));

				a.ACK.block = i++;
				a.ACK.opcode = 4;

				ti_recv();					//Sending ACK using Time func^()
//				sendto(sfd,&a,sizeof(a),0,(struct sockaddr *)&v,len);		//Sending acknowledgement
				printf("Sending ACK\n");
				sleep(1);		
			}
			else if(i-1 == a.DATA.block);	
		}
	}				
}

void writefil(void)
{
	printf("\nIn write file\n");
	open_file(2);							//Read Only
	a.ACK.opcode = 4;
	a.ACK.block = 0;
	j=1;
	sendto(sfd,&a,sizeof(a),0,(struct sockaddr *)&v,len);
	char str[511];
	while(1)
	{
		r = read(fd,str,511);
		bzero(&a,sizeof(a));

		if(r == 0)
		{
			printf("Reading Done...\n");
			a.DATA.block = 0;
			a.DATA.opcode = 3;
			sendto(sfd,&a,sizeof(a),0,(struct sockaddr *)&v,len);
			exit(0);
		}
abcd:
		a.DATA.opcode = 3;
		a.DATA.block = j++;
		strcpy(a.DATA.data,str);

		sendto(sfd,&a,sizeof(a),0,(struct sockaddr *)&v,len);				//Sending Data Frame
		printf("%d Block Sent\n",a.DATA.block);
		t1 = time(0);									//Printing into log file
		if(sno < 9)
	sprintf(s,"\t  %d     %d      %s            %d     %s",++sno,j,inet_ntoa(v.sin_addr),ntohs(v.sin_port),ctime(&t1));
		else 
	sprintf(s,"         %d    %d        %s            %d     %s",++sno,i,inet_ntoa(v.sin_addr),ntohs(v.sin_port),ctime(&t1));
		if(qwe!=0 && qwe!=1)
			write(qwe,s,strlen(s));


		bzero(&a,sizeof(a));
		printf("Waiting For Ack..\n");

		t.tv_sec = 10;
		t.tv_usec = 0;
		FD_ZERO(&b);
		FD_SET(sfd,&b);
		l = select(sfd+1,&b,0,0,&t);
		if(l < 0)
		{
			perror("select");
			exit(0);
		}
		else if(l == 0)
		{
			printf("Time Out for Acknowledgement , Data Frame Re-sending\n"); 
			j--; 
			goto abcd; 
		} 
		else 
			recvfrom(sfd,&a,sizeof(a),0,(struct sockaddr *)&v,&len);			//Waiting For acknowledgment 

			bzero(s,sizeof(s)); 

			if(a.ACK.block == j-1)							//if ack is successful 
				bzero(str,sizeof(str)); 
			else if(a.ACK.opcode == 5)							//If an Error to Re-send 
			{	
				if(a.ERROR.error != 8) 
					read_error(); 
				else 
				{ 
					j--; 
					bzero(&a,sizeof(a)); 
					goto abcd; 
				} 
			} 
	}  
}


void open_file(int o)
{
	if(o == 4)
		fd = open(a.RRQ.fil_name,O_WRONLY|O_CREAT|O_APPEND|O_TRUNC,0664);
	else if(o == 2)
		fd = open(a.RRQ.fil_name,O_RDONLY);

	if(fd < 0)
	{
		if(errno == 2)
			error_fr(1);				//File Lekapothey     1
		else if(errno == 13)	  
			error_fr(2);				//No Read Permission  2
		else if(errno == 12)
			error_fr(3);				//Disk Full           3
		else if(errno == 17)
			error_fr(6);				//File already Exists 6
	}	
	printf("%s %d\n",strerror(errno),errno);
}	

void read_error()
{
	printf("%s\n",a.ERROR.variable);
	exit(0);
}

void error_fr(int err)
{
	a.ERROR.opcode = 5;
	a.ERROR.error = err;
	if(err == 0)
		strcpy(a.ERROR.variable,"Wrong Input");		//No Netascii (or) Octet
	else if(err == 1)
		strcpy(a.ERROR.variable,"File Not Found");			//File Leka pothey
	else if(err == 2)
		strcpy(a.ERROR.variable,"Acess violation");			//Read or Write Permission is missing
	else if(err == 3)
		strcpy(a.ERROR.variable,"Disk full");				//.......
	else if(err == 4)
		strcpy(a.ERROR.variable,"Illegal Operation");			//Only Read permission vuntea
	else if(err == 5)
		strcpy(a.ERROR.variable,"Unkown Port.no");			//Port.no Wrong 
	else if(err == 6)
		strcpy(a.ERROR.variable,"File already exist");			//File is already Present aithey
	else if(err == 7)
		strcpy(a.ERROR.variable,"No Such User");			//.......
	else if(err == 8)
		printf("Packet damaged Re-sending..\n");


	printf("Sending Error Frame..\n");
	sendto(sfd,&a,sizeof(a)+1,0,(struct sockaddr *)&v,len);
	sleep(2);
	printf(" Sent..\n");	
	return;
}


void client_lo(void)
{
	printf("in clinet lo\n");	
	if(strcmp(ab[3],"RRQ")==0)
	{	
		a.RRQ.opcode = 1;
		trans =1;
		strcpy(a.RRQ.fil_name,ab[4]);
	}
	else if(strcmp(ab[3],"WRQ")==0)
	{
		a.WRQ.opcode = 2;
		trans =2;
		strcpy(a.WRQ.fil_name,ab[4]);
	}	
	else
	{
		printf("Wrong Input\n");
		exit(0);
	}

(trans == 1) ? (strcmp(ab[5],"netascii")==0 ? (strcpy(a.RRQ.mode,"netascii") ): (strcmp(ab[5],"octet")==0) ? strcpy(a.RRQ.mode,"octet"):error_fr(0)) : 0;   
(trans == 2) ? (strcmp(ab[5],"netascii")==0 ? (strcpy(a.WRQ.mode,"netascii") ): (strcmp(ab[5],"octet")==0) ? strcpy(a.WRQ.mode,"octet"):error_fr(0)) : 0;
	printf("\n\nRequirment is sending to server...\n"); 
	sendto(sfd,&a,sizeof(a)+1,0,(struct sockaddr *)&v,len); 
	sleep(2);
} 

void ti_recv(void)
{
	t.tv_sec = 10;
	t.tv_usec = 0;
	FD_ZERO(&b);
	FD_SET(sfd,&b);
	l = select(sfd+1,&b,0,0,&t);
	if(l < 0)
	{
		perror("select");
		exit(0);
	}
	else if(l == 0)
	{
		printf("Time Out...\n");
		exit(0);
	}
	else
		sendto(sfd,&a,sizeof(a),0,(struct sockaddr *)&v,len);
}
