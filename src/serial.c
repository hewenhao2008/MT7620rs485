#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include "serial.h"

int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop)
{
       	struct termios  newtio,oldtio;
		if(tcgetattr(fd,&oldtio) !=0)
   		{
   		perror("Serial set error!\n");
		return -1;
   		}
		bzero(&newtio,sizeof(newtio));
	/*�����ַ���С*/
	newtio.c_cflag |= CLOCAL | CREAD;
	newtio.c_cflag &= ~CSIZE;
	/*����ֹͣλ*/
	switch(nBits)
		{
		case 7:
			newtio.c_cflag |= CS7;
			break;
		case 8:
			newtio.c_cflag |= CS8;
			break;
		}
	/*���û�żУ��λ*/
	switch(nEvent)
		{
	case 'O'://����
            newtio.c_cflag |= PARENB;
	    newtio.c_cflag |= PARODD;
            newtio.c_iflag |= (INPCK | ISTRIP);
		break;
	case 'E': //ż��
	    newtio.c_iflag |= (INPCK | ISTRIP);
            newtio.c_cflag |= PARENB;
	    newtio.c_cflag &= ~PARODD;
		break;
	case 'N':  //��У��λ
            newtio.c_cflag &= ~PARENB;
		break;
		}

	/* ���ò�����*/
	switch(nSpeed)
		{
	case 2400:
			cfsetispeed(&newtio,B2400);
			cfsetospeed(&newtio,B2400);
		break;
	case 4800:
			cfsetispeed(&newtio,B4800);
			cfsetospeed(&newtio,B4800);			
		break;
	case 9600:
			cfsetispeed(&newtio,B9600);
			cfsetospeed(&newtio,B9600);			
		break;
	case 38400:
			cfsetispeed(&newtio,B38400);
			cfsetospeed(&newtio,B38400);			
		break;
	case 57600:
			cfsetispeed(&newtio,B57600);
			cfsetospeed(&newtio,B57600);			
		break;
	case 115200:
			cfsetispeed(&newtio,B115200);
			cfsetospeed(&newtio,B115200);			
		break;
	default:
			cfsetispeed(&newtio,B9600);
			cfsetospeed(&newtio,B9600);			
		}
	
     /* ����ֹͣλ*/
	if( nStop == 1)
		newtio.c_cflag &= ~CSTOPB;
	else if(nStop == 2)
		newtio.c_cflag |= CSTOPB;
	/* ���õȴ�ʱ�����С�����ַ�*/
	newtio.c_cc[VTIME] = 0;
	newtio.c_cc[VMIN] = 0;
	/* ����δ�����ַ�*/
	tcflush(fd,TCIFLUSH);
	/*���������� */
	if((tcsetattr(fd,TCSANOW,&newtio))!=0)
		{
		perror("com set error");
		return -1;
		}
	printf("set serial done! \n");
	return 0;
}

/*
int open_port(int fd,int comport)
{
	char *dev[]={"/dev/ttyS0","/dev/ttyS1","/dev/ttyS2"};
	long  vdisable;
	if (comport==1)
	{	fd = open( "/dev/ttyS0", O_RDWR|O_NOCTTY|O_NDELAY);
		if (-1 == fd){
			perror("Can't Open Serial Port");
			return(-1);
		}
	}
	else if(comport==2)
	{	fd = open( "/dev/ttyS1", O_RDWR|O_NOCTTY|O_NDELAY);
		if (-1 == fd){
			perror("Can't Open Serial Port");
			return(-1);
		}
	}
	else if (comport==3)
	{
		fd = open( "/dev/ttyS2", O_RDWR|O_NOCTTY|O_NDELAY);
		if (-1 == fd){
			perror("Can't Open Serial Port");
			return(-1);
		}
	}
	if(fcntl(fd, F_SETFL, 0)<0)
		printf("fcntl failed!\n");
	else
		printf("fcntl=%d\n",fcntl(fd, F_SETFL,0));
	if(isatty(STDIN_FILENO)==0)
		printf("standard input is not a terminal device\n");
	else
		printf("isatty success!\n");
	printf("fd-open=%d\n",fd);
	return fd;
}
*/

int open_port(int fd,int comport)
{
	//char *dev[]={"/dev/ttyS0","/dev/ttyS1","/dev/ttyUSB0","/dev/ttyUSB1","/dev/ttyUSB2","/dev/ttyUSB3","/dev/ttyUSB4"};
	/*if (comport==0)
	{	fd = open( "/dev/ttyS1", O_RDWR|O_NOCTTY|O_NDELAY);
		if (-1 == fd){
			perror("Can't Open Serial Port\n");
			return(-1);
		}
	}
	else if(comport==1)
	{	fd = open( "/dev/ttyUSB0", O_RDWR|O_NOCTTY|O_NDELAY);
		if (-1 == fd){
			perror("Can't Open Serial Port\n");
			return(-1);
		}
	}
	*/
	fd = open( "/dev/ttyS1", O_RDWR|O_NOCTTY|O_NDELAY);
	if(-1==fd)
		{
			perror("Can't Open Serial Port\n");
			return(-1);
		}
	if(fcntl(fd,F_SETFL,0)<0)
		printf("fcntl failed!\n");
	else
		printf("fcntl+%d\n",fcntl(fd,F_SETFL,0));
		
	/*�����Ƿ�Ϊ�ն��豸 */
	if(isatty(STDIN_FILENO)==0)
		printf("standard input is not a termial device\n");
	else
	printf("isatty success!\n");
	printf("fd-open=%d\n",fd);
	return fd;
}