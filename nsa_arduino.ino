/* Program för ArduinoUno 
 * Sänder 3 temperaturer till Telldus på 3 olika Id
 * Skriver även temperatur på 2x16LCD display
 * Temperatursensor LM335 kopplad till A1,A2,A3
 * 433MHz sändare kopplad till pin 2
 * Tack till Oliver Lebrun för Encoder software
 */


/*
 * connectingStuff, Oregon Scientific v2.1 Emitter
 * http://connectingstuff.net/blog/encodage-protocoles-oregon-scientific-sur-arduino/
 *
 * Copyright (C) 2013 olivier.lebrun@gmail.com
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/
 
//#define THN132N  //enbart termometer, ingen humidity
 
const byte TX_PIN = 2;
 
const unsigned long TIME = 512;
const unsigned long TWOTIME = TIME*2;
 
#define SEND_HIGH() digitalWrite(TX_PIN, HIGH)
#define SEND_LOW() digitalWrite(TX_PIN, LOW)
 
// Buffer for Oregon message
#ifdef THN132N
  byte OregonMessageBuffer[8];
#else
  byte OregonMessageBuffer[9];
#endif
 
/**
 * \brief    Send logical "0" over RF
 * \details  azero bit be represented by an off-to-on transition
 * \         of the RF signal at the middle of a clock period.
 * \         Remenber, the Oregon v2.1 protocol add an inverted bit first 
 */
inline void sendZero(void) 
{
  SEND_HIGH();
  delayMicroseconds(TIME);
  SEND_LOW();
  delayMicroseconds(TWOTIME);
  SEND_HIGH();
  delayMicroseconds(TIME);
}
 
/**
 * \brief    Send logical "1" over RF
 * \details  a one bit be represented by an on-to-off transition
 * \         of the RF signal at the middle of a clock period.
 * \         Remenber, the Oregon v2.1 protocol add an inverted bit first 
 */
inline void sendOne(void) 
{
   SEND_LOW();
   delayMicroseconds(TIME);
   SEND_HIGH();
   delayMicroseconds(TWOTIME);
   SEND_LOW();
   delayMicroseconds(TIME);
}
 
/**
* Send a bits quarter (4 bits = MSB from 8 bits value) over RF
*
* @param data Source data to process and sent
*/
 
/**
 * \brief    Send a bits quarter (4 bits = MSB from 8 bits value) over RF
 * \param    data   Data to send
 */
inline void sendQuarterMSB(const byte data) 
{
  (bitRead(data, 4)) ? sendOne() : sendZero();
  (bitRead(data, 5)) ? sendOne() : sendZero();
  (bitRead(data, 6)) ? sendOne() : sendZero();
  (bitRead(data, 7)) ? sendOne() : sendZero();
}
 
/**
 * \brief    Send a bits quarter (4 bits = LSB from 8 bits value) over RF
 * \param    data   Data to send
 */
inline void sendQuarterLSB(const byte data) 
{
  (bitRead(data, 0)) ? sendOne() : sendZero();
  (bitRead(data, 1)) ? sendOne() : sendZero();
  (bitRead(data, 2)) ? sendOne() : sendZero();
  (bitRead(data, 3)) ? sendOne() : sendZero();
}
 
/******************************************************************/
/******************************************************************/
/******************************************************************/
 
/**
 * \brief    Send a buffer over RF
 * \param    data   Data to send
 * \param    size   size of data to send
 */
void sendData(byte *data, byte size)
{
  for(byte i = 0; i < size; ++i)
  {
    sendQuarterLSB(data[i]);
    sendQuarterMSB(data[i]);
  }
}
 
/**
 * \brief    Send an Oregon message
 * \param    data   The Oregon message
 */
void sendOregon(byte *data, byte size)
{
    sendPreamble();
    //sendSync();
    sendData(data, size);
    sendPostamble();
}
 
/**
 * \brief    Send preamble
 * \details  The preamble consists of 16 "1" bits
 */
inline void sendPreamble(void)
{
  byte PREAMBLE[]={0xFF,0xFF};
  sendData(PREAMBLE, 2);
}
 
/**
 * \brief    Send postamble
 * \details  The postamble consists of 8 "0" bits
 */
