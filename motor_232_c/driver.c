#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>

#define DEVICE "/dev/ttyUSB0"

int Serialfd;
char buffer[100]="DI\r";

void serial_init(void)
{
	struct termios options;
   	Serialfd = open(DEVICE, O_RDWR | O_NOCTTY | O_NDELAY);//O_NDELAY);
   	if (Serialfd < 0)	perror("UART: Failed to open the file. \n"); 

   	tcgetattr(Serialfd, &options);
   	options.c_cflag |= (CLOCAL | CREAD);
   	options.c_cflag &= ~CSIZE;
   	options.c_cflag &= ~CRTSCTS;			
   	options.c_cflag |= CS8;	
   	options.c_cflag &= ~CSTOPB;
   	options.c_iflag |= IGNPAR;

	options.c_oflag &= ~OPOST; 		
   	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); 
   	options.c_iflag =(IGNBRK|IGNCR);

	options.c_iflag &= ~(ICRNL | IGNCR);

   	cfsetispeed(&options, B115200);	
	tcflush(Serialfd, TCIOFLUSH);	
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
int main(void)
{
	serial_init();
	motor_en();	
	motor_set_vel(100);
}
