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

using namespace std;

#define KEYEncoder 9902 //Endereço de memória compartilhada

//Configurações do GPIO
jetsonXavierGPIONumber gvAberta = gpio268; //Pino gaveta aberta - pino fisico 15
jetsonXavierGPIONumber gvFechada = gpio484; //Pino gaveta fechada - pino fisico 16
jetsonXavierGPIONumber BIT0 = gpio436; //Pino bit 0 - pino fisico 7
jetsonXavierGPIONumber BIT1 = gpio428; //Pino bit 1 - pino fisico 11
jetsonXavierGPIONumber BIT2 = gpio445; //Pino bit 2 - pino fisico 12
jetsonXavierGPIONumber BIT3 = gpio480; //Pino bit 3 - pino fisico 13
jetsonXavierGPIONumber ligaLampada = gpio492; //Pino liga iluminação - pino fisico 21
jetsonXavierGPIONumber encoderChA = gpio481; //Pino pulso encoder - pino fisico 22
jetsonXavierGPIONumber sobrecarga = gpio493; //Pino sobrecarga gaveta - pino fisico 19

int *contEncoder;//=0;
int contReduzirPWM=0;
int PWMDutyCycleAbertura=65*500000;//2500
int PWMDutyCycleFechamento=45*500000;//2000
int PWMPeriod=100*500000;//20000

void Motores(char DATA[4]) //Movimento dos Motores
{
  int cont = 0;
  int cont2 = 0;
  int cont3 = 0;
  int velreduzida = 0;
  int encoderAnt = 0;
  int gavetaTrancada = 0;
  *contEncoder=0;

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

  // ===== Abre gaveta =====
  if((strncmp (DATA, "AGV", 3) == 0))
  {
    gpioSetValue(BIT3, off);
  	gpioSetPwmStatus (high);
    gpioSetPwmDutyCycle (PWMPeriod);
    int contEncoderAbertura = 0;

    while (cont <= 3){

      //Contagem de pulsos do encoder
      if(gpioGetValue(gvAberta)==0 && gpioGetValue(gvFechada)==0)
      {
        poll(fdset, nfds, timeout);
        if (fdset[1].revents & POLLPRI){
          contEncoderAbertura = contEncoderAbertura+1;
          lseek(fdset[1].fd, 0, SEEK_SET);
          len = read(fdset[1].fd, buf, MAX_BUF);
          //cout << contEncoderAbertura << endl;
          if (gpioGetValue(sobrecarga)==0){
            cont3++;
            if (cont3>=40){
              gavetaTrancada = 1;
              cout << "GVT";
              break;}
          }
          else
            cont3 = 0;
        }
        else if (contEncoderAbertura>=200){
          gavetaTrancada = 1;
          cout << "GVT";
          break;
        }
        fflush(stdout);
      }
      
      //Verifica gaveta trancada
      if (contEncoderAbertura>=encoderAnt+10 || contEncoderAbertura<=200){
        cont2=0;
        encoderAnt = contEncoderAbertura;
      }
      else
        cont2++;

      if (cont2>=3000){
        gpioSetPwmDutyCycle (0);
        gavetaTrancada = 1;
        cout << "GVT";
        break;
      }

      //Reduz velocidade para finalizar movimento
      if (contEncoderAbertura >= contReduzirPWM && velreduzida == 0){
        gpioSetPwmDutyCycle (PWMDutyCycleAbertura);
        velreduzida=1;
      }

      //Verifica se a gaveta está completamente aberta
      if (gpioGetValue(gvAberta) == 1){
        cont = cont + 1;
      }
      else{
        cont = 0;
      }
    }
    gpioSetPwmDutyCycle (0);
	  gpioSetPwmStatus (low);
    if (gavetaTrancada==0)
      cout << "GVA" << " Encoder: " << contEncoderAbertura;
  }

  // ===== Fecha gaveta =====
  if ((strncmp (DATA, "FGV", 3) == 0))
  {
    gpioSetValue(BIT3, on);
	  gpioSetValue(ligaLampada, on);
  	gpioSetPwmStatus (high);
    gpioSetPwmDutyCycle (PWMPeriod);

    int cont99=0;

    while(cont99<1000){
      cont99++;
    }

    while (cont <= 5){

      //Contagem de pulsos do encoder
      if(gpioGetValue(gvAberta)==0 && gpioGetValue(gvFechada)==0)
      {
        poll(fdset, nfds, timeout);
        if (fdset[1].revents & POLLPRI){
          *contEncoder = *contEncoder+1;
          lseek(fdset[1].fd, 0, SEEK_SET);
          len = read(fdset[1].fd, buf, MAX_BUF);
          //cout << *contEncoder << endl;
          if (gpioGetValue(sobrecarga)==0){
            cont3++;
            if (cont3>=40){
              gavetaTrancada = 1;
              cout << "GVT";
              break;}
          }
          else
            cont3 = 0;
        }
        else if (*contEncoder>=200){
          gavetaTrancada = 1;
          cout << "GVT";
          break;
        }
        fflush(stdout);
      }

      //Verifica gaveta trancada
      if (*contEncoder>=encoderAnt+10 || *contEncoder<=200){
        cont2=0;
        encoderAnt = *contEncoder;
      }
      else
        cont2++;

      if (cont2>=3000){
        gpioSetPwmDutyCycle (0);
        gavetaTrancada = 1;
        cout << "GVT";
        break;
      }
      
      //Reduz velocidade para finalizar movimento
      if (*contEncoder >= contReduzirPWM && velreduzida == 0){
        gpioSetPwmDutyCycle (PWMDutyCycleFechamento);
        velreduzida=1;
      }

      //Verifica se a gaveta está completamente fechada
      if (gpioGetValue(gvFechada) == 1){
        cont = cont + 1;
      }
      else{
        cont = 0;
      }
    }
    gpioSetPwmDutyCycle (0);
	  gpioSetPwmStatus (low);
	  gpioSetValue(ligaLampada, off);
    if (gavetaTrancada==0)
      cout << "GVF" << " Encoder: " << *contEncoder;

    *contEncoder = 0;
  }
}

