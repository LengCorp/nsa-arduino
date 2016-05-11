
static int wasTriggered = 0;

/**
 * Initializes the alarm to use a specific pin 
 */
void alarm_start(const int pin)
{
    attachInterrupt(digitalPinToInterrupt(pin), alarm_interrupt, RISING);
}

/**
 * Checks if the alarm has been triggered since last check 
 */
int alarm_triggered()
{
  return wasTriggered;
}

/**
 * Resets the alarm to untriggered state
 */
void alarm_reset()
{
  wasTriggered = 0;
}

void alarm_interrupt()
{
  wasTriggered = 1;
}
