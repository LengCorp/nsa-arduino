/*
 * nsa_arduino
 */
 
byte OregonMessageBuffer[8];

const byte TX_PIN = 2;
const unsigned long TIME = 512;
const unsigned long TWOTIME = TIME*2;
 
#define SEND_HIGH() digitalWrite(TX_PIN, HIGH)
#define SEND_LOW() digitalWrite(TX_PIN, LOW)

const float TRIGGER_TEMP = -27;
const float IS_ON_TEMP = 90;
const float BROKEN_TEMP = 60;

void setup()
{
//---  Setup send433 ---------
  pinMode(TX_PIN, OUTPUT);
 
  Serial.begin(9600);
  Serial.println("\n[Oregon V2.1 encoder]");
 
  SEND_LOW();
 

  // Create the Oregon message for a temperature only sensor (TNHN132N)
  byte ID[] = {0xEA,0x4C};

 
  setType(OregonMessageBuffer, ID);
  setChannel(OregonMessageBuffer, 0x20);
  //setId(OregonMessageBuffer, 0xBB); //BB=187
  // ------ End setup send433 -----------------
  // ------ setup POJ -------------------------
  
}

// ************** void  loop ****************************************************
void loop()
{ 
  if (alarm_trigered()){
    send433(TRIGGER_TEMP);
  } else {
    send433(IS_ON_TEMP);
  }
  
  delay(4000);
}



