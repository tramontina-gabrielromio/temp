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
#include "libGPIO/jetsonGPIO.h"

 /****************************************************************
 * Constants
 ****************************************************************/
 
#define SYSFS_GPIO_DIR "/sys/class/gpio"
#define POLL_TIMEOUT (3 * 1000) /* 3 seconds */
#define MAX_BUF 64

#define KEYEncoder 9902 //Endereço de memória compartilhada

//Configurações do GPIO
jetsonXavierGPIONumber gvAberta = gpio268; //Pino gaveta aberta - pino fisico 15
jetsonXavierGPIONumber gvFechada = gpio484; //Pino gaveta fechada - pino fisico 16
jetsonXavierGPIONumber BIT0 = gpio436; //Pino bit 0 - pino fisico 7
jetsonXavierGPIONumber BIT1 = gpio428; //Pino bit 1 - pino fisico 11
jetsonXavierGPIONumber BIT2 = gpio445; //Pino bit 2 - pino fisico 12
jetsonXavierGPIONumber BIT3 = gpio480; //Pino bit 3 - pino fisico 13
jetsonXavierGPIONumber encoderChA = gpio481; //Pino pulso encoder - pino fisico 22
jetsonXavierGPIONumber sobrecarga = gpio493; //Pino sobrecarga gaveta - pino fisico 19

/****************************************************************
 * Main
 ****************************************************************/

using namespace std;

int main()
{
	int cont = 0;
	int cont2 = 0;
	int cont3 = 0;
	int velreduzida = 0;
	int encoderAnt = 0;
	int gavetaTrancada = 0;

	//Memoria compartilhada
	int shmidMemoria;
	char *path="/home/tramontina/Downloads/Gavetas";
	int *contEncoder=0;
	
	if((shmidMemoria=shmget(ftok(path,(key_t)KEYEncoder),sizeof(int*),IPC_CREAT|SHM_R|SHM_W))==-1){
		printf("\nErro na criacao do segmento de memoria...\n");
		exit(-1);
	}
	contEncoder=(int*)shmat(shmidMemoria,0,0);
	*contEncoder = 0;

	//Config borda de subida para pulsos do encoder
	struct pollfd fdset[2];
	int gpio_fd;
	int timeout;
	char *buf[MAX_BUF];
	int nfds = 2;
	int len;

	gpio_fd = gpioOpen(encoderChA);
	memset((void*)fdset, 0, sizeof(fdset));
	fdset[0].fd = STDIN_FILENO;
	fdset[0].events = POLLIN;
	fdset[1].fd = gpio_fd;
	fdset[1].events = POLLPRI;
	timeout = 20;

	//Inicializa leitura de pulsos do encoder por borda de subida
	gpioExport(encoderChA);
	gpioSetDirection(encoderChA,inputPin);
	gpioSetEdge(encoderChA, "rising");
	
	timeout = POLL_TIMEOUT;
		
	while(1){
		
		if(gpioGetValue(gvAberta)==0 && gpioGetValue(gvFechada)==0)
		{
			poll(fdset, nfds, timeout);
			if (fdset[1].revents & POLLPRI){
				*contEncoder = *contEncoder+1;
				lseek(fdset[1].fd, 0, SEEK_SET);
				len = read(fdset[1].fd, buf, MAX_BUF);
				cout << *contEncoder << endl;
			}
			fflush(stdout);
		}
	}

	gpioClose(gpio_fd);
	shmdt(contEncoder);
	return 0;
}
