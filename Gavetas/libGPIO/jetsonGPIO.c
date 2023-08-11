// jetsonGPIO.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <math.h>
#include "jetsonGPIO.h"


//
// gpioExport
// Export the given gpio to userspace;
// Return: Success = 0 ; otherwise open file error
int gpioExport ( jetsonGPIO gpio )
{
    int fileDescriptor, length;
    char commandBuffer[MAX_BUF];

    fileDescriptor = open(SYSFS_GPIO_DIR "/export", O_WRONLY);
    //printf("filedescriptor %d ", fileDescriptor);
    if (fileDescriptor < 0) {
        char errorBuffer[128] ;
        snprintf(errorBuffer,sizeof(errorBuffer), "gpioExport unable to open gpio%d",gpio) ;
        //perror(errorBuffer);
        return fileDescriptor;
    }

    //transfer gpio number to %d format and set it to commandBuffer
    length = snprintf(commandBuffer, sizeof(commandBuffer), "%d", gpio);
    if (write(fileDescriptor, commandBuffer, length) != length) {
        //perror("gpioExport");
        return fileDescriptor ;

    }
    close(fileDescriptor);

    return 0;
}

//
// gpioUnexport
// Unexport the given gpio from userspace
// Return: Success = 0 ; otherwise open file error
int gpioUnexport ( jetsonGPIO gpio )
{
    int fileDescriptor, length;
    char commandBuffer[MAX_BUF];

    fileDescriptor = open(SYSFS_GPIO_DIR "/unexport", O_WRONLY);
    if (fileDescriptor < 0) {
        char errorBuffer[128] ;
        snprintf(errorBuffer,sizeof(errorBuffer), "gpioUnexport unable to open gpio%d",gpio) ;
        //perror(errorBuffer);
        return fileDescriptor;
    }

    length = snprintf(commandBuffer, sizeof(commandBuffer), "%d", gpio);
    if (write(fileDescriptor, commandBuffer, length) != length) {
        //perror("gpioUnexport") ;
        return fileDescriptor ;
    }
    close(fileDescriptor);
    return 0;
}

// gpioSetDirection
// Set the direction of the GPIO pin 
// Return: Success = 0 ; otherwise open file error
int gpioSetDirection ( jetsonGPIO gpio, unsigned int out_flag )
{
    int fileDescriptor;
    char commandBuffer[MAX_BUF];

    snprintf(commandBuffer, sizeof(commandBuffer), SYSFS_GPIO_DIR  "/gpio%d/direction", gpio);

    fileDescriptor = open(commandBuffer, O_WRONLY);
    if (fileDescriptor < 0) {
        char errorBuffer[128] ;
        snprintf(errorBuffer,sizeof(errorBuffer), "gpioSetDirection unable to open gpio%d",gpio) ;
        //perror(errorBuffer);
        return fileDescriptor;
    }

    if (out_flag) {
        if (write(fileDescriptor, "out", 4) != 4) {
            perror("gpioSetDirection") ;
            return fileDescriptor ;
        }
    }
    else {
        if (write(fileDescriptor, "in", 3) != 3) {
            perror("gpioSetDirection") ;
            return fileDescriptor ;
        }
    }
    close(fileDescriptor);
    return 0;
}

//
// gpioSetValue
// Set the value of the GPIO pin to 1 or 0
// Return: Success = 0 ; otherwise open file error
int gpioSetValue ( jetsonGPIO gpio, unsigned int value )
{
    int fileDescriptor;
    char commandBuffer[MAX_BUF];

    snprintf(commandBuffer, sizeof(commandBuffer), SYSFS_GPIO_DIR "/gpio%d/value", gpio);

    fileDescriptor = open(commandBuffer, O_WRONLY);
    if (fileDescriptor < 0) {
        char errorBuffer[128] ;
        snprintf(errorBuffer,sizeof(errorBuffer), "gpioSetValue unable to open gpio%d",gpio) ;
        //perror(errorBuffer);
        return fileDescriptor;
    }

    if (value) {
        if (write(fileDescriptor, "1", 2) != 2) {
            perror("gpioSetValue") ;
            return fileDescriptor ;
        }
    }
    else {
        if (write(fileDescriptor, "0", 2) != 2) {
            perror("gpioSetValue") ;
            return fileDescriptor ;
        }
    }
    close(fileDescriptor);
    return 0;
}

