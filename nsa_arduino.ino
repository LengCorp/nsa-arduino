/*
 * nsa_arduino
 */

byte OregonMessageBuffer[8];

const byte TX_PIN = 2;
const byte ALARM_PIN = 3;

const float TRIGGER_TEMP = -27;
const float IS_ON_TEMP = 90;
const float BROKEN_TEMP = 60;



void setup()
{
//---  Setup send433 ---------
  pinMode(TX_PIN, OUTPUT);
 
  Serial.begin(9600);
  Serial.println("\n[Oregon V2.1 encoder]");
 
  // Make sure it starts at the low frequency 
  digitalWrite(TX_PIN, LOW);
 
  // Create the Oregon message fo<<<<<<<<<r a temperature only sensor (TNHN132N)
  byte ID[] = {0xEA,0x4C};

 
  setType(OregonMessageBuffer, ID);
  setChannel(OregonMessageBuffer, 0x20);

  alarm_start(ALARM_PIN);
}

// ************** void  loop ****************************************************
void loop()
{ 
  if (alarm_triggered()){
    alarm_reset();
    send433(TRIGGER_TEMP, 0xBB);
  } else {
    send433(IS_ON_TEMP, 0xBB);
  }
  
  delay(4000);
}
