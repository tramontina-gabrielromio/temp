
#ifndef JETSONGPIO_H_
#define JETSONGPIO_H_

 /****************************************************************
 * Constants
 ****************************************************************/
 
#define SYSFS_GPIO_DIR "/sys/class/gpio"
#define POLL_TIMEOUT (3 * 1000) /* 3 seconds */
#define MAX_BUF 64

typedef unsigned int jetsonGPIO ;
typedef unsigned int pinDirection ;
typedef unsigned int pinValue ;

enum pinDirections {
	inputPin  = 0,
	outputPin = 1
} ;

enum pinValues {
    low = 0,
    high = 1,
    off = 0,  // synonym for things like lights
    on = 1
}  ;

enum jetsonXavierGPIONumber {
       gpio436 = 436,      // Pin  7
       gpio428 = 428,      // Pin 11
       gpio445 = 445,      // Pin 12
       gpio480 = 480,      // Pin 13
       gpio268 = 268,      // Pin 15
       gpio484 = 484,      // Pin 16
       gpio483 = 483,      // Pin 18
       gpio493 = 493,      // Pin 19
       gpio492 = 492,      // Pin 21
       gpio481 = 481,      // Pin 22
       gpio491 = 491,      // Pin 23
       gpio494 = 494,      // Pin 24
       gpio495 = 495,      // Pin 26
       gpio421 = 421,      // Pin 29
       gpio422 = 422,      // Pin 31
       gpio424 = 424,      // Pin 32
       gpio393 = 393,      // Pin 33
       gpio448 = 448,      // Pin 35
       gpio429 = 429,      // Pin 36
       gpio482 = 482,      // Pin 37
       gpio447 = 447,      // Pin 38
       gpio446 = 446,      // Pin 40
} ;

int gpioExport ( jetsonGPIO gpio ) ;
int gpioUnexport ( jetsonGPIO gpio ) ;
int gpioSetDirection ( jetsonGPIO, pinDirection out_flag ) ;
int gpioSetValue ( jetsonGPIO gpio, pinValue value ) ;
int gpioGetValue ( jetsonGPIO gpio ) ;
int gpioSetEdge ( jetsonGPIO gpio, char *edge ) ;
int gpioOpen ( jetsonGPIO gpio ) ;
int gpioClose ( int fileDescriptor ) ;
int gpioActiveLow ( jetsonGPIO gpio, unsigned int value ) ;
int gpioPwmExport ( ) ;
int gpioPwmUnexport ( ) ;
int gpioSetPwmDutyCycle ( unsigned int value ) ;
int gpioGetPwmDutyCycle ( ) ;
int gpioSetPwmPeriod ( unsigned int value ) ;
int gpioGetPwmPeriod ( ) ;
int gpioSetPwmStatus ( pinValue value ) ;
int gpioGetPwmStatus ( ) ;

#endif // JETSONGPIO_H_
