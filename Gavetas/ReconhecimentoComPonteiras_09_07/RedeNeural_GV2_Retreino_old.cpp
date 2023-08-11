#include <iostream>
#include <ueye.h>
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

HIDS hCam = 0;      // 0 for the next available camera. 1-254 to access by ID
SENSORINFO sInfo;
HWND hWndDisplay;
SENSORSCALERINFO sScaleInfo;

char* pcImageMemory;
int DisplayWidth, DisplayHeight;
double newFPS;

#define KEYEncoder 9902 //Endereço de memória compartilhada

string stringTool1 = "Chave fenda 5x38mm";
string stringTool2 = "Chave fenda 5x38mm";
string stringTool3 = "Chave philips 5x38mm";
string stringTool4 = "Chave philips 3x100mm";
string stringTool5 = "Chave fenda 5x100mm";
string stringTool6 = "Chave philips 5x100mm";
string stringTool7 = "Chave philips 5x125mm";
string stringTool8 = "Chave philips 6x100mm";
string stringTool9 = "Chave philips 6x125mm";
string stringPont9 = "Pont. philips 6x125mm";
string stringTool10 = "Chave fenda 6x150mm";
string stringPont10 = "Pont. fenda 6x150mm";
string stringTool11 = "Chave philips 6x150mm";
string stringPont11 = "Pont. philips 6x150mm";
string stringTool12 = "Chave fenda 8x150mm";
string stringPont12 = "Pont. fenda 8x150mm";
string stringTool13 = "Chave philips 8x150mm";
string stringPont13 = "Pont. philips 8x150mm";

cppflow::model model1("RedesGV2/model_1");
cppflow::model model2("RedesGV2/model_2");
cppflow::model model3("RedesGV2/model_3");
cppflow::model model4("RedesGV2/model_4");
cppflow::model model5("RedesGV2/model_5");
cppflow::model model6("RedesGV2/model_6");
cppflow::model model7("RedesGV2/model_7");
cppflow::model model8("RedesGV2/model_8");
cppflow::model model9("RedesGV2/model_9");
cppflow::model model10("RedesGV2/model_10");
cppflow::model model11("RedesGV2/model_11");
cppflow::model model12("RedesGV2/model_12");
cppflow::model model13("RedesGV2/model_13");

cppflow::model model_pont9("RedesPonteirasGV2/model_pont_9");
cppflow::model model_pont10("RedesPonteirasGV2/model_pont_10");
cppflow::model model_pont11("RedesPonteirasGV2/model_pont_11");
cppflow::model model_pont12("RedesPonteirasGV2/model_pont_12");
cppflow::model model_pont13("RedesPonteirasGV2/model_pont_13");

