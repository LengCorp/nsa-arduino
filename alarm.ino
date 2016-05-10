
static int wasTriggered = 0;
static int alarmPin;


void alarm_start(const int pin){
  // set interrupt och alarmPin till pin
}

int alarm_triggered()
{
  return wasTriggered;
}


void alarm_reset(){
  wasTriggered = 0;
}

