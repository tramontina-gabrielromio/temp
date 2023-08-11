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
string stringPont1 = "Pont. fenda 5x38mm";
string stringTool2 = "Chave fenda 5x38mm";
string stringPont2 = "Pont. fenda 5x38mm";
string stringTool3 = "Chave philips 5x38mm";
string stringPont3 = "Pont. philips 5x38mm";
string stringTool4 = "Chave philips 3x100mm";
string stringPont4 = "Pont. philips 3x100mm";
string stringTool5 = "Chave fenda 5x100mm";
string stringPont5 = "Pont. fenda 5x100mm";
string stringTool6 = "Chave philips 5x100mm";
string stringPont6 = "Pont. philips 5x100mm";
string stringTool7 = "Chave philips 5x125mm";
string stringPont7 = "Pont. philips 5x125mm";
string stringTool8 = "Chave philips 6x100mm";
string stringPont8 = "Pont. philips 6x100mm";
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

cppflow::model model_pont1("RedesPonteirasGV2/model_pont_1");
cppflow::model model_pont2("RedesPonteirasGV2/model_pont_2");
cppflow::model model_pont3("RedesPonteirasGV2/model_pont_3");
cppflow::model model_pont4("RedesPonteirasGV2/model_pont_4");
cppflow::model model_pont5("RedesPonteirasGV2/model_pont_5");
cppflow::model model_pont6("RedesPonteirasGV2/model_pont_6");
cppflow::model model_pont7("RedesPonteirasGV2/model_pont_7");
cppflow::model model_pont8("RedesPonteirasGV2/model_pont_8");
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