char AuxLeGaveta(){
  int cont1=0;
  int cont2=0;
  int cont3=0;
  char saida;
  while(1){
    if (gpioGetValue(gvAberta) == 1 && gpioGetValue(gvFechada) == 0){
      cont1++;
      cont2=0;
      cont3=0;
    }
    else if (gpioGetValue(gvAberta) == 0 && gpioGetValue(gvFechada) == 1){
      cont1=0;
      cont2++;
      cont3=0;
    }
    else{
      cont1=0;
      cont2=0;
      cont3++;
    }
    if (cont1>=10){
      saida = 'A';
      break;
    }
    else if (cont2>=10){
      saida = 'F';
      break;
    }
    else if (cont3>=10){
      saida = 'D';
      break;
    }
  }
  return saida;
}

void EstadoGavetas(){
  char Gavetas[8];

  gpioSetValue(BIT0, off);
  gpioSetValue(BIT1, off);
  gpioSetValue(BIT2, off);
  Gavetas[0]= AuxLeGaveta();

  gpioSetValue(BIT0, on);
  gpioSetValue(BIT1, off);
  gpioSetValue(BIT2, off);
  Gavetas[1]= AuxLeGaveta();

  gpioSetValue(BIT0, off);
  gpioSetValue(BIT1, on);
  gpioSetValue(BIT2, off);
  Gavetas[2]= AuxLeGaveta();

  gpioSetValue(BIT0, on);
  gpioSetValue(BIT1, on);
  gpioSetValue(BIT2, off);
  Gavetas[3]= AuxLeGaveta();

  gpioSetValue(BIT0, off);
  gpioSetValue(BIT1, off);
  gpioSetValue(BIT2, on);
  Gavetas[4]= AuxLeGaveta();

  gpioSetValue(BIT0, on);
  gpioSetValue(BIT1, off);
  gpioSetValue(BIT2, on);
  Gavetas[5]= AuxLeGaveta();

  gpioSetValue(BIT0, off);
  gpioSetValue(BIT1, on);
  gpioSetValue(BIT2, on);
  Gavetas[6]= AuxLeGaveta();

  gpioSetValue(BIT0, on);
  gpioSetValue(BIT1, on);
  gpioSetValue(BIT2, on);
  Gavetas[7]= AuxLeGaveta();

  cout << Gavetas;
}

