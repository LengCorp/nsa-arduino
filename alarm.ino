
static int is_triggered = 0;

int alarm_triggered()
{
   if(is_on){
     is_on = 0;
     return 1;
   } else {
     is_on = 1;
     return 0;
   }
}