inline void sendPostamble(void)
{
#ifdef THN132N
  sendQuarterLSB(0x00);
#else
  byte POSTAMBLE[]={0x00};
  sendData(POSTAMBLE, 1);  
#endif
}
 
/**
 * \brief    Send sync nibble
 * \details  The sync is 0xA. It is not use in this version since the sync nibble
 * \         is include in the Oregon message to send.
 */
inline void sendSync(void)
{
  sendQuarterLSB(0xA);
}
 
/******************************************************************/
/******************************************************************/
/******************************************************************/
 
/**
 * \brief    Set the sensor type
 * \param    data       Oregon message
 * \param    type       Sensor type
 */
inline void setType(byte *data, byte* type) 
{
  data[0] = type[0];
  data[1] = type[1];
}
 
/**
 * \brief    Set the sensor channel
 * \param    data       Oregon message
 * \param    channel    Sensor channel (0x10, 0x20, 0x30)
 */
inline void setChannel(byte *data, byte channel) 
{
    data[2] = channel;
}
 
/**
 * \brief    Set the sensor ID
 * \param    data       Oregon message
 * \param    ID         Sensor unique ID
 */
inline void setId(byte *data, byte ID) 
{
  data[3] = ID;
}
 
/**
 * \brief    Set the sensor battery level
 * \param    data       Oregon message
 * \param    level      Battery level (0 = low, 1 = high)
 */
void setBatteryLevel(byte *data, byte level)
{
  if(!level) data[4] = 0x0C;
  else data[4] = 0x00;
}
 
/**
 * \brief    Set the sensor temperature
 * \param    data       Oregon message
 * \param    temp       the temperature
 */
void setTemperature(byte *data, float temp) 
{
  // Set temperature sign
  if(temp < 0)
  {
    data[6] = 0x08;
    temp *= -1;  
  }
  else
  {
    data[6] = 0x00;
  }
 
  // Determine decimal and float part
  int tempInt = (int)temp;
  int td = (int)(tempInt / 10);
  int tf = (int)round((float)((float)tempInt/10 - (float)td) * 10);
 
  int tempFloat =  (int)round((float)(temp - (float)tempInt) * 10);
 
  // Set temperature decimal part
  data[5] = (td << 4);
  data[5] |= tf;
 
  // Set temperature float part
  data[4] |= (tempFloat << 4);
}
 
/**
 * \brief    Set the sensor humidity
 * \param    data       Oregon message
 * \param    hum        the humidity
 */
void setHumidity(byte* data, byte hum)
{
    data[7] = (hum/10);
    data[6] |= (hum - data[7]*10) << 4;
}
 
/**
 * \brief    Sum data for checksum
 * \param    count      number of bit to sum
 * \param    data       Oregon message
 */
int Sum(byte count, const byte* data)
{
  int s = 0;
 
  for(byte i = 0; i<count;i++)
  {
    s += (data[i]&0xF0) >> 4;
    s += (data[i]&0xF);
  }
 
  if(int(count) != count)
    s += (data[count]&0xF0) >> 4;
 
  return s;
}
 
/**
 * \brief    Calculate checksum
 * \param    data       Oregon message
 */
void calculateAndSetChecksum(byte* data)
{
#ifdef THN132N
    int s = ((Sum(6, data) + (data[6]&0xF) - 0xa) & 0xff);
 
    data[6] |=  (s&0x0F) << 4;     data[7] =  (s&0xF0) >> 4;
#else
    data[8] = ((Sum(8, data) - 0xa) & 0xFF);
#endif
}
 
/***************** End send 433 *************************************************/
/******************************************************************/
/******************************************************************/
//------- start POJ --------------
#include <LiquidCrystal.h>
LiquidCrystal lcd(8,9,4,5,6,7);

float volt1=0;
float volt2=0;
float volt3=0;
float garTemp1=0;
float inneTemp2=0;
float uteTemp3=0;
int i=0;
// ---------- End POJ ---------------