int main(int argc, char *argv[]){
  int Estado;
	int cont1=0;
	int cont2=0;

	struct pollfd fdset[2];
  int gpio_fd, timeout;
	char *buf[MAX_BUF];
	int nfds = 2;
	int len;
  string select;

  //Inicializa entradas e saídas digitais
  gpioExport(gvAberta);
  gpioSetDirection(gvAberta,inputPin);
  gpioExport(gvFechada);
  gpioSetDirection(gvFechada,inputPin);
  gpioExport(sobrecarga);
  gpioSetDirection(sobrecarga,inputPin);
  gpioExport(BIT0);
  gpioSetDirection(BIT0,outputPin);
  gpioExport(BIT1);
  gpioSetDirection(BIT1,outputPin);
  gpioExport(BIT2);
  gpioSetDirection(BIT2,outputPin);
  gpioExport(BIT3);
  gpioSetDirection(BIT3,outputPin);
  gpioExport(ligaLampada);
  gpioSetDirection(ligaLampada,outputPin);

  //Inicializa leitura de pulsos do encoder por borda de subida
  gpioExport(encoderChA);
  gpioSetDirection(encoderChA,inputPin);
  gpioSetEdge(encoderChA, "rising");

	//Memoria compartilhada
	int shmidMemoria;
	char *path="/home/tramontina/Downloads/Gavetas";
	
	if((shmidMemoria=shmget(ftok(path,(key_t)KEYEncoder),sizeof(int*),IPC_CREAT|SHM_R|SHM_W))==-1){
		printf("\nErro na criacao do segmento de memoria...\n");
		exit(-1);
	}
	contEncoder=(int*)shmat(shmidMemoria,0,0);
	//*contEncoder = 0;

	gpioSetValue(BIT0, off);
	gpioSetValue(BIT1, off);
	gpioSetValue(BIT2, off);
	gpioSetValue(BIT3, off);
	gpioSetValue(ligaLampada, off);

	//Inicializa PWM
	gpioPwmExport ();
	gpioSetPwmPeriod (PWMPeriod);
	gpioSetPwmDutyCycle (0);
	gpioSetPwmStatus (low);

  select = argv[1];

  if (select=="AGV1"){ //AGV1
    gpioSetValue(BIT0, off);
    gpioSetValue(BIT1, off);
    gpioSetValue(BIT2, off);
    contReduzirPWM = 3800;
    Motores("AGV");
  }
  else if (select=="FGV1"){ //FGV1
    gpioSetValue(BIT0, off);
    gpioSetValue(BIT1, off);
    gpioSetValue(BIT2, off);
    contReduzirPWM = 4300;
    *contEncoder = 0;
    Motores("FGV");
  }
  else if (select=="AGV2"){ //AGV2
    gpioSetValue(BIT0, on);
    gpioSetValue(BIT1, off);
    gpioSetValue(BIT2, off);
    contReduzirPWM = 3100;
    Motores("AGV");
  }
  else if (select=="FGV2"){ //FGV2
    gpioSetValue(BIT0, on);
    gpioSetValue(BIT1, off);
    gpioSetValue(BIT2, off);
    contReduzirPWM = 3200;
    *contEncoder = 0;
    Motores("FGV");
  }
  else if (select=="AGV3"){ //AGV3
    gpioSetValue(BIT0, off);
    gpioSetValue(BIT1, on);
    gpioSetValue(BIT2, off);
    contReduzirPWM = 3300;
    Motores("AGV");
  }
  else if (select=="FGV3"){ //FGV3
    gpioSetValue(BIT0, off);
    gpioSetValue(BIT1, on);
    gpioSetValue(BIT2, off);
    contReduzirPWM = 3400;
    *contEncoder = 0;
    Motores("FGV");
  }
  else if (select=="AGV4"){ //AGV4
    gpioSetValue(BIT0, on);
    gpioSetValue(BIT1, on);
    gpioSetValue(BIT2, off);
    contReduzirPWM = 3300;
    Motores("AGV");
  }
  else if (select=="FGV4"){ //FGV4
    gpioSetValue(BIT0, on);
    gpioSetValue(BIT1, on);
    gpioSetValue(BIT2, off);
    contReduzirPWM = 3400;
    *contEncoder = 0;
    Motores("FGV");
  }
  else if (select=="AGV5"){ //AGV5
    gpioSetValue(BIT0, off);
    gpioSetValue(BIT1, off);
    gpioSetValue(BIT2, on);
    contReduzirPWM = 3300;
    Motores("AGV");
  }
  else if (select=="FGV5"){ //FGV5
    gpioSetValue(BIT0, off);
    gpioSetValue(BIT1, off);
    gpioSetValue(BIT2, on);
    contReduzirPWM = 3400;
    *contEncoder = 0;
    Motores("FGV");
  }
  else if (select=="AGV6"){ //AGV6
    gpioSetValue(BIT0, on);
    gpioSetValue(BIT1, off);
    gpioSetValue(BIT2, on);
    contReduzirPWM = 3300;
    Motores("AGV");
  }
  else if (select=="FGV6"){ //FGV6
    gpioSetValue(BIT0, on);
    gpioSetValue(BIT1, off);
    gpioSetValue(BIT2, on);
    contReduzirPWM = 3400;
    *contEncoder = 0;
    Motores("FGV");
  }
  else if (select=="AGV7"){ //AGV7
    gpioSetValue(BIT0, off);
    gpioSetValue(BIT1, on);
    gpioSetValue(BIT2, on);
    contReduzirPWM = 3300;
    Motores("AGV");
  }
  else if (select=="FGV7"){ //FGV7
    gpioSetValue(BIT0, off);
    gpioSetValue(BIT1, on);
    gpioSetValue(BIT2, on);
    contReduzirPWM = 3400;
    *contEncoder = 0;
    Motores("FGV");
  }
  else if (select=="AGV8"){ //AGV8
    gpioSetValue(BIT0, on);
    gpioSetValue(BIT1, on);
    gpioSetValue(BIT2, on);
    contReduzirPWM = 3300;
    Motores("AGV");
  }
  else if (select=="FGV8"){ //FGV8
    gpioSetValue(BIT0, on);
    gpioSetValue(BIT1, on);
    gpioSetValue(BIT2, on);
    contReduzirPWM = 3400;
    *contEncoder = 0;
    Motores("FGV");
  }
  else if (select=="GAVETAS"){ //Estado das GAVETAS
    EstadoGavetas();
  }

	gpioClose(gpio_fd);
	shmdt(contEncoder);

  return 0;
}
