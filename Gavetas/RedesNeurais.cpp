#include <iostream>
//#include <ueye.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <string.h>
#include <sstream>
#include <math.h>
#include <arpa/inet.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <vector>
#include <cmath>
#include "cppflow/cppflow.h"
#include "tensorflow/c/c_api.h"

using namespace cv;
using namespace std;

/*HIDS hCam = 0;      // 0 for the next available camera. 1-254 to access by ID
SENSORINFO sInfo;
HWND hWndDisplay;
SENSORSCALERINFO sScaleInfo;

char* pcImageMemory;
int DisplayWidth, DisplayHeight;
double newFPS;*/

//Endereços de memória compartilhada
#define KEYEncoder 9902
#define KEYImagemCam1OK 9903
#define KEYImagemCam2OK 9904
#define KEYGavetaVerificacao 9905
#define KEYResultado 9906

typedef struct structResultados{
	char saida[50];
} structResultados;

cppflow::model model1_1("RedesGV1/model_1");
cppflow::model model1_2("RedesGV1/model_2");
cppflow::model model1_3("RedesGV1/model_3");
cppflow::model model1_4("RedesGV1/model_4");
cppflow::model model1_5("RedesGV1/model_5");
cppflow::model model1_6("RedesGV1/model_6");
cppflow::model model1_7("RedesGV1/model_7");
cppflow::model model1_8("RedesGV1/model_8");
cppflow::model model1_9("RedesGV1/model_9");
cppflow::model model1_10("RedesGV1/model_10");
cppflow::model model1_11("RedesGV1/model_11");

cppflow::model model2_1("RedesGV2/model_1");
cppflow::model model2_2("RedesGV2/model_2");
cppflow::model model2_3("RedesGV2/model_3");
cppflow::model model2_4("RedesGV2/model_4");

cppflow::model model3_1("RedesGV3/model_1");
cppflow::model model3_2("RedesGV3/model_2");
cppflow::model model3_3("RedesGV3/model_3");
cppflow::model model3_4("RedesGV3/model_4");
cppflow::model model3_5("RedesGV3/model_5");
cppflow::model model3_6("RedesGV3/model_6");
cppflow::model model3_7("RedesGV3/model_7");
cppflow::model model3_8("RedesGV3/model_8");
cppflow::model model3_9("RedesGV3/model_9");
cppflow::model model3_10("RedesGV3/model_10");
cppflow::model model3_11("RedesGV3/model_11");
cppflow::model model3_12("RedesGV3/model_12");

cppflow::model model4_1("RedesGV4/model_1");
cppflow::model model4_2("RedesGV4/model_2");
cppflow::model model4_3("RedesGV4/model_3");
cppflow::model model4_4("RedesGV4/model_4");
cppflow::model model4_5("RedesGV4/model_5");
cppflow::model model4_6("RedesGV4/model_6");
cppflow::model model4_7("RedesGV4/model_7");
cppflow::model model4_8("RedesGV4/model_8");
cppflow::model model4_9("RedesGV4/model_9");
cppflow::model model4_10("RedesGV4/model_10");

cppflow::model model5_1("RedesGV5/model_1");
cppflow::model model5_2("RedesGV5/model_2");
cppflow::model model5_3("RedesGV5/model_3");
cppflow::model model5_4("RedesGV5/model_4");
cppflow::model model5_5("RedesGV5/model_5");
cppflow::model model5_6("RedesGV5/model_6");
cppflow::model model5_7("RedesGV5/model_7");
cppflow::model model5_8("RedesGV5/model_8");
cppflow::model model5_9("RedesGV5/model_9");
cppflow::model model5_10("RedesGV5/model_10");

cppflow::model model6_1("RedesGV6/model_1");
cppflow::model model6_2("RedesGV6/model_2");
cppflow::model model6_3("RedesGV6/model_3");
cppflow::model model6_4("RedesGV6/model_4");
cppflow::model model6_5("RedesGV6/model_5");
cppflow::model model6_6("RedesGV6/model_6");
cppflow::model model6_7("RedesGV6/model_7");
cppflow::model model6_8("RedesGV6/model_8");
cppflow::model model6_9("RedesGV6/model_9");
cppflow::model model6_10("RedesGV6/model_10");
cppflow::model model6_11("RedesGV6/model_11");
cppflow::model model6_12("RedesGV6/model_12");
cppflow::model model6_13("RedesGV6/model_13");
cppflow::model model6_14("RedesGV6/model_14");
cppflow::model model6_15("RedesGV6/model_15");
cppflow::model model6_16("RedesGV6/model_16");
cppflow::model model6_17("RedesGV6/model_17");
cppflow::model model6_18("RedesGV6/model_18");
cppflow::model model6_19("RedesGV6/model_19");

cppflow::model model7_1("RedesGV7/model_1");
cppflow::model model7_2("RedesGV7/model_2");
cppflow::model model7_3("RedesGV7/model_3");
cppflow::model model7_4("RedesGV7/model_4");
cppflow::model model7_5("RedesGV7/model_5");
cppflow::model model7_6("RedesGV7/model_6");
cppflow::model model7_7("RedesGV7/model_7");
cppflow::model model7_8("RedesGV7/model_8");
cppflow::model model7_9("RedesGV7/model_9");

cppflow::model model8_1("RedesGV8/model_1");
cppflow::model model8_2("RedesGV8/model_2");
cppflow::model model8_3("RedesGV8/model_3");
cppflow::model model8_4("RedesGV8/model_4");
cppflow::model model8_5("RedesGV8/model_5");
cppflow::model model8_6("RedesGV8/model_6");
cppflow::model model8_7("RedesGV8/model_7");
cppflow::model model8_8("RedesGV8/model_8");

