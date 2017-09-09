#include "freertps/timer.h"

#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "portmacro.h"
#include "osi.h"

freertps_timer_cb_t int_timer_cb = NULL;
TickType_t xFrequency = 1000;

void freertps_timer(void *p)
{
  TickType_t lastTimeRun = xTaskGetTickCount ();
  for(;;)
  {
    vTaskDelayUntil( &lastTimeRun, xFrequency );
    int_timer_cb();
  }
}

void freertps_timer_set_freq(uint32_t freq, freertps_timer_cb_t cb)
{
    int_timer_cb = cb;
    xFrequency = freq*100;
    osi_TaskCreate( freertps_timer, ( signed portCHAR * ) "TASK_READ", 1024, NULL, 9, NULL);
}
