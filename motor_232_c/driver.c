#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>

#define DEVICE "/dev/ttyUSB0"

int Serialfd;
char buffer[100]="DI\r";

void serial_init(void)
{
	struct termios options;
   	Serialfd = open(DEVICE, O_RDWR | O_NOCTTY & ~O_NDELAY);
   	if (Serialfd < 0)	perror("UART: Failed to open the file. \n"); 

   	tcgetattr(Serialfd, &options);
	tcflush(Serialfd, TCIOFLUSH);	
   	cfsetispeed(&options, B115200);	

   	options.c_cflag |= CS8;	
   	options.c_cflag |= (CLOCAL | CREAD);
   	options.c_cflag &= ~CSIZE;
   	options.c_cflag &= ~CRTSCTS;			
   	options.c_cflag &= ~CSTOPB;
   	options.c_iflag |= IGNPAR;

	options.c_oflag &= ~OPOST; 		
   	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); 
   	options.c_iflag =(IGNBRK|IGNCR);

	options.c_cc[VMIN]=0;
	options.c_cc[VTIME]=1;

	options.c_iflag &= ~(ICRNL | IGNCR);

    	tcsetattr(Serialfd, TCSANOW, &options);
	
}

void motor_en(void)
{
	int nByte;
	char buff[]="EN\r";
	nByte = write(Serialfd,buff,sizeof(buff));
	
	printf("Send:%d\n",nByte);
}

int motor_set_vel(long vel)
{
	char buff[100];
	int len,nByte;
	if(vel>30000||vel<-30000)
	{
		printf("Set Motor Velocity Error: out of range");
		return -1;
	}
	sprintf(buff,"V%ld\r",vel);
	len = strlen(buff);
	nByte = write(Serialfd,buff,len);
	printf("Send:%d\n",nByte);
	return 0;
}
int motor_get_vc(void)
{
	char trace=201;
	char buff[100];
	int fa;
	struct timeval t1,t2;
	write(Serialfd,&trace,1);
	gettimeofday(&t1,NULL);
	fa=read(Serialfd,&buff,5);
	gettimeofday(&t2,NULL);
	printf("Time:%d\n",(t2.tv_sec-t1.tv_sec)*1000000+t2.tv_usec-t1.tv_usec);
	printf("len:%d\n",fa);
	printf("receive:%X %X %X %X %X\n",buff[0],buff[1],buff[2],buff[3],buff[4]);
	return 0;
}
int main(void)
{
	serial_init();
	motor_en();	
	while(1)
	{
		motor_get_vc();
		motor_set_vel(100);
	}
}
