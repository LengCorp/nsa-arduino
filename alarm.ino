
static int wasTriggered = 0;
static int alarmPin;


int start(const int pin){
  // set interrupt och alarmPin till pin
}

int alarm_triggered()
{
  return wasTriggered;
}


int alarm_reset(){
  wasTriggered = 0;
}

