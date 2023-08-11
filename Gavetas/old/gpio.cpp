#include <iostream>
// for delay function.
#include <chrono> 
#include <thread>

// for signal handling

#include <JetsonGPIO.h>

using namespace std;

// Pin Definitions
int output_pin = 18; // BOARD pin 12, BCM pin 18

int main(){

	// Pin Setup. 
	GPIO::setmode(GPIO::BCM);

	GPIO::output(output_pin, 1);

	while(true){
	}

	return 0;
}