/* Arduino sketch to communicate with AMT-203 rotarty encoder and Honeywell  
 * HSCDANN1.6BASA5 pressure sensor via SPI bus. 
 *  
 * It converts the encoder binary bit stream into a decimal degree and prints to serial.
 * 
 * It also converts the pressure sensor bit stream to pressure (bar) and temperature (C).
 * 
 * Used for Stirling Engine Lab at CU Denver Physics.
 * 
 * Author: Kristopher Bunker
 * Date: 2016 July 14
 */
 
#include <SPI.h>

// DECLARE VARIABLES HERE
const int ENCODER = 10; // Slave select pin for encoder
const int PRESSURE = 9; // Slave select pin for pressure sensor
uint8_t temp[3]; // An array that will be used to receive data from the AMT-203 encoder
float degree; // This is used to convert the binary data to a degree between 0 and 360

// SETUP HERE - RUN ONCE AT INITIALIZATION
void setup()
{
    pinMode(ENCODER,OUTPUT); // Specify SPI Slave Select pin for encoder
    pinMode(PRESSURE, OUTPUT); // Specify SPI Slave Select pin for pressure sensor
    digitalWrite(ENCODER, HIGH); // Make sure SPI device is not selected
    digitalWrite(PRESSURE, HIGH); // Make sure SPI device is not selected
    SPI.begin(); // Start transmission
    SPI.setBitOrder(MSBFIRST); // Specify SPI bit order
    SPI.setDataMode(SPI_MODE0); // Specify SPI data mode
    //SPI.setClockDivider(SPI_CLOCK_DIV2); // Specify SPI clock divider - comment out for no division
    Serial.begin(250000); // Specify serial baud rate
    Serial.flush(); // Flush the serial buffer
    SPI.end(); // End transmission
}

// FUNCTION TO COMMUNICATE WITH AMT-203 ENCODER
uint8_t ENCODER_T (uint8_t msg) 
{
    uint8_t msg_temp = 0; // Vairable to hold recieved data
    digitalWrite(ENCODER,LOW); // Select SPI device
    msg_temp = SPI.transfer(msg); // Send and recieve
    digitalWrite(ENCODER,HIGH); // Deselect SPI device
    return(msg_temp); // Return recieved byte
}

// FUNCTION TO COMMUNICATE WITH HONEYWELL HSCDANN1.6BASA5 PRESSURE SENSOR
void PRESSURE_T (uint8_t msg)
{
    uint8_t msg_temp = 0; // Vairable to hold recieved data
    SPI.begin(); // start transmition
    digitalWrite(PRESSURE,LOW); // Select SPI device
    int inByte_1 = SPI.transfer(msg);
    int inByte_2 = SPI.transfer(msg);
    int inByte_3 = SPI.transfer(msg);
    int inByte_4 = SPI.transfer(msg);
    digitalWrite(PRESSURE,HIGH); // Deselect SPI device
    SPI.end(); // end transmition
    float out = inByte_1<<8|inByte_2;
    float bar = ((out - 1638)*1.6)/(14745-1638); // Transfer function defined in data sheet
    inByte_3 = inByte_3<<3;
    float realTemp = ((float)inByte_3*200/2047)-50; // Transfer function defined in data sheet
    Serial.print(bar);Serial.print(" ");
    Serial.print(realTemp);Serial.print(" ");
    delay(1);
}

// MAIN LOOP
void loop()
{ 
    PRESSURE_T(0X00); // Call the 
    
    SPI.begin(); // start transmition
    
    ENCODER_T(0x10); //issue read command
    temp[2] = ENCODER_T(0x00); // Issue no operation command - receive null statement
    temp[0] = ENCODER_T(0x00); // Issue no operation command - receive MSB   
    temp[1] = ENCODER_T(0x00); // Issue no operation command - receive LSB
            
    degree=((temp[0]*256)+temp[1])*.08789; // Convert received binary bitstreams into degree [360/4096 = 0.08789]

    // Some of the data received from the encoder is erroneous - this will only print good data to the serial port
    if ( (degree >= 0) && (degree <= 360) )
    {
      Serial.println(degree, DEC); // Print degree to serial port to a new line in decimal format
    }        
}
