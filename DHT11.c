#include <string.h>
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include "DHT11.h"

int* readDHT(int dhtpin){
    int* dht_data = malloc(5*sizeof(int)); // Initialize array to store the data
    memset(dht_data, 0, 5*sizeof(int));
    int bit_position = 7; //start at highest bit
    int byte_position = 0;

    pinMode(dhtpin, OUTPUT);
    digitalWrite(dhtpin, LOW); //MCU START SIGNAL
    delay(18);//start signal must last at least 18 ms for DHT to notice

    digitalWrite(dhtpin, HIGH); //set high and wait for response
    delayMicroseconds(40); //wait for response

    pinMode(dhtpin, INPUT);//DHT pulls data pin to low


    // delayMicroseconds(160);//DHT pulls to low for 80us and up for 80us - transmission start
    while(digitalRead(dhtpin) == LOW){
        delayMicroseconds(1);
    }

    while(digitalRead(dhtpin) == HIGH){
        delayMicroseconds(1);
    }

    for(int i = 0; i < 40; i++){ //read 40 bits
        
        while(digitalRead(dhtpin) == LOW){//delay until high
            delayMicroseconds(1);
        }
        //delayMicroseconds(50);

        int counter = 0;
        while(digitalRead(dhtpin) == HIGH){//count how long pin is high, 27 microseconds means 0, 70 microseconds means 1
            delayMicroseconds(1);
            counter++;
            if(counter > 80){
                //printf("stuck on start\n");
                return NULL; //something went horribly wrong, skip
            }
        }
        //printf("%d\n", counter);

        if(counter >= 30){//if higher than 30, assume its 1
            int bit = 1<<bit_position; //we gotta left-shift the bit by 7 decreasing because we are working with MSB first
            //printf("BIT: %d\n", bit);
            //printf("1");
            dht_data[byte_position] |= bit;
            bit_position--;
            
        }
        else{ //else its 0
            bit_position--;//0 we aint gotta do SHIT
            //printf("0");
        }

        if(bit_position < 0){
            bit_position = 7;
            byte_position++;
            //printf("\n");
        }

        int tempCount = 0;
        while(digitalRead(dhtpin) == HIGH){ //wait until low again
            delayMicroseconds(1);
            tempCount++;
            if(tempCount > 100){
                //printf("stuck on end\n");
                return NULL; //something went horribly wrong, skip
            }
        }
    }

    if((dht_data[4] == ( (dht_data[0] + dht_data[1] + dht_data[2] + dht_data[3]) & 0xFF) )){
            return dht_data;
        }
        else{
            return NULL;
        }
}