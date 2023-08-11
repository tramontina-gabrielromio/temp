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

//Endereços de memória compartilhada
#define KEYEncoder 9902
#define KEYImagemCam1OK 9903
#define KEYImagemCam2OK 9904

/*string stringTool1 = "Chave fenda 5x38mm";
string stringTool2 = "Chave fenda 5x38mm";
string stringTool3 = "Chave philips 5x38mm";
string stringTool4 = "Chave philips 3x100mm";
string stringTool5 = "Chave fenda 5x100mm";
string stringTool6 = "Chave philips 5x100mm";
string stringTool7 = "Chave philips 5x125mm";*/

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
cppflow::model model14("RedesGV2/model_14");
cppflow::model model15("RedesGV2/model_15");
cppflow::model model16("RedesGV2/model_16");
cppflow::model model17("RedesGV2/model_17");

int RedeTool1(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV2/Tool1.png"; //Define endereço das imagens
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
	//std::cout << std::fixed << stringTool1 << ":	" << fResult*100 << "%";
	
	if (fResult >= 0.95) {
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

int RedeTool2(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV2/Tool2.png"; //Define endereço das imagens
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
	//std::cout << std::fixed << stringTool2 << ":	" << fResult*100 << "%";
	
	if (fResult >= 0.95) {
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

int RedeTool3(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV2/Tool3.png"; //Define endereço das imagens
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
	//std::cout << std::fixed << stringTool3 << ":	" << fResult*100 << "%";

	if (fResult >= 0.95) {
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

int RedeTool4(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV2/Tool4.png"; //Define endereço das imagens
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
	//std::cout << std::fixed << stringTool4 << ":	" << fResult*100 << "%";

	if (fResult >= 0.95) {
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

int RedeTool5(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV2/Tool5.png"; //Define endereço das imagens
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
	//std::cout << std::fixed << stringTool5 << ":	" << fResult*100 << "%";

	if (fResult >= 0.95) {
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

int RedeTool6(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV2/Tool6.png"; //Define endereço das imagens
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
	//std::cout << std::fixed << stringTool6 << ":	" << fResult*100 << "%";

	if (fResult >= 0.95) {
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

int RedeTool7(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV2/Tool7.png"; //Define endereço das imagens
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
	//std::cout << std::fixed << stringTool7 << ":	" << fResult*100 << "%";

	if (fResult >= 0.95) {
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

int RedeTool8(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV2/Tool8.png"; //Define endereço das imagens
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
	//std::cout << std::fixed << stringTool8 << ":	" << fResult*100 << "%";

	if (fResult >= 0.95) {
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

int RedeTool9(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV2/Tool9.png"; //Define endereço das imagens
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
	//std::cout << std::fixed << stringTool9 << ":	" << fResult*100 << "%";

	if (fResult >= 0.95) {
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

int RedeTool10(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV2/Tool10.png"; //Define endereço das imagens
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
	//std::cout << std::fixed << stringTool10 << ":	" << fResult*100 << "%";

	if (fResult >= 0.95) {
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

int RedeTool11(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV2/Tool11.png"; //Define endereço das imagens
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
	//std::cout << std::fixed << stringTool11 << ":	" << fResult*100 << "%";

	if (fResult >= 0.95) {
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

int RedeTool12(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV2/Tool12.png"; //Define endereço das imagens
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
	//std::cout << std::fixed << stringTool12 << ":	" << fResult*100 << "%";

	if (fResult >= 0.95) {
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

int RedeTool13(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV2/Tool13.png"; //Define endereço das imagens
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
	//std::cout << std::fixed << stringTool13 << ":	" << fResult*100 << "%";

	if (fResult >= 0.95) {
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

int RedeTool14(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV2/Tool14.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model14({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
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
	//std::cout << std::fixed << stringTool13 << ":	" << fResult*100 << "%";

	if (fResult >= 0.95) {
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

int RedeTool15(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV2/Tool15.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model15({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
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
	//std::cout << std::fixed << stringTool13 << ":	" << fResult*100 << "%";

	if (fResult >= 0.95) {
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

int RedeTool16(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV2/Tool16.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model16({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
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
	//std::cout << std::fixed << stringTool13 << ":	" << fResult*100 << "%";

	if (fResult >= 0.95) {
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

int RedeTool17(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV2/Tool17.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model17({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
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
	//std::cout << std::fixed << stringTool13 << ":	" << fResult*100 << "%";

	if (fResult >= 0.95) {
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
	int shmidMemoria, shmidImagemCam1, shmidImagemCam2;
	char *path="/home/tramontina/Downloads/Gavetas";
	int *contEncoder, *ImagemCam1OK, *ImagemCam2OK;
	int ProximoRegistro = 0; //Contagem de pulsos do encoder para próxima captura

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

    //Carrega os tensores pela primeira vez, dessa forma as próximas execuções serão muito mais rápidas
    //Verifica se os arquivos existem
    ifstream obj1("ToolsGV2/Tool1.png"), obj2("ToolsGV2/Tool2.png"), obj3("ToolsGV2/Tool3.png"), obj4("ToolsGV2/Tool4.png"), obj5("ToolsGV2/Tool5.png"), obj6("ToolsGV2/Tool6.png"), obj7("ToolsGV2/Tool7.png"),
	obj8("ToolsGV2/Tool8.png"), obj9("ToolsGV2/Tool9.png"), obj10("ToolsGV2/Tool10.png"), obj11("ToolsGV2/Tool11.png"), obj12("ToolsGV2/Tool14.png"), obj13("ToolsGV2/Tool13.png"), obj14("ToolsGV2/Tool14.png"),
	obj15("ToolsGV2/Tool15.png"), obj16("ToolsGV2/Tool16.png"), obj17("ToolsGV2/Tool17.png");
    if ((!obj1||!obj2||!obj3||!obj4||!obj5||!obj6||!obj7||!obj8||!obj9||!obj10||!obj11||!obj12||!obj13||!obj14||!obj15||!obj16||!obj17)==0 && *contEncoder==0){
        //Carrega tensores, para evitar problemas de processamento, encerra o carregamento caso a gaveta se feche durante o processo
		for(int i=1; i<=17; i++){
			std::string name = "ToolsGV2/Tool" + std::to_string(i) + ".png";
			auto input = cppflow::expand_dims(cppflow::cast(cppflow::decode_jpeg(cppflow::read_file(std::string(name))), TF_UINT8, TF_FLOAT), 0);
			if(*contEncoder>0) break;
			else if (i==1) auto output = model1({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==2) auto output = model2({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==3) auto output = model3({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==4) auto output = model4({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==5) auto output = model5({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==6) auto output = model6({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==7) auto output = model7({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==8) auto output = model8({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==9) auto output = model9({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==10) auto output = model10({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==11) auto output = model11({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==12) auto output = model12({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==13) auto output = model13({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==14) auto output = model14({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==15) auto output = model15({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==16) auto output = model16({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==17) auto output = model17({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
		}
    }

	//Aguarda finalização da captura das imagens
	while(*ImagemCam1OK!=1 || *ImagemCam2OK!=1){}

	*ImagemCam1OK = 0;
	*ImagemCam2OK = 0;

	std::string result = "";
	cv::Mat frame1, frame2, concat;
	frame1 = cv::imread("GV2_cam1_resized.png");
	frame2 = cv::imread("GV2_cam2_resized.png");

	if (RedeTool1()){
		cv::rectangle(frame1, cv::Point(2*0, 2*282), cv::Point(2*232, 2*406), cv::Scalar(0, 255, 0, 0), 2, LINE_4, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(90, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
		result = result + "1";
	}
	else{
		cv::rectangle(frame1, cv::Point(2*0, 2*282), cv::Point(2*232, 2*406), cv::Scalar(0, 0, 255, 0), 2, LINE_4, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(90, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
		result = result + "0";
	}
	
	if (RedeTool2()){
		cv::rectangle(frame1, cv::Point(2*12, 2*234), cv::Point(2*272, 2*380), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(360, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
		result = result + "1";
	}
	else{
		cv::rectangle(frame1, cv::Point(2*12, 2*234), cv::Point(2*272, 2*380), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(360, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
		result = result + "0";
	}
	
	if (RedeTool3()){
		cv::rectangle(frame1, cv::Point(2*22, 2*184), cv::Point(2*312, 2*336), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(620, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
		result = result + "1";
	}
	else{
		cv::rectangle(frame1, cv::Point(2*22, 2*184), cv::Point(2*312, 2*336), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(620, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
		result = result + "0";
	}
	
	if (RedeTool4()){
		cv::rectangle(frame1, cv::Point(2*54, 2*136), cv::Point(2*354, 2*292), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(885, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
		result = result + "1";
	}
	else{
		cv::rectangle(frame1, cv::Point(2*54, 2*136), cv::Point(2*354, 2*292), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(885, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
		result = result + "0";
	}
	
	if (RedeTool5()){
		cv::rectangle(frame1, cv::Point(2*58, 2*82), cv::Point(2*394, 2*250), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
		result = result + "1";
	}
	else{
		cv::rectangle(frame1, cv::Point(2*58, 2*82), cv::Point(2*394, 2*250), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
		result = result + "0";
	}
	
	if (RedeTool6()){
		cv::rectangle(frame1, cv::Point(2*70, 2*26), cv::Point(2*440, 2*206), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
		result = result + "1";
	}
	else{
		cv::rectangle(frame1, cv::Point(2*70, 2*26), cv::Point(2*440, 2*206), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
		result = result + "0";
	}
	
	if (RedeTool7()){
		cv::rectangle(frame1, cv::Point(2*4, 2*392), cv::Point(2*140, 2*600), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
		result = result + "1";
	}
	else{
		cv::rectangle(frame1, cv::Point(2*4, 2*392), cv::Point(2*140, 2*600), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
		result = result + "0";
	}
	
	if (RedeTool8()){
		cv::rectangle(frame1, cv::Point(2*140, 2*392), cv::Point(2*294, 2*600), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
		result = result + "1";
	}
	else{
		cv::rectangle(frame1, cv::Point(2*140, 2*392), cv::Point(2*294, 2*600), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
		result = result + "0";
	}
	
	if (RedeTool9()){
		cv::rectangle(frame1, cv::Point(2*290, 2*348), cv::Point(2*430, 2*600), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
		result = result + "1";
	}
	else{
		cv::rectangle(frame1, cv::Point(2*290, 2*348), cv::Point(2*430, 2*600), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
		result = result + "0";
	}
	
	if (RedeTool10()){
		cv::rectangle(frame1, cv::Point(2*352, 2*280), cv::Point(2*480, 2*544), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
		result = result + "1";
	}
	else{
		cv::rectangle(frame1, cv::Point(2*352, 2*280), cv::Point(2*480, 2*544), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
		result = result + "0";
	}
	
	if (RedeTool11()){
		cv::rectangle(frame2, cv::Point(2*20, 2*14), cv::Point(2*180, 2*190), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
		result = result + "1";
	}
	else{
		cv::rectangle(frame2, cv::Point(2*20, 2*14), cv::Point(2*180, 2*190), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
		result = result + "0";
	}
	
	if (RedeTool12()){
		cv::rectangle(frame2, cv::Point(2*184, 2*18), cv::Point(2*468, 2*178), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
		result = result + "1";
	}
	else{
		cv::rectangle(frame2, cv::Point(2*184, 2*18), cv::Point(2*468, 2*178), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
		result = result + "0";
	}
	
	if (RedeTool13()){
		cv::rectangle(frame2, cv::Point(2*6, 2*308), cv::Point(2*134, 2*600), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
		result = result + "1";
	}
	else{
		cv::rectangle(frame2, cv::Point(2*6, 2*308), cv::Point(2*134, 2*600), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
		result = result + "0";
	}
	
	if (RedeTool14()){
		cv::rectangle(frame2, cv::Point(2*46, 2*230), cv::Point(2*206, 2*550), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
		result = result + "1";
	}
	else{
		cv::rectangle(frame2, cv::Point(2*46, 2*230), cv::Point(2*206, 2*550), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
		result = result + "0";
	}
	
	if (RedeTool15()){
		cv::rectangle(frame2, cv::Point(2*138, 2*240), cv::Point(2*306, 2*600), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
		result = result + "1";
	}
	else{
		cv::rectangle(frame2, cv::Point(2*138, 2*240), cv::Point(2*306, 2*600), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
		result = result + "0";
	}
	
	if (RedeTool16()){
		cv::rectangle(frame2, cv::Point(2*196, 2*162), cv::Point(2*382, 2*554), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
		result = result + "1";
	}
	else{
		cv::rectangle(frame2, cv::Point(2*196, 2*162), cv::Point(2*382, 2*554), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
		result = result + "0";
	}
	
	if (RedeTool17()){
		cv::rectangle(frame2, cv::Point(2*308, 2*172), cv::Point(2*476, 2*600), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
		result = result + "1";
	}
	else{
		cv::rectangle(frame2, cv::Point(2*308, 2*172), cv::Point(2*476, 2*600), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
		result = result + "0";
	}

	cout << result << endl;
	
	//hconcat(frame1(Rect(0, 0, frame1.cols, frame1.rows)), frame2(Rect(0, 0, frame2.cols, frame2.rows)), concat);
	//Point2f pointRotate(concat.cols/2, concat.rows/2);
	//warpAffine(concat, concat, getRotationMatrix2D(pointRotate, 180, 1.0), Size(concat.cols, concat.rows));
	//imwrite("public/outputs/Gaveta8.png", concat);
	//imwrite("Gaveta8.png", concat);
	imwrite("frame1.png", frame1);
	imwrite("frame2.png", frame2);
	
	//imwrite("public/outputs/Final.jpg", imageFV2);
	//cv::imshow("Image",imageFV2);
	//cv::waitKey();
	//cv::waitKey();

	
	// ========== Fecha todos os dispositivos e janelas ==========
	cv::destroyAllWindows();
	
    return 0;
}