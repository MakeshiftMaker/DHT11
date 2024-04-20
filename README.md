## DHT11 Rasberry-Pi C Readout

This library can be used to read Humidity and Temperature data from the DHT11 Humidture sensor.
The library is written in C and tested on a Rasberry-Pi 4

## Installation

Clone this repository to your local machine

```git clone https://github.com/MakeshiftMaker/DHT11```

## Usage

Include the DHT11.h header file in your C file:

```#include "DHT11.h"```

Compile your C file and link against the DHT11 library:



```gcc -o your_program your_file.c DHT11.c```

The library comes with the function readDHT(int dhtPin):

the dhtPin being the GPIO pin on your Pi to which youve hooked up your DATA pin on the DHT11. (using the WiringPi GPIO numbering scheme)

The function returns a pointer to an Array which contains Integral and Decimal Humidity/Temperature values and a checksum.

If the checksum doesnt check out, or the program cant read a bit for whatever reason (it happens every now and then) it returns a NULL-pointer

## Usage Example

```
#define DHTPIN 26

for(int i = 0 ; i < 100 ; i++){
        delay(1000); //delay 1 sec for DHT "cooldown"
        int* dht_data = readDHT(DHTPIN);
        if(dht_data == NULL){ //check if something went wrong
            printf("%d: Checksum Error\n", i);
        }
        else{
            printf("%d: Humidity: %d.%d%%\nTemperature: %d.%dC\n", i, dht_data[0], dht_data[1], dht_data[2], dht_data[3]); 
        }
    }
```

this program reads out the values from the Sensor 100 times and prints them if the data is in tact

The datasheet advises to give the Sensor about 1 second between each readout, or else it might not be able to keep up.