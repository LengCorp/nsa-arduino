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
  
  oregon_init(TX_PIN);
  
  Serial.begin(9600);
  Serial.println("\nNSA UNO v. 1.0");
 
  alarm_start(ALARM_PIN);
}

// ************** void  loop ****************************************************
void loop()
{ 
  if (alarm_triggered()){
    alarm_reset();
    oregon_send433(TRIGGER_TEMP, 0xBB);
  } else {
    oregon_send433(IS_ON_TEMP, 0xBB);
  }
  
  delay(4000);
}