int RedePont1(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "Pont1.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model_pont1({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
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
	std::cout << std::fixed << stringPont1 << ":	" << fResult*100 << "%";

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

int RedePont2(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "Pont2.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model_pont2({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
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
	std::cout << std::fixed << stringPont2 << ":	" << fResult*100 << "%";

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

int RedePont3(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "Pont3.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model_pont3({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
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
	std::cout << std::fixed << stringPont3 << ":	" << fResult*100 << "%";

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

int RedePont4(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "Pont4.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model_pont4({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
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
	std::cout << std::fixed << stringPont4 << ":	" << fResult*100 << "%";

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

int RedePont5(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "Pont5.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model_pont5({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
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
	std::cout << std::fixed << stringPont5 << ":	" << fResult*100 << "%";

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

int RedePont6(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "Pont6.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model_pont6({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
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
	std::cout << std::fixed << stringPont6 << ":	" << fResult*100 << "%";

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

int RedePont7(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "Pont7.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model_pont7({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
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
	std::cout << std::fixed << stringPont7 << ":	" << fResult*100 << "%";

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

int RedePont8(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "Pont8.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model_pont8({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
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
	std::cout << std::fixed << stringPont8 << ":	" << fResult*100 << "%";

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
	
	//Processa imagens registradas na rede neural
	system("clear");
	
	/*RedeTool1();
	RedePont1();
	cout << endl;
	RedeTool2();
	RedePont2();
	cout << endl;
	RedeTool3();
	RedePont3();
	cout << endl;
	RedeTool4();
	RedePont4();
	cout << endl;
	RedeTool5();
	RedePont5();
	cout << endl;
	RedeTool6();
	RedePont6();
	cout << endl;
	RedeTool7();
	RedePont7();
	cout << endl;
	RedeTool8();
	RedePont8();
	cout << endl;
	RedeTool9();
	RedePont9();
	cout << endl;
	RedeTool10();
	RedePont10();
	cout << endl;
	RedeTool11();
	RedePont11();
	cout << endl;
	RedeTool12();
	RedePont12();
	cout << endl;
	RedeTool13();
	RedePont13();
	cout << endl;*/

	cv::Mat frame1, frame2, concat;
	frame1 = cv::imread("GV2_cam1_resized.png");
	frame2 = cv::imread("GV2_cam2_resized.png");

	if ((RedeTool1() + RedePont1())==2){
		cv::rectangle(frame2, cv::Point(2*85, 2*820), cv::Point(2*205, 2*1100), cv::Scalar(0, 255, 0, 0), 2, LINE_4, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(90, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
	}
	else{
		cv::rectangle(frame2, cv::Point(2*85, 2*820), cv::Point(2*205, 2*1100), cv::Scalar(0, 0, 255, 0), 2, LINE_4, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(90, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
	}

	cout << endl;
			
	if ((RedeTool2() + RedePont2())==2){
		cv::rectangle(frame2, cv::Point(2*230, 2*830), cv::Point(2*350, 2*1100), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(360, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
	}
	else{
		cv::rectangle(frame2, cv::Point(2*230, 2*830), cv::Point(2*350, 2*1100), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(360, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
	}

	cout << endl;
	
	if ((RedeTool3() + RedePont3())==2){
		cv::rectangle(frame2, cv::Point(2*385, 2*830), cv::Point(2*515, 2*1100), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(620, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
	}
	else{
		cv::rectangle(frame2, cv::Point(2*385, 2*830), cv::Point(2*515, 2*1100), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(620, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
	}

	cout << endl;
	
	if ((RedeTool4() + RedePont4())==2){
		cv::rectangle(frame2, cv::Point(2*90, 2*200), cv::Point(2*200, 2*815), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(885, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
	}
	else{
		cv::rectangle(frame2, cv::Point(2*90, 2*200), cv::Point(2*200, 2*815), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(885, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
	}

	cout << endl;
	
	if ((RedeTool5() + RedePont5())==2){
		cv::rectangle(frame2, cv::Point(2*235, 2*185), cv::Point(2*355, 2*825), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
	}
	else{
		cv::rectangle(frame2, cv::Point(2*235, 2*185), cv::Point(2*355, 2*825), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
	}

	cout << endl;
	
	if ((RedeTool6() + RedePont6())==2){
		cv::rectangle(frame2, cv::Point(2*390, 2*185), cv::Point(2*510, 2*825), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
	}
	else{
		cv::rectangle(frame2, cv::Point(2*390, 2*185), cv::Point(2*510, 2*825), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
	}

	cout << endl;
	
	if ((RedeTool7() + RedePont7())==2){
		cv::rectangle(frame2, cv::Point(2*600, 2*284), cv::Point(2*720, 2*1000), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
	}
	else{
		cv::rectangle(frame2, cv::Point(2*600, 2*284), cv::Point(2*720, 2*1000), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
	}

	cout << endl;
	
	if ((RedeTool8() + RedePont8())==2){
		cv::rectangle(frame2, cv::Point(2*740, 2*320), cv::Point(2*860, 2*1000), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
	}
	else{
		cv::rectangle(frame2, cv::Point(2*740, 2*320), cv::Point(2*860, 2*1000), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
	}

	cout << endl;
	
	if ((RedeTool9() + RedePont9())==2){
		cv::rectangle(frame1, cv::Point(2*20, 2*250), cv::Point(2*140, 2*1000), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
	}
	else{
		cv::rectangle(frame1, cv::Point(2*20, 2*250), cv::Point(2*140, 2*1000), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
	}

	cout << endl;
	
	if ((RedeTool10() + RedePont10())==2){
		cv::rectangle(frame1, cv::Point(2*155, 2*180), cv::Point(2*285, 2*1010), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
	}
	else{
		cv::rectangle(frame1, cv::Point(2*155, 2*180), cv::Point(2*285, 2*1010), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
	}

	cout << endl;
	
	if ((RedeTool11() + RedePont11())==2){
		cv::rectangle(frame1, cv::Point(2*305, 2*170), cv::Point(2*435, 2*1020), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
	}
	else{
		cv::rectangle(frame1, cv::Point(2*305, 2*170), cv::Point(2*435, 2*1020), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
	}

	cout << endl;
	
	if ((RedeTool12() + RedePont12())==2){
		cv::rectangle(frame1, cv::Point(2*460, 2*170), cv::Point(2*600, 2*1020), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
	}
	else{
		cv::rectangle(frame1, cv::Point(2*460, 2*170), cv::Point(2*600, 2*1020), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
	}

	cout << endl;
	
	if ((RedeTool13() + RedePont13())==2){
		cv::rectangle(frame1, cv::Point(2*615, 2*170), cv::Point(2*745, 2*1010), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
	}
	else{
		cv::rectangle(frame1, cv::Point(2*615, 2*170), cv::Point(2*745, 2*1010), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//v::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
	}

	cout << endl;

	hconcat(frame2(Rect(0, 140, frame2.cols, frame2.rows-140)), frame1(Rect(0, 140, frame1.cols, frame1.rows-140)), concat);
	imwrite("concat.png", concat);
	
	//imwrite("public/outputs/Final.jpg", imageFV2);
	//cv::imshow("Image",imageFV2);
	//cv::waitKey();
	//cv::waitKey();
	
	// ========== Fecha todos os dispositivos e janelas ==========
	cv::destroyAllWindows();
	
    return 0;
}