int RedeTool1(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "Tool1.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model1({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
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
	std::cout << std::fixed << stringTool1 << ":	" << fResult*100 << "%";
	
	if (fResult >= 0.95) {
		result=1;
		std::cout << "     Correto\n";}
	else {
		result=0;
		std::cout << "     Errado\n";}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool2(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "Tool2.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
    
    //Aplica rede neural para análise da imagem
	auto output = model2({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
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
	std::cout << std::fixed << stringTool2 << ":	" << fResult*100 << "%";
	
	if (fResult >= 0.95) {
		result=1;
		std::cout << "     Correto\n";}
	else {
		result=0;
		std::cout << "     Errado\n";}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool3(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "Tool3.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model3({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
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
	std::cout << std::fixed << stringTool3 << ":	" << fResult*100 << "%";

	if (fResult >= 0.95) {
		result=1;
		std::cout << "     Correto\n";}
	else {
		result=0;
		std::cout << "     Errado\n";}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool4(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "Tool4.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model4({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
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
	std::cout << std::fixed << stringTool4 << ":	" << fResult*100 << "%";

	if (fResult >= 0.95) {
		result=1;
		std::cout << "     Correto\n";}
	else {
		result=0;
		std::cout << "     Errado\n";}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool5(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "Tool5.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model5({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
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
	std::cout << std::fixed << stringTool5 << ":	" << fResult*100 << "%";

	if (fResult >= 0.95) {
		result=1;
		std::cout << "     Correto\n";}
	else {
		result=0;
		std::cout << "     Errado\n";}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool6(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "Tool6.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model6({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
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
	std::cout << std::fixed << stringTool6 << ":	" << fResult*100 << "%";

	if (fResult >= 0.95) {
		result=1;
		std::cout << "     Correto\n";}
	else {
		result=0;
		std::cout << "     Errado\n";}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool7(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "Tool7.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model7({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
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
	std::cout << std::fixed << stringTool7 << ":	" << fResult*100 << "%";

	if (fResult >= 0.95) {
		result=1;
		std::cout << "     Correto\n";}
	else {
		result=0;
		std::cout << "     Errado\n";}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool8(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "Tool8.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model8({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
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
	std::cout << std::fixed << stringTool8 << ":	" << fResult*100 << "%";

	if (fResult >= 0.95) {
		result=1;
		std::cout << "     Correto\n";}
	else {
		result=0;
		std::cout << "     Errado\n";}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool9(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "Tool9.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model9({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
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
	std::cout << std::fixed << stringTool9 << ":	" << fResult*100 << "%";

	if (fResult >= 0.95) {
		result=1;
		std::cout << "     Correto\n";}
	else {
		result=0;
		std::cout << "     Errado\n";}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedePont9(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "Pont9.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model_pont9({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
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
	std::cout << std::fixed << stringPont9 << ":	" << fResult*100 << "%";

	if (fResult >= 0.95) {
		result=1;
		std::cout << "     Correto\n";}
	else {
		result=0;
		std::cout << "     Errado\n";}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool10(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "Tool10.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model10({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
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
	std::cout << std::fixed << stringTool10 << ":	" << fResult*100 << "%";

	if (fResult >= 0.95) {
		result=1;
		std::cout << "     Correto\n";}
	else {
		result=0;
		std::cout << "     Errado\n";}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedePont10(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "Pont10.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model_pont10({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
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
	std::cout << std::fixed << stringPont10 << ":	" << fResult*100 << "%";

	if (fResult >= 0.95) {
		result=1;
		std::cout << "     Correto\n";}
	else {
		result=0;
		std::cout << "     Errado\n";}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool11(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "Tool11.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model11({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
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
	std::cout << std::fixed << stringTool11 << ":	" << fResult*100 << "%";

	if (fResult >= 0.95) {
		result=1;
		std::cout << "     Correto\n";}
	else {
		result=0;
		std::cout << "     Errado\n";}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedePont11(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "Pont11.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model_pont11({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
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
	std::cout << std::fixed << stringPont11 << ":	" << fResult*100 << "%";

	if (fResult >= 0.95) {
		result=1;
		std::cout << "     Correto\n";}
	else {
		result=0;
		std::cout << "     Errado\n";}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool12(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "Tool12.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model12({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
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
	std::cout << std::fixed << stringTool12 << ":	" << fResult*100 << "%";

	if (fResult >= 0.95) {
		result=1;
		std::cout << "     Correto\n";}
	else {
		result=0;
		std::cout << "     Errado\n";}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedePont12(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "Pont12.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model_pont12({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
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
	std::cout << std::fixed << stringPont12 << ":	" << fResult*100 << "%";

	if (fResult >= 0.95) {
		result=1;
		std::cout << "     Correto\n";}
	else {
		result=0;
		std::cout << "     Errado\n";}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedeTool13(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "Tool13.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model13({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
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
	std::cout << std::fixed << stringTool13 << ":	" << fResult*100 << "%";

	if (fResult >= 0.95) {
		result=1;
		std::cout << "     Correto\n";}
	else {
		result=0;
		std::cout << "     Errado\n";}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int RedePont13(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "Pont13.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model_pont13({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
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
	std::cout << std::fixed << stringPont13 << ":	" << fResult*100 << "%";

	if (fResult >= 0.95) {
		result=1;
		std::cout << "     Correto\n";}
	else {
		result=0;
		std::cout << "     Errado\n";}
		
	//std::cout << "Matriz: " << output[0] << "\n\n";
	
	return result;
}

int main()
{
	// ========== Memória compartilhada ==========
	int shmidMemoria;
	char *path="/home/tramontina/Downloads/Gavetas";
	int *contEncoder;
	int ProximoRegistro = 0; //Contagem de pulsos do encoder para próxima captura
	
	if((shmidMemoria=shmget(ftok(path,(key_t)KEYEncoder),sizeof(int*),IPC_CREAT|SHM_R|SHM_W))==-1){
		printf("\nErro na criacao do segmento de memoria...\n");
		exit(-1);
	}
	contEncoder=(int*)shmat(shmidMemoria,0,0);
	*contEncoder = 0;

	cout << "Sistema carregado" << endl;

	int cont9=0;
	int cont10=0;
	int cont11=0;
	int cont12=0;
	int cont13=0;
	std::string name;
	cv::Mat frame;

	while(1){
		//Processa imagens registradas na rede neural
		system("clear");
		
		/*RedeTool1();
		RedeTool2();
		RedeTool3();
		RedeTool4();
		RedeTool5();
		RedeTool6();
		RedeTool7();
		RedeTool8();*/
		RedeTool9();
		if(RedePont9()==0){
			frame = cv::imread("Pont9.png");
			name = "Retreino/9/" + std::to_string(cont9) + ".png";
			imwrite(name, frame);
			cont9++;
		}
		cout << endl;
		RedeTool10();
		if(RedePont10()==0){
			frame = cv::imread("Pont10.png");
			name = "Retreino/10/" + std::to_string(cont10) + ".png";
			imwrite(name, frame);
			cont10++;
		}
		cout << endl;
		RedeTool11();
		if(RedePont11()==0){
			frame = cv::imread("Pont11.png");
			name = "Retreino/11/" + std::to_string(cont11) + ".png";
			imwrite(name, frame);
			cont11++;
		}
		cout << endl;
		RedeTool12();
		if(RedePont12()==0){
			frame = cv::imread("Pont12.png");
			name = "Retreino/12/" + std::to_string(cont12) + ".png";
			imwrite(name, frame);
			cont12++;
		}
		cout << endl;
		RedeTool13();
		if(RedePont13()==0){
			frame = cv::imread("Pont13.png");
			name = "Retreino/13/" + std::to_string(cont13) + ".png";
			imwrite(name, frame);
			cont13++;
		}
		cout << endl;

		cin.get();
	}
	/*
	Mat imageFV2 = imageF.clone();
	
	if ((RedeTool1() + RedePont1())==2){
		cv::rectangle(imageFV2, cv::Point(2*90, 2*210), cv::Point(2*270, 2*1460), cv::Scalar(0, 255, 0, 0), 4, LINE_8, 1);
		cv::putText(imageFV2, "Correto", cv::Point(90, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
	}
	else{
		cv::rectangle(imageFV2, cv::Point(2*90, 2*210), cv::Point(2*270, 2*1460), cv::Scalar(0, 0, 255, 0), 4, LINE_8, 1);
		cv::putText(imageFV2, "Errado", cv::Point(90, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
	}
			
	if ((RedeTool2() + RedePont2())==2){
		cv::rectangle(imageFV2, cv::Point(2*360, 2*210), cv::Point(2*540, 2*1460), cv::Scalar(0, 255, 0, 0), 4, LINE_8, 1);
		cv::putText(imageFV2, "Correto", cv::Point(360, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
	}
	else{
		cv::rectangle(imageFV2, cv::Point(2*360, 2*210), cv::Point(2*540, 2*1460), cv::Scalar(0, 0, 255, 0), 4, LINE_8, 1);
		cv::putText(imageFV2, "Errado", cv::Point(360, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
	}
	
	if ((RedeTool3() + RedePont3())==2){
		cv::rectangle(imageFV2, cv::Point(2*620, 2*210), cv::Point(2*800, 2*1460), cv::Scalar(0, 255, 0, 0), 4, LINE_8, 1);
		cv::putText(imageFV2, "Correto", cv::Point(620, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
	}
	else{
		cv::rectangle(imageFV2, cv::Point(2*620, 2*210), cv::Point(2*800, 2*1460), cv::Scalar(0, 0, 255, 0), 4, LINE_8, 1);
		cv::putText(imageFV2, "Errado", cv::Point(620, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
	}
	
	if ((RedeTool4() + RedePont4())==2){
		cv::rectangle(imageFV2, cv::Point(2*885, 2*210), cv::Point(2*1065, 2*1460), cv::Scalar(0, 255, 0, 0), 4, LINE_8, 1);
		cv::putText(imageFV2, "Correto", cv::Point(885, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
	}
	else{
		cv::rectangle(imageFV2, cv::Point(2*885, 2*210), cv::Point(2*1065, 2*1460), cv::Scalar(0, 0, 255, 0), 4, LINE_8, 1);
		cv::putText(imageFV2, "Errado", cv::Point(885, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
	}
	
	if ((RedeTool5() + RedePont5())==2){
		cv::rectangle(imageFV2, cv::Point(2*1140, 2*186), cv::Point(2*1340, 2*1460), cv::Scalar(0, 255, 0, 0), 4, LINE_8, 1);
		cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
	}
	else{
		cv::rectangle(imageFV2, cv::Point(2*1140, 2*186), cv::Point(2*1340, 2*1460), cv::Scalar(0, 0, 255, 0), 4, LINE_8, 1);
		cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
	}

	imwrite("public/outputs/Final.jpg", imageFV2);
	//cv::imshow("Image",imageFV2);
	//cv::waitKey();
	//cv::waitKey();
	*/
	
	// ========== Fecha todos os dispositivos e janelas ==========
	cv::destroyAllWindows();

	cout << "Rede finalizada" << endl;
	
    return 0;
}