#include <Encoder.h>

/*
Test programma voor position chip AS5601
 Leest Raw angle en status regusters.
 Jan Jansen 07-04-2015
 */



#include <Wire.h>
int reading = 0;
byte result;
int encPos;
Encoder AS5601enc(18, 19); 
void setup()
{
  Wire.begin();
  Serial.begin(115200);  // start serial for output
  //************************************************************************
  // SetupQuadrature resolution to max (2048)
  Serial.print("Qres: ");
  Wire.beginTransmission(0x36);       // transmit to device as5601
  Wire.write(byte(0x09));             // sets register pointer to ABN 
  Wire.write(byte(0x0F));             //resolution 2048 
  result = Wire.endTransmission();    // stop transmitting
  //************************************************************************
  // Check SetupQuadrature resolution to max (2048) 
  Wire.beginTransmission(0x36);       // transmit to device as5601
  Wire.write(byte(0x09));             // sets register pointer to ABN 
  result = Wire.endTransmission();    // stop transmitting
  Wire.requestFrom(0x36, 1);          // request 2 bytes from as5601
  reading = Wire.read();              // receive high byte 
  Serial.print(reading,BIN);   
  Serial.println(""); 
  //**************************************************************************
  
  //************************************************************************
  // Quadrature enc with the AS5601 Hall position.
  Serial.print("Read Angle: ");
  Wire.beginTransmission(0x36);       // transmit to device as5601
  Wire.write(byte(0x0E));             // sets register pointer 
  result = Wire.endTransmission();    // stop transmitting

  Wire.requestFrom(0x36, 2);         // request 2 bytes from as5601
  reading = Wire.read();             // receive high byte 
  reading = reading << 8;            // shift high byte to be high 8 bits
  reading |= Wire.read();            // receive low byte as lower 8 bits
  Serial.print(reading);   
  // adjusting  
  AS5601enc.write(reading/2); //(as5601 is 12bit and enc is only 11 bit resolutions)
  delay(5000);
  
  
}

void loop()
{
  //************************************************************************
  // Raw angle
  Serial.print("Angle: ");
  Wire.beginTransmission(0x36);       // transmit to device as5601
  Wire.write(byte(0x0E));             // sets register pointer 
  result = Wire.endTransmission();    // stop transmitting

  Wire.requestFrom(0x36, 2);         // request 2 bytes from as5601
  reading = Wire.read();             // receive high byte 
  reading = reading << 8;            // shift high byte to be high 8 bits
  reading |= Wire.read();            // receive low byte as lower 8 bits
  Serial.print(reading/2);          

  //**************************************************************************
  // Quadrature encoder
  encPos = AS5601enc.read();
  Serial.print(" Encpos: " );
  Serial.print(encPos);
  //**************************************************************************
  // Status
  //Serial.print(" Status: ");        // remove // if like to display the bits
  Wire.beginTransmission(0x36);       // transmit to device as5601
  Wire.write(byte(0x0b));             // sets register pointer 
  result = Wire.endTransmission();    // stop transmitting

  Wire.requestFrom(0x36, 1 );         // request 1 bytes from as5601
  reading = Wire.read();              // receive high byte
  //Serial.print(reading, BIN);       // remove // if like to display the bits
  
  if (bitRead(reading, 5)!=1) { Serial.print(" Status: "); if (bitRead(reading, 4)==1) Serial.print("too high ");if (bitRead(reading, 3)==1) Serial.print("too low ");}
  
   //************************************************************************
// AGC
  Serial.print(" AGC: ");
  Wire.beginTransmission(0x36);       // transmit to device as5601
  Wire.write(byte(0x1a));             // sets register pointer 
  result = Wire.endTransmission();    // stop transmitting

  Wire.requestFrom(0x36, 1);          // request 1 bytes from as5601
  reading = Wire.read();              // receive high byte
  Serial.print(reading, DEC);             
  Serial.println("");
 //************************************************************************

  delay(300);
}
