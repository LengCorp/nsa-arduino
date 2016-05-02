
static int is_triggered = 0;

int alarm_triggered()
{
   if(is_triggered){
     is_triggered = 0;
     return 1;
   } else {
     is_triggered = 1;
     return 0;
   }
}