//
// gpioGetValue
// Get the value of the requested GPIO pin ; value return is 0 or 1
// Return: Success = 0 ; otherwise open file error
int gpioGetValue (jetsonGPIO gpio)
{
    int value;
    int fileDescriptor;
    char commandBuffer[MAX_BUF];
    char ch;

    snprintf(commandBuffer, sizeof(commandBuffer), SYSFS_GPIO_DIR "/gpio%d/value", gpio);

    fileDescriptor = open(commandBuffer, O_RDONLY);
    if (fileDescriptor < 0) {
        char errorBuffer[128] ;
        snprintf(errorBuffer,sizeof(errorBuffer), "gpioGetValue unable to open gpio%d",gpio) ;
        //perror(errorBuffer);
        return fileDescriptor;
    }

    if (read(fileDescriptor, &ch, 1) != 1) {
        //perror("gpioGetValue") ;
        return fileDescriptor ;
     }

    if (ch != '0') {
        value = 1;
    } else {
        value = 0;
    }

    close(fileDescriptor);
    return value;
}


//
// gpioSetEdge
// Set the edge of the GPIO pin
// Valid edges: 'none' 'rising' 'falling' 'both'
// Return: Success = 0 ; otherwise open file error
int gpioSetEdge ( jetsonGPIO gpio, char *edge )
{
    int fileDescriptor;
    char commandBuffer[MAX_BUF];

    snprintf(commandBuffer, sizeof(commandBuffer), SYSFS_GPIO_DIR "/gpio%d/edge", gpio);

    fileDescriptor = open(commandBuffer, O_WRONLY);
    if (fileDescriptor < 0) {
        char errorBuffer[128] ;
        snprintf(errorBuffer,sizeof(errorBuffer), "gpioSetEdge unable to open gpio%d",gpio) ;
        //perror(errorBuffer);
        return fileDescriptor;
    }

    if (write(fileDescriptor, edge, strlen(edge) + 1) != ((int)(strlen(edge) + 1))) {
        //perror("gpioSetEdge") ;
        return fileDescriptor ;
    }
    close(fileDescriptor);
    return 0;
}

//
// gpioOpen
// Open the given pin for reading
// Returns the file descriptor of the named pin
int gpioOpen( jetsonGPIO gpio )
{
    int fileDescriptor;
    char commandBuffer[MAX_BUF];

    snprintf(commandBuffer, sizeof(commandBuffer), SYSFS_GPIO_DIR "/gpio%d/value", gpio);

    fileDescriptor = open(commandBuffer, O_RDONLY | O_NONBLOCK );
    if (fileDescriptor < 0) {
        char errorBuffer[128] ;
        snprintf(errorBuffer,sizeof(errorBuffer), "gpioOpen unable to open gpio%d",gpio) ;
        //perror(errorBuffer);
    }
    return fileDescriptor;
}

//
// gpioClose
// Close the given file descriptor 
int gpioClose ( int fileDescriptor )
{
    return close(fileDescriptor);
}

// gpioActiveLow
// Set the active_low attribute of the GPIO pin to 1 or 0
// Return: Success = 0 ; otherwise open file error
int gpioActiveLow ( jetsonGPIO gpio, unsigned int value )
{
    int fileDescriptor;
    char commandBuffer[MAX_BUF];

    snprintf(commandBuffer, sizeof(commandBuffer), SYSFS_GPIO_DIR "/gpio%d/active_low", gpio);

    fileDescriptor = open(commandBuffer, O_WRONLY);
    if (fileDescriptor < 0) {
        char errorBuffer[128] ;
        snprintf(errorBuffer,sizeof(errorBuffer), "gpioActiveLow unable to open gpio%d",gpio) ;
        //perror(errorBuffer);
        return fileDescriptor;
    }

    if (value) {
        if (write(fileDescriptor, "1", 2) != 2) {
            //perror("gpioActiveLow") ;
            return fileDescriptor ;
        }
    }
    else {
        if (write(fileDescriptor, "0", 2) != 2) {
            //perror("gpioActiveLow") ;
            return fileDescriptor ;
        }
    }
    close(fileDescriptor);
    return 0;
}

