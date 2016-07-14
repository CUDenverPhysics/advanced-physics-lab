/* Arduino sketch to communicate with AMT-203 rotarty encoder via SPI bus. 
 * It converts the binary bit stream into a decimal degree and prints to serial.
 * 
 * Author: Kristopher Bunker
 * Date: 2016 July 12
 */
 
#include <SPI.h>

// DECLARE VARIABLES HERE
const int CS = 10; // Slave select pin 
uint8_t temp[3]; // An array that will be used to receive data from the AMT-203 encoder
float degree; // This is used to convert the binary data to a degree between 0 and 360

// SETUP HERE - RUN ONCE AT INITIALIZATION
void setup()
{
    pinMode(CS,OUTPUT); // Specify SPI Slave Select pin
    SPI.begin(); // Start transmission
    SPI.setBitOrder(MSBFIRST); // Specify SPI bit order
    SPI.setDataMode(SPI_MODE0); // Specify SPI data mode
    //SPI.setClockDivider(SPI_CLOCK_DIV2); // Specify SPI clock divider - comment out for no division
    Serial.begin(250000); // Specify serial baud rate
    Serial.flush(); // Flush the serial buffer
    SPI.end(); // End transmission
}

// FUNCTION TO COMMUNICATE WITH AMT-203 ENCODER
uint8_t SPI_T (uint8_t msg) 
{
    uint8_t msg_temp = 0; // Vairable to hold recieved data
    digitalWrite(CS,LOW); // Select SPI device
    msg_temp = SPI.transfer(msg); // Send and recieve
    digitalWrite(CS,HIGH); // Deselect SPI device
    return(msg_temp); // Return recieved byte
}

// MAIN LOOP
void loop()
{ 
    SPI.begin(); // start transmition

    SPI_T(0x10); //issue read command
    temp[2] = SPI_T(0x00); // Issue no operation command - receive null statement
    temp[0] = SPI_T(0x00); // Issue no operation command - receive MSB   
    temp[1] = SPI_T(0x00); // Issue no operation command - receive LSB
    
    SPI.end(); // end transmition

    degree=((temp[0]*256)+temp[1])*.08789; // Convert received binary bitstreams into degree [360/4096 = 0.08789]

    // Some of the data received is erroneous - this will only print good data to the serial port
    if ( (degree >= 0) && (degree <= 360) )
    {
      Serial.println(degree, DEC); // Print degree to serial port to a new line in decimal format
    }
}
