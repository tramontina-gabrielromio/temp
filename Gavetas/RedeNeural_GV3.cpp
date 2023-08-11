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

cppflow::model model1("RedesGV3/model_1");
cppflow::model model2("RedesGV3/model_2");
cppflow::model model3("RedesGV3/model_3");
cppflow::model model4("RedesGV3/model_4");
cppflow::model model5("RedesGV3/model_5");
cppflow::model model6("RedesGV3/model_6");
cppflow::model model7("RedesGV3/model_7");
cppflow::model model8("RedesGV3/model_8");
cppflow::model model9("RedesGV3/model_9");
cppflow::model model10("RedesGV3/model_10");
cppflow::model model11("RedesGV3/model_11");
cppflow::model model12("RedesGV3/model_12");
cppflow::model model13("RedesGV3/model_13");
cppflow::model model14("RedesGV3/model_14");
cppflow::model model15("RedesGV3/model_15");
cppflow::model model16("RedesGV3/model_16");
cppflow::model model17("RedesGV3/model_17");
cppflow::model model18("RedesGV3/model_18");
cppflow::model model19("RedesGV3/model_19");
cppflow::model model20("RedesGV3/model_20");
cppflow::model model21("RedesGV3/model_21");
cppflow::model model22("RedesGV3/model_22");
cppflow::model model23("RedesGV3/model_23");
cppflow::model model24("RedesGV3/model_24");
cppflow::model model25("RedesGV3/model_25");
cppflow::model model26("RedesGV3/model_26");
cppflow::model model27("RedesGV3/model_27");
cppflow::model model28("RedesGV3/model_28");
cppflow::model model29("RedesGV3/model_29");
cppflow::model model30("RedesGV3/model_30");
cppflow::model model31("RedesGV3/model_31");
cppflow::model model32("RedesGV3/model_32");
cppflow::model model33("RedesGV3/model_33");
cppflow::model model34("RedesGV3/model_34");
cppflow::model model35("RedesGV3/model_35");
cppflow::model model36("RedesGV3/model_36");
cppflow::model model37("RedesGV3/model_37");
cppflow::model model38("RedesGV3/model_38");
cppflow::model model39("RedesGV3/model_39");

