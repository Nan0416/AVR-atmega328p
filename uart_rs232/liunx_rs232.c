
#include <fcntl.h> 
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
int set_interface_attribs (int fd, int speed, int parity)
{
        struct termios tty;
        memset(&tty, 0, sizeof tty);
        if (tcgetattr (fd, &tty) != 0)
        {
                perror("Error in tcgetattr");
                return -1;
        }

        cfsetospeed(&tty, speed);
        cfsetispeed(&tty, speed);
		tty.c_iflag |= IGNPAR;			// ignore frame error 
		tty.c_iflag &= ~IGNBRK;         // disable break processing
		tty.c_iflag &= ~INPCK;  		// disable parity checking
		tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl
		
		tty.c_cflag &= ~CSIZE;
        tty.c_cflag |= CS8;     // 8-bit chars
        tty.c_cflag &= ~CSTOPB; // one stop bit
        tty.c_cflag |= CREAD;   // enable read
        tty.c_cflag &= ~PARENB; // shut off parity
        tty.c_cflag |= CLOCAL; 
        tty.c_cflag &= ~CRTSCTS;
        tty.c_cflag &= ~HUPCL;
        
       
        //tty.c_lflag = 0;                // no signaling chars, no echo,
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


int main(){
	
	char *portname = "/dev/ttyUSB0";
	int fd = open (portname, O_RDWR | O_NOCTTY | O_SYNC);
	if (fd == -1){
        perror("Error in opening file: ");
        return -1;
	}
	set_interface_attribs(fd, B9600, 0);  // set speed to 115,200 bps, 8n1 (no parity)
	set_blocking(fd, 0);                // set no blocking
	if(dup2(fd,2) == -1){
		perror("Error in dup2");
		
	}
	
	sleep(2);
	if(close(11) == -1){
		perror("close");
		
	}
	
	//char buf[32];
	//read(fd, buf, 32);
	//printf("%s\n", buf);

	close(fd);
	
}