//
// PWM config - PWM0, Pino 33
//
// Abre o PWM
int gpioPwmExport ()
{
    int fileDescriptor, length;
    char commandBuffer[MAX_BUF];

    fileDescriptor = open("/sys/class/pwm/pwmchip0/export", O_WRONLY);
    //printf("filedescriptor %d ", fileDescriptor);
    if (fileDescriptor < 0) {
        char errorBuffer[128] ;
        snprintf(errorBuffer,sizeof(errorBuffer), "gpioPwmExport unable to open pwm") ;
        //perror(errorBuffer);
        return fileDescriptor;
    }

    length = snprintf(commandBuffer, sizeof(commandBuffer), "0");
    if (write(fileDescriptor, commandBuffer, length) != length) {
        //perror("gpioExport");
        return fileDescriptor ;

    }
    close(fileDescriptor);

    return 0;
}

//Fecha o PWM
int gpioPwmUnexport ()
{
    int fileDescriptor, length;
    char commandBuffer[MAX_BUF];

    fileDescriptor = open("/sys/class/pwm/pwmchip0/unexport", O_WRONLY);
    if (fileDescriptor < 0) {
        char errorBuffer[128] ;
        snprintf(errorBuffer,sizeof(errorBuffer), "gpioPwmUnexport unable to open pwm") ;
        //perror(errorBuffer);
        return fileDescriptor;
    }

    length = snprintf(commandBuffer, sizeof(commandBuffer), "0");
    if (write(fileDescriptor, commandBuffer, length) != length) {
        //perror("gpioUnexport") ;
        return fileDescriptor ;
    }
    close(fileDescriptor);
    return 0;
}

//Seta o DutyCycle do PWM
int gpioSetPwmDutyCycle ( unsigned int value )
{
    int fileDescriptor;
    char commandBuffer[MAX_BUF];
    char valueSt[MAX_BUF];

    snprintf(commandBuffer, sizeof(commandBuffer), "/sys/class/pwm/pwmchip0/pwm0/duty_cycle");

    fileDescriptor = open(commandBuffer, O_WRONLY);
    if (fileDescriptor < 0) {
        char errorBuffer[128] ;
        snprintf(errorBuffer,sizeof(errorBuffer), "gpioSetPwmDutyCicle unable to open pwm") ;
        //perror(errorBuffer);
        return fileDescriptor;
    }
    
    sprintf(valueSt, "%d", value);

    if (write(fileDescriptor, valueSt, sizeof(valueSt)) != 2) {
        //perror("gpioSetPwmDutyCicle") ;
        return fileDescriptor ;
    }

    close(fileDescriptor);
    return 0;
}

//Le o DutyCycle do PWM
int gpioGetPwmDutyCycle ()
{
    int value = 0;
    int fileDescriptor;
    char commandBuffer[MAX_BUF];
    char ch[MAX_BUF];

    snprintf(commandBuffer, sizeof(commandBuffer), "/sys/class/pwm/pwmchip0/pwm0/duty_cycle");

    fileDescriptor = open(commandBuffer, O_RDONLY);
    if (fileDescriptor < 0) {
        char errorBuffer[128] ;
        snprintf(errorBuffer,sizeof(errorBuffer), "gpioGetPwmDutyCicle unable to open pwm") ;
        //perror(errorBuffer);
        return fileDescriptor;
    }

    if (read(fileDescriptor, &ch, sizeof(ch)) != 1) {
        //perror("gpioGetPwmDutyCicle") ;
     }

    //Converte o valor para int
    int i,j;
    for(i=0; ch[i]!='\n'; i++); //Verifica o tamanho da string
    for(j=0; j<i; j++){ //Converte e calcula valor inteiro
        value = (((int)ch[j])-48)*pow(10,i-1-j)+value;
    }
    
    return value;
}