void setup()
{
//---  Setup send433 ---------
  pinMode(TX_PIN, OUTPUT);
 
  Serial.begin(9600);
  Serial.println("\n[Oregon V2.1 encoder]");
 
  SEND_LOW();  
 
#ifdef THN132N  
  // Create the Oregon message for a temperature only sensor (TNHN132N)
  byte ID[] = {0xEA,0x4C};
#else
  // Create the Oregon message for a temperature/humidity sensor (THGR2228N)
  byte ID[] = {0x1A,0x2D};
#endif  
 
  setType(OregonMessageBuffer, ID);
  setChannel(OregonMessageBuffer, 0x20);
  //setId(OregonMessageBuffer, 0xBB); //BB=187
  // ------ End setup send433 -----------------
  // ------ setup POJ -------------------------
  lcd.begin(16,2);
  lcd.print("Templog 140125");
  delay(1000);
  lcd.clear();
    // -------End setup POJ ---------------------
  
}
//-------------  end void setup -------------------------------------------------

// ************** void  loop ****************************************************
void loop()
{
  //read voltage
 volt1=0;
 volt2=0;
 volt3=0;
 for (int i=0; i<10; i++){
       volt1=volt1 + analogRead(1);
       volt2=volt2 + analogRead(2);
       volt3=volt3 + analogRead(3);
       delay(10+i);}
 volt1=volt1/10; //medel värde på 10 samplingar
 volt1=volt1*5.00/1024; //spänning i volt
 volt2=volt2/10; //medel värde på 10 samplingar
 volt2=volt2*5.00/1024; //spänning i volt
 volt3=volt3/10; //medel värde på 10 samplingar
 volt3=volt3*5.00/1024; //spänning i volt
 
 garTemp1=(volt1-2.73)*100+5; //+5 kompenserar för mätfel pga långa kablar
 inneTemp2=(volt2-2.73)*100+5;
 uteTemp3=(volt3-2.73)*100;
 //output to LCD
 lcd.setCursor (0,0);
 lcd.print("Gar=");
 //lcd.print(volt1,2);
 //lcd.print(" / ");
 lcd.print(garTemp1,1);
 lcd.print(" Ute=");
 lcd.print(uteTemp3,1);
 lcd.setCursor (0,1);
 lcd.print("Inne=");
 //lcd.print(volt2,2);
 //lcd.print(" / ");
 lcd.print(inneTemp2,1);
 
 //out to RS232 
  Serial.print(garTemp1*10,0);
  Serial.print(" garaget   ");             //Skickar :
  send433(garTemp1,01,0xBB);
  delay(1000);
  
  Serial.print(inneTemp2*10,0);
  Serial.print(" inne   ");
  send433(inneTemp2,02,0xBC);
  delay(1000);
  
  Serial.print(uteTemp3*10,0);
  Serial.print(" ute   ");
  send433(uteTemp3,03,0xBD);
  delay(1000);
  
  delay(5000);
}



 //---------------- Send data to Telldus ----------------------------
 void send433(float temperature, byte humidity, byte Identitet)
 {
  // Get Temperature, humidity and battery level from sensors
  // (ie: 1wire DS18B20 for température, ...)
  setId(OregonMessageBuffer, Identitet); //BB=187
  setBatteryLevel(OregonMessageBuffer, 0); // 0 : low, 1 : high
  setTemperature(OregonMessageBuffer, temperature); //org  setTemperature(OregonMessageBuffer, 55.5);
 
#ifndef THN132N
  // Set Humidity
  setHumidity(OregonMessageBuffer, humidity);
#endif  
 
  // Calculate the checksum
  calculateAndSetChecksum(OregonMessageBuffer);
 
  // Show the Oregon Message
  for (byte i = 0; i < sizeof(OregonMessageBuffer); ++i)   {     Serial.print(OregonMessageBuffer[i] >> 4, HEX);
    Serial.print(OregonMessageBuffer[i] & 0x0F, HEX);
  }
    Serial.println();
  // Send the Message over RF
  sendOregon(OregonMessageBuffer, sizeof(OregonMessageBuffer));
  // Send a "pause"
  SEND_LOW();
  delayMicroseconds(TWOTIME*8);
  // Send a copie of the first message. The v2.1 protocol send the
  // message two time 
  sendOregon(OregonMessageBuffer, sizeof(OregonMessageBuffer));
 
  SEND_LOW();
  
 }
 // ---------- End send data till Telldus ---------
