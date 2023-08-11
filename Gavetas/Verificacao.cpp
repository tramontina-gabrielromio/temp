#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <math.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <vector>
#include <cmath>

using namespace std;

#define KEYImagemCam1OK 9903
#define KEYImagemCam2OK 9904
#define KEYGavetaVerificacao 9905
#define KEYResultado 9906
#define KEYGavetaMovimento 9909

typedef struct structResultados{
	char saida[50];
} structResultados;

int main(int argc, char *argv[])
{
	// ========== Memória compartilhada ==========verificac
	int shmidImagemCam1, shmidImagemCam2, shmidGavetaVerificacao, shmidResultado, shmidGavetaMovimento;
	char *path="/home/tramontina/Downloads/Gavetas";
	int *ImagemCam1OK, *ImagemCam2OK, *GavetaVerificacao, *GavetaMovimento;
	structResultados *Resultado;

	if((shmidImagemCam1=shmget(ftok(path,(key_t)KEYImagemCam1OK),sizeof(int*),IPC_CREAT|SHM_R|SHM_W))==-1){
		printf("\nErro na criacao do segmento de memoria...\n");
		exit(-1);
	}
	ImagemCam1OK=(int*)shmat(shmidImagemCam1,0,0);

	if((shmidImagemCam2=shmget(ftok(path,(key_t)KEYImagemCam2OK),sizeof(int*),IPC_CREAT|SHM_R|SHM_W))==-1){
		printf("\nErro na criacao do segmento de memoria...\n");
		exit(-1);
	}
	ImagemCam2OK=(int*)shmat(shmidImagemCam2,0,0);
	
	if((shmidGavetaVerificacao=shmget(ftok(path,(key_t)KEYGavetaVerificacao),sizeof(int*),IPC_CREAT|SHM_R|SHM_W))==-1){
		printf("\nErro na criacao do segmento de memoria...\n");
		exit(-1);
	}
	GavetaVerificacao=(int*)shmat(shmidGavetaVerificacao,0,0);

	if((shmidResultado=shmget(ftok(path,(key_t)KEYResultado),sizeof(structResultados*),IPC_CREAT|SHM_R|SHM_W))==-1){
		printf("\nErro na criacao do segmento de memoria...\n");
		exit(-1);
	}
	Resultado=(structResultados*)shmat(shmidResultado,0,0);

	string select = argv[1];

	while(*ImagemCam1OK==0 || *ImagemCam2OK==0){}

	if (*ImagemCam1OK==1 && *ImagemCam2OK==1){
		if (select=="GV1"){
			*GavetaVerificacao = 1;
		}
		else if (select=="GV2"){
			*GavetaVerificacao = 2;
		}
		else if (select=="GV3"){
			*GavetaVerificacao = 3;
		}
		else if (select=="GV4"){
			*GavetaVerificacao = 4;
		}
		else if (select=="GV5"){
			*GavetaVerificacao = 5;
		}
		else if (select=="GV6"){
			*GavetaVerificacao = 6;
		}
		else if (select=="GV7"){
			*GavetaVerificacao = 7;
		}
		else if (select=="GV8"){
			*GavetaVerificacao = 8;
		}
		while (*GavetaVerificacao!=0){}
		cout << Resultado->saida;
	}
	else if (*ImagemCam1OK!=1 && *ImagemCam2OK!=1)
		cout << "FCAM12";
	else if (*ImagemCam1OK!=1)
		cout << "FCAM1";
	else if (*ImagemCam2OK!=1)
		cout << "FCAM2";

	//*ImagemCam1OK=0;
	//*ImagemCam2OK=0;
	
    return 0;
}