int RedeTool1(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV3/Tool1.png"; //Define endereço das imagens
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
	std::string name = "ToolsGV3/Tool2.png"; //Define endereço das imagens
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
	std::string name = "ToolsGV3/Tool3.png"; //Define endereço das imagens
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
	std::string name = "ToolsGV3/Tool4.png"; //Define endereço das imagens
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
	std::string name = "ToolsGV3/Tool5.png"; //Define endereço das imagens
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
	std::string name = "ToolsGV3/Tool6.png"; //Define endereço das imagens
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
	std::string name = "ToolsGV3/Tool7.png"; //Define endereço das imagens
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
	std::string name = "ToolsGV3/Tool8.png"; //Define endereço das imagens
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
	std::string name = "ToolsGV3/Tool9.png"; //Define endereço das imagens
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
	std::string name = "ToolsGV3/Tool10.png"; //Define endereço das imagens
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
	std::string name = "ToolsGV3/Tool11.png"; //Define endereço das imagens
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
	std::string name = "ToolsGV3/Tool12.png"; //Define endereço das imagens
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
	std::string name = "ToolsGV3/Tool13.png"; //Define endereço das imagens
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
	std::string name = "ToolsGV3/Tool14.png"; //Define endereço das imagens
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
	std::string name = "ToolsGV3/Tool15.png"; //Define endereço das imagens
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
	std::string name = "ToolsGV3/Tool16.png"; //Define endereço das imagens
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
	std::string name = "ToolsGV3/Tool17.png"; //Define endereço das imagens
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

int RedeTool18(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV3/Tool18.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model18({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
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

int RedeTool19(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV3/Tool19.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model19({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
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

int RedeTool20(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV3/Tool20.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model20({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
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

int RedeTool21(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV3/Tool21.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model21({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
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

int RedeTool22(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV3/Tool22.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model22({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
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

int RedeTool23(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV3/Tool23.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model23({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
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

int RedeTool24(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV3/Tool24.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model24({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
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

int RedeTool25(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV3/Tool25.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model25({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
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

int RedeTool26(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV3/Tool26.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model26({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
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

int RedeTool27(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV3/Tool27.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model27({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
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

int RedeTool28(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV3/Tool28.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model28({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
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

int RedeTool29(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV3/Tool29.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model29({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
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

int RedeTool30(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV3/Tool30.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model30({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
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

int RedeTool31(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV3/Tool31.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model31({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
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

int RedeTool32(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV3/Tool32.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model32({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
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

int RedeTool33(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV3/Tool33.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model33({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
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

int RedeTool34(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV3/Tool34.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model34({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
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

int RedeTool35(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV3/Tool35.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model35({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
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

int RedeTool36(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV3/Tool36.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model36({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
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

int RedeTool37(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV3/Tool37.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model37({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
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

int RedeTool38(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV3/Tool38.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model38({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
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

int RedeTool39(){
	// ========== Carrega e testa a imagem com a rede neural ==========
	int result;
	std::string name = "ToolsGV3/Tool39.png"; //Define endereço das imagens
    auto input = cppflow::decode_jpeg(cppflow::read_file(std::string(name)));
    //cout << cppflow::read_file(std::string(name));
    input = cppflow::cast(input, TF_UINT8, TF_FLOAT);
    input = cppflow::expand_dims(input, 0);
        
    //Aplica rede neural para análise da imagem
	auto output = model39({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
	
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
    ifstream obj1("ToolsGV3/Tool1.png"), obj2("ToolsGV3/Tool2.png"), obj3("ToolsGV3/Tool3.png"), obj4("ToolsGV3/Tool4.png"), obj5("ToolsGV3/Tool5.png"), obj6("ToolsGV3/Tool6.png"), obj7("ToolsGV3/Tool7.png"),
	obj8("ToolsGV3/Tool8.png"), obj9("ToolsGV3/Tool9.png"), obj10("ToolsGV3/Tool10.png"), obj11("ToolsGV3/Tool11.png"), obj12("ToolsGV3/Tool14.png"), obj13("ToolsGV3/Tool13.png"), obj14("ToolsGV3/Tool14.png"),
	obj15("ToolsGV3/Tool15.png"), obj16("ToolsGV3/Tool16.png"), obj17("ToolsGV3/Tool17.png"), obj18("ToolsGV3/Tool18.png"), obj19("ToolsGV3/Tool19.png"), obj20("ToolsGV3/Tool20.png"), obj21("ToolsGV3/Tool21.png"), 
	obj22("ToolsGV3/Tool22.png"), obj23("ToolsGV3/Tool23.png"), obj24("ToolsGV3/Tool24.png"),obj25("ToolsGV3/Tool25.png"), obj26("ToolsGV3/Tool26.png"), obj27("ToolsGV3/Tool27.png"), obj28("ToolsGV3/Tool28.png"), 
	obj29("ToolsGV3/Tool29.png"), obj30("ToolsGV3/Tool30.png"), obj31("ToolsGV3/Tool31.png"), obj32("ToolsGV3/Tool32.png"), obj33("ToolsGV3/Tool33.png"), obj34("ToolsGV3/Tool34.png"), obj35("ToolsGV3/Tool35.png"), 
	obj36("ToolsGV3/Tool36.png"), obj37("ToolsGV3/Tool37.png"), obj38("ToolsGV3/Tool38.png"), obj39("ToolsGV3/Tool39.png");
	if ((!obj1||!obj2||!obj3||!obj4||!obj5||!obj6||!obj7||!obj8||!obj9||!obj10||!obj11||!obj12||!obj13||!obj14||!obj15||!obj16||!obj17||!obj18||!obj19||!obj20||!obj21||
	!obj22||!obj23||!obj24||!obj25||!obj26||!obj27||!obj28||!obj29||!obj30||!obj31||!obj32||!obj33||!obj34||!obj35||!obj36||!obj37||!obj38||!obj39)==0 && *contEncoder==0){
        //Carrega tensores, para evitar problemas de processamento, encerra o carregamento caso a gaveta se feche durante o processo
		for(int i=1; i<=39; i++){
			std::string name = "ToolsGV3/Tool" + std::to_string(i) + ".png";
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
			else if (i==18) auto output = model18({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==19) auto output = model19({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==20) auto output = model20({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==21) auto output = model21({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==22) auto output = model22({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==23) auto output = model23({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==24) auto output = model24({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==25) auto output = model25({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==26) auto output = model26({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==27) auto output = model27({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==28) auto output = model28({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==29) auto output = model29({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==30) auto output = model30({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==31) auto output = model31({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==32) auto output = model32({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==33) auto output = model33({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==34) auto output = model34({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==35) auto output = model35({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==36) auto output = model36({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==37) auto output = model37({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==38) auto output = model38({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
			else if (i==39) auto output = model39({{"serving_default_rescaling_input:0",input}},{"StatefulPartitionedCall:0"});
		}
    }
	//Aguarda finalização da captura das imagens
	while(*ImagemCam1OK!=1 || *ImagemCam2OK!=1){}

	
	*ImagemCam1OK = 0;
	*ImagemCam2OK = 0;

	std::string result = "";
	cv::Mat frame1, frame2, concat;
	frame1 = cv::imread("GV3_cam1_resized.png");
	frame2 = cv::imread("GV3_cam2_resized.png");

	if (RedeTool1()){
		cv::rectangle(frame1, cv::Point(2*18, 2*24), cv::Point(2*56, 2*200), cv::Scalar(0, 255, 0, 0), 2, LINE_4, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(90, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
		result = result + "1";
	}
	else{
		cv::rectangle(frame1, cv::Point(2*18, 2*24), cv::Point(2*56, 2*200), cv::Scalar(0, 0, 255, 0), 2, LINE_4, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(90, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
		result = result + "0";
	}
	
	if (RedeTool2()){
		cv::rectangle(frame1, cv::Point(2*74, 2*24), cv::Point(2*114, 2*200), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(360, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
		result = result + "1";
	}
	else{
		cv::rectangle(frame1, cv::Point(2*74, 2*24), cv::Point(2*114, 2*200), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(360, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
		result = result + "0";
	}
	
	if (RedeTool3()){
		cv::rectangle(frame1, cv::Point(2*132, 2*24), cv::Point(2*174, 2*200), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(620, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
		result = result + "1";
	}
	else{
		cv::rectangle(frame1, cv::Point(2*132, 2*24), cv::Point(2*174, 2*200), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(620, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
		result = result + "0";
	}
	
	if (RedeTool4()){
		cv::rectangle(frame1, cv::Point(2*190, 2*24), cv::Point(2*232, 2*200), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(885, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
		result = result + "1";
	}
	else{
		cv::rectangle(frame1, cv::Point(2*190, 2*24), cv::Point(2*232, 2*200), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(885, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
		result = result + "0";
	}
	
	if (RedeTool5()){
		cv::rectangle(frame1, cv::Point(2*246, 2*24), cv::Point(2*286, 2*200), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
		result = result + "1";
	}
	else{
		cv::rectangle(frame1, cv::Point(2*246, 2*24), cv::Point(2*286, 2*200), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
		result = result + "0";
	}
	
	if (RedeTool6()){
		cv::rectangle(frame1, cv::Point(2*6, 2*200), cv::Point(2*44, 2*320), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
		result = result + "1";
	}
	else{
		cv::rectangle(frame1, cv::Point(2*6, 2*200), cv::Point(2*44, 2*320), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
		result = result + "0";
	}
	
	if (RedeTool7()){
		cv::rectangle(frame1, cv::Point(2*44, 2*200), cv::Point(2*80, 2*320), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
		result = result + "1";
	}
	else{
		cv::rectangle(frame1, cv::Point(2*44, 2*200), cv::Point(2*80, 2*320), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
		result = result + "0";
	}
	
	if (RedeTool8()){
		cv::rectangle(frame1, cv::Point(2*80, 2*200), cv::Point(2*118, 2*320), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
		result = result + "1";
	}
	else{
		cv::rectangle(frame1, cv::Point(2*80, 2*200), cv::Point(2*118, 2*320), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
		result = result + "0";
	}
	
	if (RedeTool9()){
		cv::rectangle(frame1, cv::Point(2*118, 2*200), cv::Point(2*162, 2*320), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
		result = result + "1";
	}
	else{
		cv::rectangle(frame1, cv::Point(2*118, 2*200), cv::Point(2*162, 2*320), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
		result = result + "0";
	}
	
	if (RedeTool10()){
		cv::rectangle(frame1, cv::Point(2*162, 2*200), cv::Point(2*202, 2*320), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
		result = result + "1";
	}
	else{
		cv::rectangle(frame1, cv::Point(2*162, 2*200), cv::Point(2*202, 2*320), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
		result = result + "0";
	}
	
	if (RedeTool11()){
		cv::rectangle(frame1, cv::Point(2*202, 2*200), cv::Point(2*240, 2*320), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
		result = result + "1";
	}
	else{
		cv::rectangle(frame1, cv::Point(2*202, 2*200), cv::Point(2*240, 2*320), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
		result = result + "0";
	}
	
	if (RedeTool12()){
		cv::rectangle(frame1, cv::Point(2*240, 2*200), cv::Point(2*278, 2*320), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
		result = result + "1";
	}
	else{
		cv::rectangle(frame1, cv::Point(2*240, 2*200), cv::Point(2*278, 2*320), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
		result = result + "0";
	}
	
	if (RedeTool13()){
		cv::rectangle(frame1, cv::Point(2*278, 2*200), cv::Point(2*318, 2*320), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
		result = result + "1";
	}
	else{
		cv::rectangle(frame1, cv::Point(2*278, 2*200), cv::Point(2*318, 2*320), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
		result = result + "0";
	}
	
	if (RedeTool14()){
		cv::rectangle(frame1, cv::Point(2*4, 2*312), cv::Point(2*66, 2*418), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
		result = result + "1";
	}
	else{
		cv::rectangle(frame1, cv::Point(2*4, 2*312), cv::Point(2*66, 2*418), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
		result = result + "0";
	}
	
	if (RedeTool15()){
		cv::rectangle(frame1, cv::Point(2*66, 2*312), cv::Point(2*126, 2*418), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
		result = result + "1";
	}
	else{
		cv::rectangle(frame1, cv::Point(2*66, 2*312), cv::Point(2*126, 2*418), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
		result = result + "0";
	}
	
	if (RedeTool16()){
		cv::rectangle(frame1, cv::Point(2*126, 2*312), cv::Point(2*186, 2*418), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
		result = result + "1";
	}
	else{
		cv::rectangle(frame1, cv::Point(2*126, 2*312), cv::Point(2*186, 2*418), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
		result = result + "0";
	}
	
	if (RedeTool17()){
		cv::rectangle(frame1, cv::Point(2*186, 2*312), cv::Point(2*238, 2*418), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
		result = result + "1";
	}
	else{
		cv::rectangle(frame1, cv::Point(2*186, 2*312), cv::Point(2*238, 2*418), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
		result = result + "0";
	}
	
	if (RedeTool18()){
		cv::rectangle(frame1, cv::Point(2*238, 2*312), cv::Point(2*288, 2*418), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
		result = result + "1";
	}
	else{
		cv::rectangle(frame1, cv::Point(2*238, 2*312), cv::Point(2*288, 2*418), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
		result = result + "0";
	}
	
	if (RedeTool19()){
		cv::rectangle(frame1, cv::Point(2*288, 2*312), cv::Point(2*332, 2*418), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
		result = result + "1";
	}
	else{
		cv::rectangle(frame1, cv::Point(2*288, 2*312), cv::Point(2*332, 2*418), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
		result = result + "0";
	}
	
	if (RedeTool20()){
		cv::rectangle(frame1, cv::Point(2*8, 2*408), cv::Point(2*48, 2*480), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
		result = result + "1";
	}
	else{
		cv::rectangle(frame1, cv::Point(2*8, 2*408), cv::Point(2*48, 2*480), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
		result = result + "0";
	}
	
	if (RedeTool21()){
		cv::rectangle(frame1, cv::Point(2*48, 2*408), cv::Point(2*90, 2*480), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
		result = result + "1";
	}
	else{
		cv::rectangle(frame1, cv::Point(2*48, 2*408), cv::Point(2*90, 2*480), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
		result = result + "0";
	}
	
	if (RedeTool22()){
		cv::rectangle(frame1, cv::Point(2*90, 2*408), cv::Point(2*132, 2*480), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
		result = result + "1";
	}
	else{
		cv::rectangle(frame1, cv::Point(2*90, 2*408), cv::Point(2*132, 2*480), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
		result = result + "0";
	}
	
	if (RedeTool23()){
		cv::rectangle(frame1, cv::Point(2*132, 2*408), cv::Point(2*174, 2*480), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
		result = result + "1";
	}
	else{
		cv::rectangle(frame1, cv::Point(2*132, 2*408), cv::Point(2*174, 2*480), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
		result = result + "0";
	}
	
	if (RedeTool24()){
		cv::rectangle(frame1, cv::Point(2*174, 2*408), cv::Point(2*214, 2*480), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
		result = result + "1";
	}
	else{
		cv::rectangle(frame1, cv::Point(2*174, 2*408), cv::Point(2*214, 2*480), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
		result = result + "0";
	}
	
	if (RedeTool25()){
		cv::rectangle(frame1, cv::Point(2*214, 2*408), cv::Point(2*252, 2*480), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
		result = result + "1";
	}
	else{
		cv::rectangle(frame1, cv::Point(2*214, 2*408), cv::Point(2*252, 2*480), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
		result = result + "0";
	}
	
	if (RedeTool26()){
		cv::rectangle(frame1, cv::Point(2*252, 2*408), cv::Point(2*288, 2*480), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
		result = result + "1";
	}
	else{
		cv::rectangle(frame1, cv::Point(2*252, 2*408), cv::Point(2*288, 2*480), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
		result = result + "0";
	}
	
	if (RedeTool27()){
		cv::rectangle(frame1, cv::Point(2*288, 2*408), cv::Point(2*324, 2*480), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
		result = result + "1";
	}
	else{
		cv::rectangle(frame1, cv::Point(2*288, 2*408), cv::Point(2*324, 2*480), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
		result = result + "0";
	}
	
	if (RedeTool28()){
		cv::rectangle(frame2, cv::Point(2*0, 2*408), cv::Point(2*30, 2*480), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
		result = result + "1";
	}
	else{
		cv::rectangle(frame2, cv::Point(2*0, 2*408), cv::Point(2*30, 2*480), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
		result = result + "0";
	}
	
	if (RedeTool29()){
		cv::rectangle(frame2, cv::Point(2*30, 2*408), cv::Point(2*62, 2*480), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
		result = result + "1";
	}
	else{
		cv::rectangle(frame2, cv::Point(2*30, 2*408), cv::Point(2*62, 2*480), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
		result = result + "0";
	}
	
	if (RedeTool30()){
		cv::rectangle(frame2, cv::Point(2*62, 2*408), cv::Point(2*96, 2*480), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
		result = result + "1";
	}
	else{
		cv::rectangle(frame2, cv::Point(2*62, 2*408), cv::Point(2*96, 2*480), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
		result = result + "0";
	}
	
	if (RedeTool31()){
		cv::rectangle(frame2, cv::Point(2*96, 2*408), cv::Point(2*134, 2*480), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
		result = result + "1";
	}
	else{
		cv::rectangle(frame2, cv::Point(2*96, 2*408), cv::Point(2*134, 2*480), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
		result = result + "0";
	}
	
	if (RedeTool32()){
		cv::rectangle(frame2, cv::Point(2*14, 2*24), cv::Point(2*76, 2*380), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
		result = result + "1";
	}
	else{
		cv::rectangle(frame2, cv::Point(2*14, 2*24), cv::Point(2*76, 2*380), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
		result = result + "0";
	}
	
	if (RedeTool33()){
		cv::rectangle(frame2, cv::Point(2*78, 2*24), cv::Point(2*142, 2*380), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
		result = result + "1";
	}
	else{
		cv::rectangle(frame2, cv::Point(2*78, 2*24), cv::Point(2*142, 2*380), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
		result = result + "0";
	}
	
	if (RedeTool34()){
		cv::rectangle(frame2, cv::Point(2*150, 2*24), cv::Point(2*190, 2*380), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
		result = result + "1";
	}
	else{
		cv::rectangle(frame2, cv::Point(2*150, 2*24), cv::Point(2*190, 2*380), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
		result = result + "0";
	}
	
	if (RedeTool35()){
		cv::rectangle(frame2, cv::Point(2*220, 2*24), cv::Point(2*274, 2*380), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
		result = result + "1";
	}
	else{
		cv::rectangle(frame2, cv::Point(2*220, 2*24), cv::Point(2*274, 2*380), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
		result = result + "0";
	}
	
	if (RedeTool36()){
		cv::rectangle(frame2, cv::Point(2*282, 2*24), cv::Point(2*330, 2*346), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
		result = result + "1";
	}
	else{
		cv::rectangle(frame2, cv::Point(2*282, 2*24), cv::Point(2*330, 2*346), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
		result = result + "0";
	}
	
	if (RedeTool37()){
		cv::rectangle(frame2, cv::Point(2*338, 2*24), cv::Point(2*386, 2*346), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
		result = result + "1";
	}
	else{
		cv::rectangle(frame2, cv::Point(2*338, 2*24), cv::Point(2*386, 2*346), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
		result = result + "0";
	}
	
	if (RedeTool38()){
		cv::rectangle(frame2, cv::Point(2*240, 2*394), cv::Point(2*346, 2*446), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
		result = result + "1";
	}
	else{
		cv::rectangle(frame2, cv::Point(2*240, 2*394), cv::Point(2*346, 2*446), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
		result = result + "0";
	}
	
	if (RedeTool39()){
		cv::rectangle(frame2, cv::Point(2*182, 2*446), cv::Point(2*346, 2*480), cv::Scalar(0, 255, 0, 0), 2, LINE_8, 1);
		//cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
		result = result + "1";
	}
	else{
		cv::rectangle(frame2, cv::Point(2*182, 2*446), cv::Point(2*346, 2*480), cv::Scalar(0, 0, 255, 0), 2, LINE_8, 1);
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