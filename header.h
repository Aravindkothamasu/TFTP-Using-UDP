#include<stdio.h>
#include<arpa/inet.h>
#include<sys/time.h>
#include<time.h>
#include<stdio.h>
#include<arpa/inet.h>
#include<errno.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<strings.h>
#include<sys/ipc.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<fcntl.h>
#include<dirent.h>
#include<netinet/in.h>
#include<sys/un.h>
#include<signal.h>

/*
#define SUCC 0				//Sucess ga open chestea
#define EPERM 1				//Operation Not permitted
#define ENOENT 2			//No Such File or Directory 	*
#define ESRCH 3				//No Such Process
#define EINTR 4				//Interrupted System Cal
#define EIO 5				//I/O Error 
#define EBADF 9				//Bad file descriptor (Opening with one option and doing some other operation 
#define ENOMEM 12			//Out of Memory 		* 
#define EACCES 13			//permission Denied 		* 
#define EEXIST 17			//File already exist
#define EROFS 30			//Read Only File system 	* 
#define ENAMETOOLONG 36			//File name is too long 
*/


void client_lo(void);
void readfil(void);
void writefil(void);
void error_fr(int);
void open_file(int);
void read_error(void);
void ti_recv(void);

struct sockaddr_in v,f; 		// f = Sender  v = Reciever:w
int r,len,sfd,fd,op,trans,i,j,qwe,sno,l;
char ab[6][25],s[100];
time_t t1;
struct timeval t;
fd_set b;


union tftp
{
	struct rrq //1
	{ 
		short int opcode;
		char fil_name[256];	
		char mode[10];
	}RRQ; 

	struct wrq //2
	{
		short int opcode;
		char fil_name[256];
		char mode[10];
	}WRQ; 

	struct data //3
	{
		short int opcode;
		short int block;
		char data[512];
	}DATA; 

	struct ack //4
	{
		short int opcode;
		short int block;
	}ACK; 

	struct error //5
	{
		short int opcode;
		short int error;
		char variable[512];
	}ERROR;	
}a;