int RedeTool1_1(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV1/Tool1.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model1_1({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool1 << ":	" << fResult*100 << "%";
	
	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool1_2(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV1/Tool2.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
    
    //Aplica rede neural para análise da imagem
	auto output = model1_2({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool2 << ":	" << fResult*100 << "%";
	
	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool1_3(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV1/Tool3.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model1_3({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool3 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool1_4(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV1/Tool4.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model1_4({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool4 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool1_5(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV1/Tool5.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model1_5({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool5 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool1_6(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV1/Tool6.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model1_6({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool6 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool1_7(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV1/Tool7.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model1_7({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool7 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool1_8(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV1/Tool8.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model1_8({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool8 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool1_9(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV1/Tool9.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model1_9({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool9 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool1_10(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV1/Tool10.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model1_10({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool10 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool1_11(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV1/Tool11.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model1_11({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool11 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}


int RedeTool2_1(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV2/Tool1.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model2_1({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool1 << ":	" << fResult*100 << "%";
	
	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool2_2(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV2/Tool2.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
    
    //Aplica rede neural para análise da imagem
	auto output = model2_2({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool2 << ":	" << fResult*100 << "%";
	
	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool2_3(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV2/Tool3.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model2_3({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool3 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool2_4(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV2/Tool4.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model2_4({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool4 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}


int RedeTool3_1(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV3/Tool1.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model3_1({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool1 << ":	" << fResult*100 << "%";
	
	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool3_2(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV3/Tool2.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
    
    //Aplica rede neural para análise da imagem
	auto output = model3_2({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool2 << ":	" << fResult*100 << "%";
	
	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool3_3(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV3/Tool3.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model3_3({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool3 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool3_4(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV3/Tool4.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model3_4({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool4 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool3_5(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV3/Tool5.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model3_5({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool5 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool3_6(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV3/Tool6.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model3_6({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool6 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool3_7(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV3/Tool7.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model3_7({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool7 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool3_8(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV3/Tool8.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model3_8({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool8 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool3_9(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV3/Tool9.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model3_9({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool9 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool3_10(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV3/Tool10.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model3_10({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool10 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool3_11(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV3/Tool11.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model3_11({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool11 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool3_12(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV3/Tool12.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model3_12({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool12 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool4_1(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV4/Tool1.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model4_1({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool1 << ":	" << fResult*100 << "%";
	
	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool4_2(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV4/Tool2.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
    
    //Aplica rede neural para análise da imagem
	auto output = model4_2({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool2 << ":	" << fResult*100 << "%";
	
	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool4_3(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV4/Tool3.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model4_3({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool3 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool4_4(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV4/Tool4.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model4_4({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool4 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool4_5(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV4/Tool5.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model4_5({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool5 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool4_6(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV4/Tool6.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model4_6({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool6 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool4_7(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV4/Tool7.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model4_7({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool7 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool4_8(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV4/Tool8.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model4_8({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool8 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool4_9(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV4/Tool9.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model4_9({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool9 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool4_10(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV4/Tool10.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model4_10({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool10 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool5_1(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV5/Tool1.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model5_1({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool1 << ":	" << fResult*100 << "%";
	
	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool5_2(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV5/Tool2.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
    
    //Aplica rede neural para análise da imagem
	auto output = model5_2({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool2 << ":	" << fResult*100 << "%";
	
	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool5_3(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV5/Tool3.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model5_3({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool3 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool5_4(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV5/Tool4.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model5_4({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool4 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool5_5(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV5/Tool5.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model5_5({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool5 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool5_6(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV5/Tool6.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model5_6({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool6 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool5_7(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV5/Tool7.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model5_7({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool7 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool5_8(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV5/Tool8.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model5_8({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool7 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool5_9(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV5/Tool9.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model5_9({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool7 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool5_10(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV5/Tool10.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model5_10({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool7 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool6_1(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV6/Tool1.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model6_1({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool1 << ":	" << fResult*100 << "%";
	
	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool6_2(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV6/Tool2.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
    
    //Aplica rede neural para análise da imagem
	auto output = model6_2({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool2 << ":	" << fResult*100 << "%";
	
	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool6_3(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV6/Tool3.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model6_3({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool3 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool6_4(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV6/Tool4.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model6_4({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool4 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool6_5(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV6/Tool5.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model6_5({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool5 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool6_6(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV6/Tool6.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model6_6({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool6 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool6_7(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV6/Tool7.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model6_7({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool7 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool6_8(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV6/Tool8.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model6_8({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool7 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool6_9(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV6/Tool9.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model6_9({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool7 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool6_10(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV6/Tool10.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model6_10({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool7 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool6_11(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV6/Tool11.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model6_11({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool7 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool6_12(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV6/Tool12.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model6_12({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool7 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool6_13(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV6/Tool13.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model6_13({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool7 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool6_14(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV6/Tool14.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model6_14({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool7 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool6_15(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV6/Tool15.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model6_15({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool7 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool6_16(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV6/Tool16.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model6_16({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool7 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool6_17(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV6/Tool17.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model6_17({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool7 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool6_18(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV6/Tool18.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model6_18({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool7 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool6_19(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV6/Tool19.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model6_19({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool7 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool7_1(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV7/Tool1.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model7_1({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool1 << ":	" << fResult*100 << "%";
	
	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool7_2(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV7/Tool2.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
    
    //Aplica rede neural para análise da imagem
	auto output = model7_2({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool2 << ":	" << fResult*100 << "%";
	
	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool7_3(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV7/Tool3.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model7_3({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool3 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool7_4(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV7/Tool4.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model7_4({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool4 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool7_5(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV7/Tool5.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model7_5({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool5 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool7_6(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV7/Tool6.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model7_6({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool6 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool7_7(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV7/Tool7.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model7_7({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool6 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool7_8(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV7/Tool8.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model7_8({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool6 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool7_9(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV7/Tool9.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model7_9({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool6 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool8_1(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV8/Tool1.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model8_1({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool1 << ":	" << fResult*100 << "%";
	
	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool8_2(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV8/Tool2.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
    
    //Aplica rede neural para análise da imagem
	auto output = model8_2({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool2 << ":	" << fResult*100 << "%";
	
	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool8_3(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV8/Tool3.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model8_3({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool3 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool8_4(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV8/Tool4.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model8_4({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool4 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool8_5(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV8/Tool5.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model8_5({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool4 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool8_6(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV8/Tool6.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model8_6({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool4 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool8_7(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV8/Tool7.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model8_7({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool4 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool8_8(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV8/Tool8.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model8_8({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
	//Converte resultado para uma string que pode ser tratada pelo programa
	std::string sResult;
	std::stringstream temp;
	temp << output[0];
	sResult = temp.str();
	
	//Remove o "[[" do inicio da string
	int lenghResult = sizeof(sResult) - 2; 
	char temp2[lenghResult];
	memcpy(temp2, &sResult[2], lenghResult);
	
	//Converte a string com o resultado para float
	double fResult = std::stod(temp2);
	
	//Imprime resultado da análise
	std::cout.precision(2); //Limita casas decimais
	//std::cout << std::fixed << stringTool4 << ":	" << fResult*100 << "%";

	if (fResult >= 0.9) {
		result=1;
		//std::cout << "     Correto\n";
	}
	else {
		result=0;
		//std::cout << "     Errado\n";
	}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int main()
{
	// ========== Memória compartilhada ==========
	int shmidMemoria, shmidImagemCam1, shmidImagemCam2, shmidGavetaVerificacao, shmidResultado;
	char *path="/home/tramontina/Downloads/Gavetas";
	int *contEncoder, *ImagemCam1OK, *ImagemCam2OK, *GavetaVerificacao;
	int ProximoRegistro = 0; //Contagem de pulsos do encoder para próxima captura
	structResultados *Resultado;

	if((shmidMemoria=shmget(ftok(path,(key_t)KEYEncoder),sizeof(int*),IPC_CREAT|SHM_R|SHM_W))==-1){
		printf("\nErro na criacao do segmento de memoria...\n");
		exit(-1);
	}
	contEncoder=(int*)shmat(shmidMemoria,0,0);

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
	*GavetaVerificacao = 0;

	if((shmidResultado=shmget(ftok(path,(key_t)KEYResultado),sizeof(structResultados*),IPC_CREAT|SHM_R|SHM_W))==-1){
		printf("\nErro na criacao do segmento de memoria...\n");
		exit(-1);
	}
	Resultado=(structResultados*)shmat(shmidResultado,0,0);

	//cout << "Redes neurais carregadas" << endl;
	cout << "1" << endl; //Indica que todas as redes estão carregadas

    //Carrega os tensores pela primeira vez, dessa forma as próximas execuções serão muito mais rápidas
    //Verifica se os arquivos existem
    ifstream obj1_1("ToolsGV1/Tool1.png"), obj1_2("ToolsGV1/Tool2.png"), obj1_3("ToolsGV1/Tool3.png"), obj1_4("ToolsGV1/Tool4.png"), obj1_5("ToolsGV1/Tool5.png"), obj1_6("ToolsGV1/Tool6.png"), obj1_7("ToolsGV1/Tool7.png"),
	obj1_8("ToolsGV1/Tool8.png"), obj1_9("ToolsGV1/Tool9.png"), obj1_10("ToolsGV1/Tool10.png"), obj1_11("ToolsGV1/Tool11.png");
    if ((!obj1_1||!obj1_2||!obj1_3||!obj1_4||!obj1_5||!obj1_6||!obj1_7||!obj1_8||!obj1_9||!obj1_10||!obj1_11)==0 && *contEncoder==0){
        //Carrega tensores, para evitar problemas de processamento, encerra o carregamento caso a gaveta se feche durante o processo
		for(int i=1; i<=11; i++){
			std::string name = "ToolsGV1/Tool" + std::to_string(i) + ".png";
			auto input = cppflow::expand_dims(cppflow::cast(cppflow::decode_jpeg(cppflow::read_file(std::string(name))), TF_UINT8, TF_FLOAT), 0);
			if (*GavetaVerificacao!=0) break;
			else if (i==1) auto output = model1_1({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==2) auto output = model1_2({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==3) auto output = model1_3({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==4) auto output = model1_4({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==5) auto output = model1_5({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==6) auto output = model1_6({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==7) auto output = model1_7({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==8) auto output = model1_8({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==9) auto output = model1_9({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==10) auto output = model1_10({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==11) auto output = model1_11({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
		}
    }

	ifstream obj2_1("ToolsGV2/Tool1.png"), obj2_2("ToolsGV2/Tool2.png"), obj2_3("ToolsGV2/Tool3.png"), obj2_4("ToolsGV2/Tool4.png");
    if ((!obj2_1||!obj2_2||!obj2_3||!obj2_4)==0 && *contEncoder==0){
        //Carrega tensores, para evitar problemas de processamento, encerra o carregamento caso a gaveta se feche durante o processo
		for(int i=1; i<=4; i++){
			std::string name = "ToolsGV2/Tool" + std::to_string(i) + ".png";
			auto input = cppflow::expand_dims(cppflow::cast(cppflow::decode_jpeg(cppflow::read_file(std::string(name))), TF_UINT8, TF_FLOAT), 0);
			if (*GavetaVerificacao!=0) break;
			else if (i==1) auto output = model2_1({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==2) auto output = model2_2({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==3) auto output = model2_3({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==4) auto output = model2_4({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
		}
    }

	ifstream obj3_1("ToolsGV3/Tool1.png"), obj3_2("ToolsGV3/Tool2.png"), obj3_3("ToolsGV3/Tool3.png"), obj3_4("ToolsGV3/Tool4.png"), obj3_5("ToolsGV3/Tool5.png"), obj3_6("ToolsGV3/Tool6.png"), obj3_7("ToolsGV3/Tool7.png"),
	obj3_8("ToolsGV3/Tool8.png"), obj3_9("ToolsGV3/Tool9.png"), obj3_10("ToolsGV3/Tool10.png"), obj3_11("ToolsGV3/Tool11.png"), obj3_12("ToolsGV3/Tool12.png");
	if ((!obj3_1||!obj3_2||!obj3_3||!obj3_4||!obj3_5||!obj3_6||!obj3_7||!obj3_8||!obj3_9||!obj3_10||!obj3_11||!obj3_12)==0 && *contEncoder==0){
        //Carrega tensores, para evitar problemas de processamento, encerra o carregamento caso a gaveta se feche durante o processo
		for(int i=1; i<=12; i++){
			std::string name = "ToolsGV3/Tool" + std::to_string(i) + ".png";
			auto input = cppflow::expand_dims(cppflow::cast(cppflow::decode_jpeg(cppflow::read_file(std::string(name))), TF_UINT8, TF_FLOAT), 0);
			if (*GavetaVerificacao!=0) break;
			else if (i==1) auto output = model3_1({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==2) auto output = model3_2({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==3) auto output = model3_3({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==4) auto output = model3_4({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==5) auto output = model3_5({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==6) auto output = model3_6({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==7) auto output = model3_7({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==8) auto output = model3_8({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==9) auto output = model3_9({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==10) auto output = model3_10({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==11) auto output = model3_11({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==12) auto output = model3_12({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
		}
    }

	ifstream obj4_1("ToolsGV4/Tool1.png"), obj4_2("ToolsGV4/Tool2.png"), obj4_3("ToolsGV4/Tool3.png"), obj4_4("ToolsGV4/Tool4.png"), obj4_5("ToolsGV4/Tool5.png"), obj4_6("ToolsGV4/Tool6.png"), obj4_7("ToolsGV4/Tool7.png"),
	obj4_8("ToolsGV4/Tool8.png"), obj4_9("ToolsGV4/Tool9.png"), obj4_10("ToolsGV4/Tool10.png");
    if ((!obj4_1||!obj4_2||!obj4_3||!obj4_4||!obj4_5||!obj4_6||!obj4_7||!obj4_8||!obj4_9||!obj4_10)==0 && *contEncoder==0){
        //Carrega tensores, para evitar problemas de processamento, encerra o carregamento caso a gaveta se feche durante o processo
		for(int i=1; i<=10; i++){
			std::string name = "ToolsGV4/Tool" + std::to_string(i) + ".png";
			auto input = cppflow::expand_dims(cppflow::cast(cppflow::decode_jpeg(cppflow::read_file(std::string(name))), TF_UINT8, TF_FLOAT), 0);
			if (*GavetaVerificacao!=0) break;
			else if (i==1) auto output = model4_1({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==2) auto output = model4_2({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==3) auto output = model4_3({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==4) auto output = model4_4({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==5) auto output = model4_5({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==6) auto output = model4_6({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==7) auto output = model4_7({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==8) auto output = model4_8({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==9) auto output = model4_9({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==10) auto output = model4_10({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
		}
    }
	//cout << "Tensores GV4 carregados" << endl;

	ifstream obj5_1("ToolsGV5/Tool1.png"), obj5_2("ToolsGV5/Tool2.png"), obj5_3("ToolsGV5/Tool3.png"), obj5_4("ToolsGV5/Tool4.png"), obj5_5("ToolsGV5/Tool5.png"), obj5_6("ToolsGV5/Tool6.png"), obj5_7("ToolsGV5/Tool7.png"),
	obj5_8("ToolsGV5/Tool8.png"), obj5_9("ToolsGV5/Tool9.png"), obj5_10("ToolsGV5/Tool10.png");
    if ((!obj5_1||!obj5_2||!obj5_3||!obj5_4||!obj5_5||!obj5_6||!obj5_7||!obj5_8||!obj5_9||!obj5_10)==0 && *contEncoder==0){
        //Carrega tensores, para evitar problemas de processamento, encerra o carregamento caso a gaveta se feche durante o processo
		for(int i=1; i<=10; i++){
			std::string name = "ToolsGV5/Tool" + std::to_string(i) + ".png";
			auto input = cppflow::expand_dims(cppflow::cast(cppflow::decode_jpeg(cppflow::read_file(std::string(name))), TF_UINT8, TF_FLOAT), 0);
			if (*GavetaVerificacao!=0) break;
			else if (i==1) auto output = model5_1({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==2) auto output = model5_2({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==3) auto output = model5_3({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==4) auto output = model5_4({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==5) auto output = model5_5({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==6) auto output = model5_6({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==7) auto output = model5_7({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==8) auto output = model5_8({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==9) auto output = model5_9({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==10) auto output = model5_10({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
		}
    }
	//cout << "Tensores GV5 carregados" << endl;

	ifstream obj6_1("ToolsGV6/Tool1.png"), obj6_2("ToolsGV6/Tool2.png"), obj6_3("ToolsGV6/Tool3.png"), obj6_4("ToolsGV6/Tool4.png"), obj6_5("ToolsGV6/Tool5.png"), obj6_6("ToolsGV6/Tool6.png"), obj6_7("ToolsGV6/Tool7.png"),
	obj6_8("ToolsGV6/Tool8.png"), obj6_9("ToolsGV6/Tool9.png"), obj6_10("ToolsGV6/Tool10.png"), obj6_11("ToolsGV6/Tool11.png"), obj6_12("ToolsGV6/Tool14.png"), obj6_13("ToolsGV6/Tool13.png"), obj6_14("ToolsGV6/Tool14.png"),
	obj6_15("ToolsGV6/Tool15.png"), obj6_16("ToolsGV6/Tool16.png"), obj6_17("ToolsGV6/Tool17.png"), obj6_18("ToolsGV6/Tool18.png"), obj6_19("ToolsGV6/Tool19.png");
    if ((!obj6_1||!obj6_2||!obj6_3||!obj6_4||!obj6_5||!obj6_6||!obj6_7||!obj6_8||!obj6_9||!obj6_10||!obj6_11||!obj6_12||!obj6_13||!obj6_14||!obj6_15||!obj6_16||!obj6_17||!obj6_18||!obj6_19)==0 && *contEncoder==0){
        //Carrega tensores, para evitar problemas de processamento, encerra o carregamento caso a gaveta se feche durante o processo
		for(int i=1; i<=19; i++){
			std::string name = "ToolsGV6/Tool" + std::to_string(i) + ".png";
			auto input = cppflow::expand_dims(cppflow::cast(cppflow::decode_jpeg(cppflow::read_file(std::string(name))), TF_UINT8, TF_FLOAT), 0);
			if (*GavetaVerificacao!=0) break;
			else if (i==1) auto output = model6_1({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==2) auto output = model6_2({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==3) auto output = model6_3({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==4) auto output = model6_4({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==5) auto output = model6_5({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==6) auto output = model6_6({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==7) auto output = model6_7({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==8) auto output = model6_8({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==9) auto output = model6_9({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==10) auto output = model6_10({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==11) auto output = model6_11({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==12) auto output = model6_12({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==13) auto output = model6_13({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==14) auto output = model6_14({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==15) auto output = model6_15({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==16) auto output = model6_16({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==17) auto output = model6_17({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==18) auto output = model6_18({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==19) auto output = model6_19({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
		}
    }
	//cout << "Tensores GV6 carregados" << endl;

	ifstream obj7_1("ToolsGV7/Tool1.png"), obj7_2("ToolsGV7/Tool2.png"), obj7_3("ToolsGV7/Tool3.png"), obj7_4("ToolsGV7/Tool4.png"), obj7_5("ToolsGV7/Tool5.png"), obj7_6("ToolsGV7/Tool6.png"), obj7_7("ToolsGV7/Tool7.png"), 
	obj7_8("ToolsGV7/Tool8.png"), obj7_9("ToolsGV7/Tool9.png");
    if ((!obj7_1||!obj7_2||!obj7_3||!obj7_4||!obj7_5||!obj7_6||!obj7_7||!obj7_8||!obj7_9)==0 && *contEncoder==0){
        //Carrega tensores, para evitar problemas de processamento, encerra o carregamento caso a gaveta se feche durante o processo
		for(int i=1; i<=9; i++){
			std::string name = "ToolsGV7/Tool" + std::to_string(i) + ".png";
			auto input = cppflow::expand_dims(cppflow::cast(cppflow::decode_jpeg(cppflow::read_file(std::string(name))), TF_UINT8, TF_FLOAT), 0);
			if (*GavetaVerificacao!=0) break;
			else if (i==1) auto output = model7_1({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==2) auto output = model7_2({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==3) auto output = model7_3({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==4) auto output = model7_4({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==5) auto output = model7_5({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==6) auto output = model7_6({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==7) auto output = model7_7({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==8) auto output = model7_9({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==9) auto output = model7_8({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
		}
    }
	//cout << "Tensores GV7 carregados" << endl;

	ifstream obj8_1("ToolsGV8/Tool1.png"), obj8_2("ToolsGV8/Tool2.png"), obj8_3("ToolsGV8/Tool3.png"), obj8_4("ToolsGV8/Tool4.png"), obj8_5("ToolsGV8/Tool5.png"),obj8_6("ToolsGV8/Tool6.png"),obj8_7("ToolsGV8/Tool7.png"),
	obj8_8("ToolsGV8/Tool8.png");
    if ((!obj8_1||!obj8_2||!obj8_3||!obj8_4||!obj8_5||!obj8_6||!obj8_7||!obj8_8)==0 && *contEncoder==0){
        //Carrega tensores, para evitar problemas de processamento, encerra o carregamento caso a gaveta se feche durante o processo
		for(int i=1; i<=6; i++){
			std::string name = "ToolsGV8/Tool" + std::to_string(i) + ".png";
			auto input = cppflow::expand_dims(cppflow::cast(cppflow::decode_jpeg(cppflow::read_file(std::string(name))), TF_UINT8, TF_FLOAT), 0);
			if (*GavetaVerificacao!=0) break;
			else if (i==1) auto output = model8_1({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==2) auto output = model8_2({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==3) auto output = model8_3({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==4) auto output = model8_4({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==5) auto output = model8_5({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==6) auto output = model8_6({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==7) auto output = model8_7({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==8) auto output = model8_8({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
		}
    }
	//cout << "Tensores GV8 carregados" << endl;

	//Aguarda finalização da captura das imagens
	//while(*ImagemCam1OK!=1 || *ImagemCam2OK!=1){}

	//*ImagemCam1OK = 0;
	//*ImagemCam2OK = 0;

	//cout << "1" << endl; //Indica que todas as redes estão carregadas

	int nGaveta = 0;
	int contTool = 0;

	while (1){
		//cout << "Entre com o numero da gaveta: " << endl;
		//cin >> *GavetaVerificacao;
		//contTool = 0;
		while(*ImagemCam1OK!=1 || *ImagemCam2OK!=1){}

		if (*GavetaVerificacao==1){
			std::string result = "";
			//cv::Mat frame1, frame2, concat;
			//frame1 = cv::imread("GV1_cam1_resized.png");
			//frame2 = cv::imread("GV1_cam2_resized.png");

			if (RedeTool1_1()){
				//cv::rectangle(frame1, cv::Point(2*5, 2*68), cv::Point(2*77, 2*400), cv::Scalar(0, 255, 0, 0), 2, LINE_4, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(90, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame1, cv::Point(2*5, 2*68), cv::Point(2*77, 2*400), cv::Scalar(0, 0, 255, 0), 2, LINE_4, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(90, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool1_2()){
				//cv::rectangle(frame1, cv::Point(2*39, 2*422), cv::Point(2*113, 2*750), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(360, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame1, cv::Point(2*39, 2*422), cv::Point(2*113, 2*750), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(360, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool1_3()){
				//cv::rectangle(frame1, cv::Point(2*102, 2*80), cv::Point(2*178, 2*512), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(620, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame1, cv::Point(2*102, 2*80), cv::Point(2*178, 2*512), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(620, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool1_4()){
				//cv::rectangle(frame1, cv::Point(2*160, 2*326), cv::Point(2*236, 2*750), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(885, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame1, cv::Point(2*160, 2*326), cv::Point(2*236, 2*750), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(885, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool1_5()){
				//cv::rectangle(frame1, cv::Point(2*230, 2*84), cv::Point(2*314, 2*492), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame1, cv::Point(2*230, 2*84), cv::Point(2*314, 2*492), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool1_6()){
				//cv::rectangle(frame1, cv::Point(2*305, 2*250), cv::Point(2*389, 2*750), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame1, cv::Point(2*305, 2*250), cv::Point(2*389, 2*750), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool1_7()){
				//cv::rectangle(frame1, cv::Point(2*382, 2*74), cv::Point(2*470, 2*606), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame1, cv::Point(2*382, 2*74), cv::Point(2*470, 2*606), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool1_8()){
				//cv::rectangle(frame1, cv::Point(2*450, 2*66), cv::Point(2*530, 2*750), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame1, cv::Point(2*450, 2*66), cv::Point(2*530, 2*750), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool1_9()){
				//cv::rectangle(frame1, cv::Point(2*516, 2*62), cv::Point(2*580, 2*446), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame1, cv::Point(2*516, 2*62), cv::Point(2*580, 2*446), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool1_10()){
				//cv::rectangle(frame2, cv::Point(2*0, 2*338), cv::Point(2*56, 2*738), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame2, cv::Point(2*0, 2*338), cv::Point(2*56, 2*738), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool1_11()){
				//cv::rectangle(frame2, cv::Point(2*42, 2*54), cv::Point(2*102, 2*534), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame2, cv::Point(2*42, 2*54), cv::Point(2*102, 2*534), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}

			Resultado->saida[contTool] = '\0';
			contTool = 0;
			*GavetaVerificacao = 0;
			//cout << result << endl;
			//imwrite("frame1.png", frame1);
			//imwrite("frame2.png", frame2);
		}
		else if (*GavetaVerificacao==2){
			std::string result = "";
			//cv::Mat frame1, frame2, concat;
			//frame1 = cv::imread("GV2_cam1_resized.png");
			//frame2 = cv::imread("GV2_cam2_resized.png");

			if (RedeTool2_1()){
				//cv::rectangle(frame1, cv::Point(2*0, 2*282), cv::Point(2*232, 2*406), cv::Scalar(0, 255, 0, 0), 2, LINE_4, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(90, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame1, cv::Point(2*0, 2*282), cv::Point(2*232, 2*406), cv::Scalar(0, 0, 255, 0), 2, LINE_4, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(90, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool2_2()){
				//cv::rectangle(frame1, cv::Point(2*12, 2*234), cv::Point(2*272, 2*380), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(360, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame1, cv::Point(2*12, 2*234), cv::Point(2*272, 2*380), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(360, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool2_3()){
				//cv::rectangle(frame1, cv::Point(2*22, 2*184), cv::Point(2*312, 2*336), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(620, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame1, cv::Point(2*22, 2*184), cv::Point(2*312, 2*336), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(620, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool2_4()){
				//cv::rectangle(frame1, cv::Point(2*54, 2*136), cv::Point(2*354, 2*292), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(885, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame1, cv::Point(2*54, 2*136), cv::Point(2*354, 2*292), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(885, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}

			Resultado->saida[contTool] = '\0';
			contTool = 0;
			*GavetaVerificacao = 0;
			//cout << result << endl;
			//imwrite("frame1.png", frame1);
			//imwrite("frame2.png", frame2);
		}
		else if (*GavetaVerificacao==3){
			std::string result = "";
			//cv::Mat frame1, frame2, concat;
			//frame1 = cv::imread("GV3_cam1_resized.png");
			//frame2 = cv::imread("GV3_cam2_resized.png");

			if (RedeTool3_1()){
				//cv::rectangle(frame1, cv::Point(2*18, 2*24), cv::Point(2*56, 2*200), cv::Scalar(0, 255, 0, 0), 2, LINE_4, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(90, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame1, cv::Point(2*18, 2*24), cv::Point(2*56, 2*200), cv::Scalar(0, 0, 255, 0), 2, LINE_4, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(90, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool3_2()){
				//cv::rectangle(frame1, cv::Point(2*74, 2*24), cv::Point(2*114, 2*200), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(360, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame1, cv::Point(2*74, 2*24), cv::Point(2*114, 2*200), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(360, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool3_3()){
				//cv::rectangle(frame1, cv::Point(2*132, 2*24), cv::Point(2*174, 2*200), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(620, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame1, cv::Point(2*132, 2*24), cv::Point(2*174, 2*200), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(620, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool3_4()){
				//cv::rectangle(frame1, cv::Point(2*190, 2*24), cv::Point(2*232, 2*200), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(885, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame1, cv::Point(2*190, 2*24), cv::Point(2*232, 2*200), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(885, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool3_5()){
				//cv::rectangle(frame1, cv::Point(2*246, 2*24), cv::Point(2*286, 2*200), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame1, cv::Point(2*246, 2*24), cv::Point(2*286, 2*200), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool3_6()){
				//cv::rectangle(frame1, cv::Point(2*6, 2*200), cv::Point(2*44, 2*320), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame1, cv::Point(2*6, 2*200), cv::Point(2*44, 2*320), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool3_7()){
				//cv::rectangle(frame1, cv::Point(2*44, 2*200), cv::Point(2*80, 2*320), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame1, cv::Point(2*44, 2*200), cv::Point(2*80, 2*320), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool3_8()){
				//cv::rectangle(frame1, cv::Point(2*80, 2*200), cv::Point(2*118, 2*320), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame1, cv::Point(2*80, 2*200), cv::Point(2*118, 2*320), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool3_9()){
				//cv::rectangle(frame1, cv::Point(2*118, 2*200), cv::Point(2*162, 2*320), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame1, cv::Point(2*118, 2*200), cv::Point(2*162, 2*320), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool3_10()){
				//cv::rectangle(frame1, cv::Point(2*162, 2*200), cv::Point(2*202, 2*320), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame1, cv::Point(2*162, 2*200), cv::Point(2*202, 2*320), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool3_11()){
				//cv::rectangle(frame1, cv::Point(2*202, 2*200), cv::Point(2*240, 2*320), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame1, cv::Point(2*202, 2*200), cv::Point(2*240, 2*320), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool3_12()){
				//cv::rectangle(frame1, cv::Point(2*240, 2*200), cv::Point(2*278, 2*320), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame1, cv::Point(2*240, 2*200), cv::Point(2*278, 2*320), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			Resultado->saida[contTool] = '\0';
			contTool = 0;
			*GavetaVerificacao = 0;
			//cout << result << endl;
			//imwrite("frame1.png", frame1);
			//imwrite("frame2.png", frame2);
		}
		else if (*GavetaVerificacao==4){
			std::string result = "";
			//cv::Mat frame1, frame2, concat;
			//frame1 = cv::imread("GV4_cam1_resized.png");
			//frame2 = cv::imread("GV4_cam2_resized.png");

			if (RedeTool4_1()){
				//cv::rectangle(frame1, cv::Point(2*0, 2*19), cv::Point(2*104, 2*261), cv::Scalar(0, 255, 0, 0), 2, LINE_4, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(90, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame1, cv::Point(2*0, 2*19), cv::Point(2*104, 2*261), cv::Scalar(0, 0, 255, 0), 2, LINE_4, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(90, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool4_2()){
				//cv::rectangle(frame1, cv::Point(2*104, 2*21), cv::Point(2*174, 2*265), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(360, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame1, cv::Point(2*104, 2*21), cv::Point(2*174, 2*265), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(360, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool4_3()){
				//cv::rectangle(frame1, cv::Point(2*181, 2*20), cv::Point(2*263, 2*174), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(620, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame1, cv::Point(2*181, 2*20), cv::Point(2*263, 2*174), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(620, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool4_4()){
				//cv::rectangle(frame1, cv::Point(2*262, 2*17), cv::Point(2*316, 2*175), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(885, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame1, cv::Point(2*262, 2*17), cv::Point(2*316, 2*175), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(885, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool4_5()){
				//cv::rectangle(frame1, cv::Point(2*55, 2*238), cv::Point(2*145, 2*390), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame1, cv::Point(2*55, 2*238), cv::Point(2*145, 2*390), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool4_6()){
				//cv::rectangle(frame1, cv::Point(2*147, 2*154), cv::Point(2*215, 2*390), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame1, cv::Point(2*147, 2*154), cv::Point(2*215, 2*390), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool4_7()){
				//cv::rectangle(frame1, cv::Point(2*216, 2*150), cv::Point(2*316, 2*390), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame1, cv::Point(2*216, 2*150), cv::Point(2*316, 2*390), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool4_8()){
				//cv::rectangle(frame2, cv::Point(2*4, 2*18), cv::Point(2*68, 2*228), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame2, cv::Point(2*4, 2*18), cv::Point(2*68, 2*228), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool4_9()){
				//cv::rectangle(frame2, cv::Point(2*68, 2*18), cv::Point(2*136, 2*254), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame2, cv::Point(2*68, 2*18), cv::Point(2*136, 2*254), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool4_10()){
				//cv::rectangle(frame2, cv::Point(2*134, 2*20), cv::Point(2*214, 2*276), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame2, cv::Point(2*134, 2*20), cv::Point(2*214, 2*276), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			Resultado->saida[contTool] = '\0';
			contTool = 0;
			*GavetaVerificacao = 0;
			//cout << result << endl;
			//imwrite("frame1.png", frame1);
			//imwrite("frame2.png", frame2);
		}
		else if (*GavetaVerificacao==5){
			std::string result = "";
			//cv::Mat frame1, frame2, concat;
			//frame1 = cv::imread("GV5_cam1_resized.png");
			//frame2 = cv::imread("GV5_cam2_resized.png");

			if (RedeTool5_1()){
				//cv::rectangle(frame1, cv::Point(2*12, 2*12), cv::Point(2*46, 2*238), cv::Scalar(0, 255, 0, 0), 2, LINE_4, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(90, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame1, cv::Point(2*12, 2*12), cv::Point(2*46, 2*238), cv::Scalar(0, 0, 255, 0), 2, LINE_4, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(90, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool5_2()){
				//cv::rectangle(frame1, cv::Point(2*45, 2*12), cv::Point(2*77, 2*200), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(360, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame1, cv::Point(2*45, 2*12), cv::Point(2*77, 2*200), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(360, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool5_3()){
				//cv::rectangle(frame1, cv::Point(2*76, 2*12), cv::Point(2*102, 2*172), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(620, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame1, cv::Point(2*76, 2*12), cv::Point(2*102, 2*172), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(620, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool5_4()){
				//cv::rectangle(frame1, cv::Point(2*102, 2*12), cv::Point(2*122, 2*146), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(885, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame1, cv::Point(2*102, 2*12), cv::Point(2*122, 2*146), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(885, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool5_5()){
				//cv::rectangle(frame1, cv::Point(2*126, 2*12), cv::Point(2*148, 2*156), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame1, cv::Point(2*126, 2*12), cv::Point(2*148, 2*156), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool5_6()){
				//cv::rectangle(frame1, cv::Point(2*148, 2*12), cv::Point(2*168, 2*156), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame1, cv::Point(2*148, 2*12), cv::Point(2*168, 2*156), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool5_7()){
				//cv::rectangle(frame1, cv::Point(2*168, 2*10), cv::Point(2*188, 2*156), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame1, cv::Point(2*168, 2*10), cv::Point(2*188, 2*156), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool5_8()){
				//cv::rectangle(frame1, cv::Point(2*168, 2*10), cv::Point(2*188, 2*156), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame1, cv::Point(2*168, 2*10), cv::Point(2*188, 2*156), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool5_9()){
				//cv::rectangle(frame1, cv::Point(2*168, 2*10), cv::Point(2*188, 2*156), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame1, cv::Point(2*168, 2*10), cv::Point(2*188, 2*156), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool5_10()){
				//cv::rectangle(frame1, cv::Point(2*168, 2*10), cv::Point(2*188, 2*156), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame1, cv::Point(2*168, 2*10), cv::Point(2*188, 2*156), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}

			Resultado->saida[contTool] = '\0';
			contTool = 0;
			*GavetaVerificacao = 0;
			//cout << result << endl;
			//imwrite("frame1.png", frame1);
			//imwrite("frame2.png", frame2);
		}
		else if (*GavetaVerificacao==6){
			std::string result = "";
			//cv::Mat frame1, frame2, concat;
			//frame1 = cv::imread("GV6_cam1_resized.png");
			//frame2 = cv::imread("GV6_cam2_resized.png");

			if (RedeTool6_1()){
				//cv::rectangle(frame1, cv::Point(2*0, 2*12), cv::Point(2*66, 2*224), cv::Scalar(0, 255, 0, 0), 2, LINE_4, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(90, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame1, cv::Point(2*0, 2*12), cv::Point(2*66, 2*224), cv::Scalar(0, 0, 255, 0), 2, LINE_4, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(90, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool6_2()){
				//cv::rectangle(frame1, cv::Point(2*58, 2*12), cv::Point(2*134, 2*196), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(360, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame1, cv::Point(2*58, 2*12), cv::Point(2*134, 2*196), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(360, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool6_3()){
				//cv::rectangle(frame1, cv::Point(2*134, 2*12), cv::Point(2*188, 2*224), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(620, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame1, cv::Point(2*134, 2*12), cv::Point(2*188, 2*224), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(620, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool6_4()){
				//cv::rectangle(frame2, cv::Point(2*90, 2*12), cv::Point(2*174, 2*184), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(885, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame2, cv::Point(2*90, 2*12), cv::Point(2*174, 2*184), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(885, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool6_5()){
				//cv::rectangle(frame2, cv::Point(2*154, 2*12), cv::Point(2*266, 2*234), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame2, cv::Point(2*154, 2*12), cv::Point(2*266, 2*234), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool6_6()){
				//cv::rectangle(frame2, cv::Point(2*268, 2*12), cv::Point(2*340, 2*236), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame2, cv::Point(2*268, 2*12), cv::Point(2*340, 2*236), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool6_7()){
				//cv::rectangle(frame2, cv::Point(2*0, 2*230), cv::Point(2*280, 2*266), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame2, cv::Point(2*0, 2*230), cv::Point(2*280, 2*266), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool6_8()){
				//cv::rectangle(frame2, cv::Point(2*0, 2*230), cv::Point(2*280, 2*266), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame2, cv::Point(2*0, 2*230), cv::Point(2*280, 2*266), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool6_9()){
				//cv::rectangle(frame2, cv::Point(2*0, 2*230), cv::Point(2*280, 2*266), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame2, cv::Point(2*0, 2*230), cv::Point(2*280, 2*266), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool6_10()){
				//cv::rectangle(frame2, cv::Point(2*0, 2*230), cv::Point(2*280, 2*266), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame2, cv::Point(2*0, 2*230), cv::Point(2*280, 2*266), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool6_11()){
				//cv::rectangle(frame2, cv::Point(2*0, 2*230), cv::Point(2*280, 2*266), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame2, cv::Point(2*0, 2*230), cv::Point(2*280, 2*266), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool6_12()){
				//cv::rectangle(frame2, cv::Point(2*0, 2*230), cv::Point(2*280, 2*266), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame2, cv::Point(2*0, 2*230), cv::Point(2*280, 2*266), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool6_13()){
				//cv::rectangle(frame2, cv::Point(2*0, 2*230), cv::Point(2*280, 2*266), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame2, cv::Point(2*0, 2*230), cv::Point(2*280, 2*266), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool6_14()){
				//cv::rectangle(frame2, cv::Point(2*0, 2*230), cv::Point(2*280, 2*266), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame2, cv::Point(2*0, 2*230), cv::Point(2*280, 2*266), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool6_15()){
				//cv::rectangle(frame2, cv::Point(2*0, 2*230), cv::Point(2*280, 2*266), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame2, cv::Point(2*0, 2*230), cv::Point(2*280, 2*266), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool6_16()){
				//cv::rectangle(frame2, cv::Point(2*0, 2*230), cv::Point(2*280, 2*266), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame2, cv::Point(2*0, 2*230), cv::Point(2*280, 2*266), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool6_17()){
				//cv::rectangle(frame2, cv::Point(2*0, 2*230), cv::Point(2*280, 2*266), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame2, cv::Point(2*0, 2*230), cv::Point(2*280, 2*266), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool6_18()){
				//cv::rectangle(frame2, cv::Point(2*0, 2*230), cv::Point(2*280, 2*266), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame2, cv::Point(2*0, 2*230), cv::Point(2*280, 2*266), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool6_19()){
				//cv::rectangle(frame2, cv::Point(2*0, 2*230), cv::Point(2*280, 2*266), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame2, cv::Point(2*0, 2*230), cv::Point(2*280, 2*266), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}

			Resultado->saida[contTool] = '\0';
			contTool = 0;
			*GavetaVerificacao = 0;
			//cout << result << endl;
			//imwrite("frame1.png", frame1);
			//imwrite("frame2.png", frame2);
		}
		else if (*GavetaVerificacao==7){
			std::string result = "";
			//cv::Mat frame1, frame2, concat;
			//frame1 = cv::imread("GV7_cam1_resized.png");
			//frame2 = cv::imread("GV7_cam2_resized.png");

			if (RedeTool7_1()){
				//cv::rectangle(frame1, cv::Point(2*0, 2*44), cv::Point(2*40, 2*240), cv::Scalar(0, 255, 0, 0), 2, LINE_4, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(90, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame1, cv::Point(2*0, 2*44), cv::Point(2*40, 2*240), cv::Scalar(0, 0, 255, 0), 2, LINE_4, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(90, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool7_2()){
				//cv::rectangle(frame1, cv::Point(2*28, 2*14), cv::Point(2*66, 2*210), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(360, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame1, cv::Point(2*28, 2*14), cv::Point(2*66, 2*210), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(360, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool7_3()){
				//cv::rectangle(frame1, cv::Point(2*56, 2*44), cv::Point(2*94, 2*232), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(620, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame1, cv::Point(2*56, 2*44), cv::Point(2*94, 2*232), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(620, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool7_4()){
				//cv::rectangle(frame1, cv::Point(2*83, 2*14), cv::Point(2*121, 2*194), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(885, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame1, cv::Point(2*83, 2*14), cv::Point(2*121, 2*194), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(885, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool7_5()){
				//cv::rectangle(frame1, cv::Point(2*110, 2*39), cv::Point(2*144, 2*209), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame1, cv::Point(2*110, 2*39), cv::Point(2*144, 2*209), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool7_6()){
				//cv::rectangle(frame1, cv::Point(2*135, 2*12), cv::Point(2*169, 2*176), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame1, cv::Point(2*135, 2*12), cv::Point(2*169, 2*176), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool7_7()){
				//cv::rectangle(frame1, cv::Point(2*135, 2*12), cv::Point(2*169, 2*176), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame1, cv::Point(2*135, 2*12), cv::Point(2*169, 2*176), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool7_8()){
				//cv::rectangle(frame1, cv::Point(2*135, 2*12), cv::Point(2*169, 2*176), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame1, cv::Point(2*135, 2*12), cv::Point(2*169, 2*176), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool7_9()){
				//cv::rectangle(frame1, cv::Point(2*135, 2*12), cv::Point(2*169, 2*176), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame1, cv::Point(2*135, 2*12), cv::Point(2*169, 2*176), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}

			Resultado->saida[contTool] = '\0';
			contTool = 0;
			*GavetaVerificacao = 0;
			//cout << result << endl;
			//imwrite("frame1.png", frame1);
			//imwrite("frame2.png", frame2);
		}
		else if (*GavetaVerificacao==8){
			std::string result = "";
			//cv::Mat frame1, frame2, concat;
			//frame1 = cv::imread("GV8_cam1_resized.png");
			//frame2 = cv::imread("GV8_cam2_resized.png");

			if (RedeTool8_1()){
				//cv::rectangle(frame1, cv::Point(2*28, 2*12), cv::Point(2*54, 2*174), cv::Scalar(0, 255, 0, 0), 2, LINE_4, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(90, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame1, cv::Point(2*28, 2*12), cv::Point(2*54, 2*174), cv::Scalar(0, 0, 255, 0), 2, LINE_4, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(90, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool8_2()){
				//cv::rectangle(frame1, cv::Point(2*58, 2*12), cv::Point(2*84, 2*174), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(360, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame1, cv::Point(2*58, 2*12), cv::Point(2*84, 2*174), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(360, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool8_3()){
				//cv::rectangle(frame1, cv::Point(2*84, 2*12), cv::Point(2*128, 2*164), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(620, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame1, cv::Point(2*84, 2*12), cv::Point(2*128, 2*164), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(620, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool8_4()){
				//cv::rectangle(frame2, cv::Point(2*80, 2*9), cv::Point(2*320, 2*107), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(885, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame2, cv::Point(2*80, 2*9), cv::Point(2*320, 2*107), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(885, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool8_5()){
				//cv::rectangle(frame2, cv::Point(2*80, 2*9), cv::Point(2*320, 2*107), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(885, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame2, cv::Point(2*80, 2*9), cv::Point(2*320, 2*107), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(885, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool8_6()){
				//cv::rectangle(frame2, cv::Point(2*80, 2*9), cv::Point(2*320, 2*107), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(885, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame2, cv::Point(2*80, 2*9), cv::Point(2*320, 2*107), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(885, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool8_7()){
				//cv::rectangle(frame2, cv::Point(2*80, 2*9), cv::Point(2*320, 2*107), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(885, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame2, cv::Point(2*80, 2*9), cv::Point(2*320, 2*107), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(885, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}
			
			if (RedeTool8_8()){
				//cv::rectangle(frame2, cv::Point(2*80, 2*9), cv::Point(2*320, 2*107), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Correto", cv::Point(885, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
				result = result + "1";
				Resultado->saida[contTool] = '1';
				contTool++;
			}
			else{
				//cv::rectangle(frame2, cv::Point(2*80, 2*9), cv::Point(2*320, 2*107), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
				//cv::putText(imageFV2, "Errado", cv::Point(885, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
				result = result + "0";
				Resultado->saida[contTool] = '0';
				contTool++;
			}

			Resultado->saida[contTool] = '\0';
			contTool = 0;
			*GavetaVerificacao = 0;
			//cout << result << endl;
			//imwrite("frame1.png", frame1);
			//imwrite("frame2.png", frame2);		
		}
	}
	
	// ========== Fecha todos os dispositivos e janelas ==========
	cv::destroyAllWindows();
	
    return 0;
}