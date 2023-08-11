#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

#include <arpa/inet.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/time.h>

 /****************************************************************
 * Constants
 ****************************************************************/
 
#define SYSFS_GPIO_DIR "/sys/class/gpio"
#define POLL_TIMEOUT (3 * 1000) /* 3 seconds */
#define MAX_BUF 64

#define KEYEncoder 9902 //Endereço de memória compartilhada

/****************************************************************
 * gpio_export
 ****************************************************************/
int gpio_export(unsigned int gpio)
{
	int fd, len;
	char buf[MAX_BUF];
 
	fd = open(SYSFS_GPIO_DIR "/export", O_WRONLY);
	if (fd < 0) {
		perror("gpio/export");
		return fd;
	}
 
	len = snprintf(buf, sizeof(buf), "%d", gpio);
	write(fd, buf, len);
	close(fd);
 
	return 0;
}

/****************************************************************
 * gpio_unexport
 ****************************************************************/
int gpio_unexport(unsigned int gpio)
{
	int fd, len;
	char buf[MAX_BUF];
 
	fd = open(SYSFS_GPIO_DIR "/unexport", O_WRONLY);
	if (fd < 0) {
		perror("gpio/export");
		return fd;
	}
 
	len = snprintf(buf, sizeof(buf), "%d", gpio);
	write(fd, buf, len);
	close(fd);
	return 0;
}

/****************************************************************
 * gpio_set_dir
 ****************************************************************/
int gpio_set_dir(unsigned int gpio, unsigned int out_flag)
{
	int fd, len;
	char buf[MAX_BUF];
 
	len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR  "/gpio%d/direction", gpio);
 
	fd = open(buf, O_WRONLY);
	if (fd < 0) {
		perror("gpio/direction");
		return fd;
	}
 
	if (out_flag)
		write(fd, "out", 4);
	else
		write(fd, "in", 3);
 
	close(fd);
	return 0;
}

/****************************************************************
 * gpio_set_value
 ****************************************************************/
int gpio_set_value(unsigned int gpio, unsigned int value)
{
	int fd, len;
	char buf[MAX_BUF];
 
	len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);
 
	fd = open(buf, O_WRONLY);
	if (fd < 0) {
		perror("gpio/set-value");
		return fd;
	}
 
	if (value)
		write(fd, "1", 2);
	else
		write(fd, "0", 2);
 
	close(fd);
	return 0;
}

/****************************************************************
 * gpio_get_value
 ****************************************************************/
int gpio_get_value(unsigned int gpio)
{
	int fd, len, value;
	char buf[MAX_BUF];
	char ch;

	len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);
 
	fd = open(buf, O_RDONLY);
	if (fd < 0) {
		perror("gpio/get-value");
		return fd;
	}
 
	read(fd, &ch, 1);

	if (ch != '0') {
		value = 1;
	} else {
		value = 0;
	}
 
	close(fd);
	return value;
}


/****************************************************************
 * gpio_set_edge
 ****************************************************************/

int gpio_set_edge(unsigned int gpio, char *edge)
{
	int fd, len;
	char buf[MAX_BUF];

	len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/edge", gpio);
 
	fd = open(buf, O_WRONLY);
	if (fd < 0) {
		perror("gpio/set-edge");
		return fd;
	}
 
	write(fd, edge, strlen(edge) + 1); 
	close(fd);
	return 0;
}

/****************************************************************
 * gpio_fd_open
 ****************************************************************/

int gpio_fd_open(unsigned int gpio)
{
	int fd, len;
	char buf[MAX_BUF];

	len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);
 
	fd = open(buf, O_RDONLY | O_NONBLOCK );
	if (fd < 0) {
		perror("gpio/fd_open");
	}
	return fd;
}

/****************************************************************
 * gpio_fd_close
 ****************************************************************/

int gpio_fd_close(int fd)
{
	return close(fd);
}

/****************************************************************
 * Main
 ****************************************************************/
int main()
{
	struct pollfd fdset[2];
	int nfds = 2;
	int gpio_fd, timeout, rc;
	char *buf[MAX_BUF];
	unsigned int gpio;
	int len;
	int cont1=0;
	int cont2=0;
	int Estado=0;
	
	//Memoria compartilhada
	int shmidMemoria;
	char *path="/home/nvidia/Downloads/Camera/uEye_SimpleSingleGrab";
	int *contEncoder=0;
	
	if((shmidMemoria=shmget(ftok(path,(key_t)KEYEncoder),sizeof(int*),IPC_CREAT|SHM_R|SHM_W))==-1){
		printf("\nErro na criacao do segmento de memoria...\n");
		exit(-1);
	}
	contEncoder=(int*)shmat(shmidMemoria,0,0);
	*contEncoder = 0;
	//

	gpio = 388; //GPIO_0 Orbit Carrier

	gpio_export(gpio);
	gpio_set_dir(gpio, 0);
	gpio_set_edge(gpio, "rising");
	gpio_fd = gpio_fd_open(gpio);

	gpio_export(298);
	gpio_set_dir(298, 0);
	gpio_fd_open(298);
	gpio_export(480);
	gpio_set_dir(480, 0);
	gpio_fd_open(480);
	
	timeout = POLL_TIMEOUT;
		
	while(1){
		
		switch (Estado){
		
			case 0:
				if (gpio_get_value(298)==1 && gpio_get_value(480)==0){
					cont1++;
					if (cont1>=10){
						cont1 = 0;
						Estado = 2;}}
				else{
					cont1 = 0;
					Estado = 1;}
					
				break;
				
			case 1:
				*contEncoder = 0;
				Estado = 0;
				break;
		
			case 2:
				while(gpio_get_value(298)==1);
			
				do{
					memset((void*)fdset, 0, sizeof(fdset));

					fdset[0].fd = STDIN_FILENO;
					fdset[0].events = POLLIN;
				  
					fdset[1].fd = gpio_fd;
					fdset[1].events = POLLPRI;

					rc = poll(fdset, nfds, timeout);
						
					if (fdset[1].revents & POLLPRI){
						*contEncoder = *contEncoder+1;
						lseek(fdset[1].fd, 0, SEEK_SET);
						len = read(fdset[1].fd, buf, MAX_BUF);
						printf("\nContador %d", *contEncoder);
					}

					fflush(stdout);
					
					if(gpio_get_value(480)==1)
						cont2++;
					else
						cont2=0;
					
				}while(cont2<=2);
				
				cont2=0;
				Estado = 0;
				
				break;
		}	
			
	}

	gpio_fd_close(gpio_fd);
	shmdt(contEncoder);
	return 0;
}
