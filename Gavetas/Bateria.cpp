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
#include <math.h>

using namespace std;

//Configurações do GPIO
jetsonXavierGPIONumber bateriaBIT0 = gpio421; //Pino nivel bateria bit 0 - pino fisico 29
jetsonXavierGPIONumber bateriaBIT2 = gpio422; //Pino nivel bateria bit 2 - pino fisico 31
jetsonXavierGPIONumber bateriaBIT1 = gpio424; //Pino nivel bateria bit 1 - pino fisico 32
jetsonXavierGPIONumber bateriaCarregando = gpio448; //Pino bateria carregando - pino fisico 35
jetsonXavierGPIONumber bateriaCarregada = gpio482; //Pino bateria carregada - pino fisico 37

int NivelBateria(){
  int ADC=0;
  float Tensao=0;
  int Carga=0;

  //Verifica combinação binária e converte para decimal
  if (gpioGetValue(bateriaBIT0)==1)
    ADC = ADC + 1;
  if (gpioGetValue(bateriaBIT1)==1)
    ADC = ADC + 2;
  if (gpioGetValue(bateriaBIT2)==1)
    ADC = ADC + 4;

  //Converte valor para nivel de tensão da bateria
  Tensao = ADC*0.625+20+0.32;

  //Converte tensão para carga da bateria
  Carga = (Tensao-22.2)*(100/3);

  return ADC;
  //cout << ADC;
}

int main(){
  gpioExport(bateriaBIT0);
  gpioSetDirection(bateriaBIT0,inputPin);
  gpioExport(bateriaBIT1);
  gpioSetDirection(bateriaBIT1,inputPin);
  gpioExport(bateriaBIT2);
  gpioSetDirection(bateriaBIT2,inputPin);
  gpioExport(bateriaCarregada);
  gpioSetDirection(bateriaCarregada,inputPin);
  gpioExport(bateriaCarregando);
  gpioSetDirection(bateriaCarregando,inputPin);

  double soma=0;

  for (int cont=0; cont<10; cont++){
    soma += NivelBateria(); //Retorna nível da bateria
    usleep(10000);
  }

  int medicao = round(soma/10);

  cout << medicao << gpioGetValue(bateriaCarregada) << gpioGetValue(bateriaCarregando);
  //cout << "Nivel da bateria: " << medicao << endl;
  //cout << "Bateria carregada: " << gpioGetValue(bateriaCarregada) << endl;
  //cout << "Bateria carregando: " << gpioGetValue(bateriaCarregando) << endl;

  return 0;
}
