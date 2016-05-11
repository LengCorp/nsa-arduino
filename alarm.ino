
static int wasTriggered = 0;

void alarm_start(const int pin){
  // set interrupt och alarmPin till pin
    attachInterrupt(digitalPinToInterrupt(pin), alarm_interrupt, FALLING);
  
}

int alarm_triggered()
{
  return wasTriggered;
}

void alarm_reset(){
  wasTriggered = 0;
}
void alarm_interrupt(){
  wasTriggered = 1;
  }
