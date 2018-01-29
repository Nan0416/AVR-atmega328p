
#include <fcntl.h> 
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
int set_interface_attribs (int fd, int speed, int parity)
{
        struct termios tty;
        memset(&tty, 0, sizeof tty);
        if (tcgetattr (fd, &tty) != 0)
        {
                perror("Error in tcgetattr");
                return -1;
        }
		// set baudrate
        cfsetospeed(&tty, speed);
        cfsetispeed(&tty, speed);
        //
		tty.c_iflag |= IGNPAR;			// ignore frame error 
		tty.c_iflag &= ~IGNBRK;         // disable break processing
		tty.c_iflag &= ~INPCK;  		// disable parity checking
		tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl
		
		// c_cflag control byte
		tty.c_cflag &= ~CSIZE;
        tty.c_cflag |= CS8;     // 8-bit chars
        tty.c_cflag |= CSTOPB; // one stop bit
        tty.c_cflag |= CREAD;   // enable read
        tty.c_cflag |= PARENB; // even parity
        tty.c_cflag &= ~PARODD; 
        tty.c_cflag |= CLOCAL; // directly connected
        tty.c_cflag &= ~CRTSCTS; // disable RTS/CTS   (hardware)   flow   control.
        tty.c_cflag &= ~HUPCL; //
        
       
        tty.c_lflag &= ~ECHO;                // no signaling chars, no echo,
                                        // no canonical processing
        tty.c_lflag &= ~ICANON;                                
                                        
        //tty.c_oflag = 0;                // no remapping, no delays
        tty.c_cc[VMIN]  = 0;            // read doesn't block
        tty.c_cc[VTIME] = 0.5;            // 0.5 seconds read timeout


        if (tcsetattr(fd, TCSANOW, &tty) != 0)
        {
                perror("Error in tcsetattr");
                return -1;
        }
        return 0;
}

int set_blocking (int fd, int should_block)
{
        struct termios tty;
        memset(&tty, 0, sizeof tty);
        if (tcgetattr(fd, &tty) != 0)
        {
			   perror("Error in tcgetattr");
               return -1;
        }

        tty.c_cc[VMIN]  = should_block ? 1 : 0;
        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

        if (tcsetattr (fd, TCSANOW, &tty) != 0){
			perror("Error in tcsetattr");
			return -1;
		}
		return 0;
}

int writeRS232(int fd, char * data, int num){
	if(num > 16){
		num = 16;
	}
	uint8_t len = (uint8_t)num;
	uint8_t len2;
	write(fd, &len, 1);
	read(fd, &len2, 1);
	if(len2 != len){
		return -1;
	}
	for(int i = 0; i < num; i++){
		write(fd, data+i, 1);
		char x;
		read(fd, &x, 1);
		if(x != data[i]){
			printf("error\n");
			return -1;
		}
	}
	return num;
}

int readRS232(int fd, char * buffer, int num){
	uint8_t len2;
	uint8_t len = (uint8_t) num;
	read(fd, &len2, 1);
	if(len2 < len){
		num = len2;
	}
	
	for(int i = 0;i < num; i++){
		read(fd, buffer+i, 1);
	}
	return num;
	
}
// passive listening (block reading)
int main(){
	
	char *portname = "/dev/ttyUSB0";
	int fd = open (portname, O_RDWR | O_NOCTTY | O_SYNC);
	if (fd == -1){
        perror("Error in opening file: ");
        return -1;
	}
	set_interface_attribs(fd, B19200, 0);  // set speed to 115,200 bps, 8n1 (no parity)
	set_blocking(fd, 1);                // set blocking
	char buf[17];
	int p;
	for(int i = 0; 1; i++){
		p = readRS232(fd, buf, 16);
		buf[p] = '\0';
		printf("%s\n", buf);
	}
	close(fd);
	
}