//Seta o período do PWM
int gpioSetPwmPeriod ( unsigned int value )
{
    int fileDescriptor;
    char commandBuffer[MAX_BUF];
    char valueSt[MAX_BUF];

    snprintf(commandBuffer, sizeof(commandBuffer), "/sys/class/pwm/pwmchip0/pwm0/period");

    fileDescriptor = open(commandBuffer, O_WRONLY);
    if (fileDescriptor < 0) {
        char errorBuffer[128] ;
        snprintf(errorBuffer,sizeof(errorBuffer), "gpioSetPwmPeriod unable to open pwm") ;
        //perror(errorBuffer);
        return fileDescriptor;
    }
    
    sprintf(valueSt, "%d", value);

    if (write(fileDescriptor, valueSt, sizeof(valueSt)) != 2) {
        //perror("gpioSetPwmPeriod") ;
        return fileDescriptor ;
    }

    close(fileDescriptor);
    return 0;
}

//Le o período do PWM
int gpioGetPwmPeriod ()
{
    int value = 0;
    int fileDescriptor;
    char commandBuffer[MAX_BUF];
    char ch[MAX_BUF];

    snprintf(commandBuffer, sizeof(commandBuffer), "/sys/class/pwm/pwmchip0/pwm0/period");

    fileDescriptor = open(commandBuffer, O_RDONLY);
    if (fileDescriptor < 0) {
        char errorBuffer[128] ;
        snprintf(errorBuffer,sizeof(errorBuffer), "gpioGetPwmPeriod unable to open pwm") ;
        //perror(errorBuffer);
        return fileDescriptor;
    }

    if (read(fileDescriptor, &ch, sizeof(ch)) != 1) {
        //perror("gpioGetPwmPeriod") ;
     }

    //Converte o valor para int
    int i,j;
    for(i=0; ch[i]!='\n'; i++); //Verifica o tamanho da string
    for(j=0; j<i; j++){ //Converte e calcula valor inteiro
        value = (((int)ch[j])-48)*pow(10,i-1-j)+value;
    }
    
    return value;
}

//Habilita ou desabilita o PWM
int gpioSetPwmStatus ( unsigned int value )
{
    int fileDescriptor;
    char commandBuffer[MAX_BUF];

    snprintf(commandBuffer, sizeof(commandBuffer), "/sys/class/pwm/pwmchip0/pwm0/enable");

    fileDescriptor = open(commandBuffer, O_WRONLY);
    if (fileDescriptor < 0) {
        char errorBuffer[128] ;
        snprintf(errorBuffer,sizeof(errorBuffer), "gpioSetPwmStatus unable to open pwm") ;
        //perror(errorBuffer);
        return fileDescriptor;
    }

    if (value) {
        if (write(fileDescriptor, "1", 2) != 2) {
            //perror("gpioSetPwmStatus") ;
            return fileDescriptor ;
        }
    }
    else {
        if (write(fileDescriptor, "0", 2) != 2) {
            //perror("gpioSetPwmStatus") ;
            return fileDescriptor ;
        }
    }
    close(fileDescriptor);
    return 0;
}

//Le o estado do PWM
int gpioGetPwmStatus ()
{
    int value;
    int fileDescriptor;
    char commandBuffer[MAX_BUF];
    char ch;

    snprintf(commandBuffer, sizeof(commandBuffer), "/sys/class/pwm/pwmchip0/pwm0/enable");

    fileDescriptor = open(commandBuffer, O_RDONLY);
    if (fileDescriptor < 0) {
        char errorBuffer[128] ;
        snprintf(errorBuffer,sizeof(errorBuffer), "gpioGetPwmStatus unable to open pwm") ;
        //perror(errorBuffer);
        return fileDescriptor;
    }

    if (read(fileDescriptor, &ch, 1) != 1) {
        //perror("gpioGetPwmStatus") ;
        return fileDescriptor ;
     }

    if (ch != '0') {
        value = 1;
    } else {
        value = 0;
    }

    close(fileDescriptor);
    return value;
}

