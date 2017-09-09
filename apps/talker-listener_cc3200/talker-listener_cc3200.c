
// Standard includes.
#include <stdio.h>
#include <stdlib.h>

// Free-RTOS includes
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "portmacro.h"
#include "osi.h"

// Simplelink includes
#include "simplelink.h"

// Driverlib includes
#include "hw_memmap.h"
#include "hw_common_reg.h"
#include "hw_types.h"
#include "hw_ints.h"
#include "interrupt.h"

#include "rom.h"
#include "rom_map.h"

// Common interface includes
#include "uart.h"
#include "prcm.h"
#include "utils.h"
#include "network_if.h"
#include "uart_if.h"
#include "gpio_if.h"
#include "common.h"

#include "pinmux.h"
#include "bma222.h"

#include "freertps/freertps.h"
#include "freertps/timer.h"

//#define MODE_IMU
#define MODE_TALKER
//#define MODE_LISTENER
//#define MODE_MSG

// Messages
#ifdef MODE_IMU
#include "sensor_msgs/imu.h"
#endif

#ifdef MODE_MSG
#include "std_msgs/string.h"
#endif

//*****************************************************************************
//                      MACRO DEFINITIONS 1024
//*****************************************************************************
#define APPLICATION_VERSION     "1.1.0"
#define UART_PRINT              Report
//#define SPAWN_TASK_PRIORITY     5
#define OSI_STACK_SIZE          2048
#define APP_NAME                "FreeRTOS-FreeRTSP"

//*****************************************************************************
//                 GLOBAL VARIABLES -- Start
//*****************************************************************************
// The queue used to send strings to the task1.
QueueHandle_t xPrintQueue;

#if defined(ccs) || defined(gcc)
extern void (* const g_pfnVectors[])(void);
#endif
#if defined(ewarm)
extern uVectorEntry __vector_table;
#endif

//*****************************************************************************
//                 GLOBAL VARIABLES -- End
//*****************************************************************************


//*****************************************************************************
//                      LOCAL FUNCTION DEFINITIONS
//*****************************************************************************
static void BoardInit();


#ifdef USE_FREERTOS
//*****************************************************************************
// FreeRTOS User Hook Functions enabled in FreeRTOSConfig.h
//*****************************************************************************

//*****************************************************************************
//
//! \brief Application defined hook (or callback) function - assert
//!
//! \param[in]  pcFile - Pointer to the File Name
//! \param[in]  ulLine - Line Number
//!
//! \return none
//!
//*****************************************************************************
void
vAssertCalled( const char *pcFile, unsigned long ulLine )
{
  //Handle Assert here
  while(1)
  {
  }
}

//*****************************************************************************
//
//! \brief Application defined idle task hook
//!
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
void
vApplicationIdleHook( void)
{
    //Handle Idle Hook for Profiling, Power Management etc
}

//*****************************************************************************
//
//! \brief Application defined malloc failed hook
//!
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
void vApplicationMallocFailedHook()
{
  //Handle Memory Allocation Errors
  while(1)
  {
  }
}

//*****************************************************************************
//
//! \brief Application defined stack overflow hook
//!
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
void vApplicationStackOverflowHook( OsiTaskHandle *pxTask,
                                   signed char *pcTaskName)
{
  //Handle FreeRTOS Stack Overflow
  while(1)
  {
  }
}
#endif //USE_FREERTOS


//*****************************************************************************
//
//! Application startup display on UART
//!
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
static void
DisplayBanner(char * AppName)
{

  Report("\r\n\r\n");
  Report("\t\t *************************************************\r\n");
  Report("\t\t    CC3200 %s Application       \r\n", AppName);
  Report("\t\t *************************************************\r\n");
  Report("Boot AlfredOS...\r\n\r\n\r\n");
}

//*****************************************************************************
//
//! Board Initialization & Configuration
//!
//! \param  None
//!
//! \return None
//
//*****************************************************************************
static void
BoardInit(void)
{
/* In case of TI-RTOS vector table is initialize by OS itself */
#ifndef USE_TIRTOS
  //
  // Set vector table base
  //
#if defined(ccs) || defined(gcc)
  MAP_IntVTableBaseSet((unsigned long)&g_pfnVectors[0]);
#endif
#if defined(ewarm)
  MAP_IntVTableBaseSet((unsigned long)&__vector_table);
#endif
#endif
  //
  // Enable Processor
  //
  MAP_IntMasterEnable();
  MAP_IntEnable(FAULT_SYSTICK);

  PRCMCC3200MCUInit();

  PRCMRTCInUseSet();
  PRCMRTCSet(0, 0);
}

#ifdef MODE_IMU
frudp_pub_t *pub_imu = NULL;

void imu_cb()
{
  static struct sensor_msgs__imu imu_msg;
  static char *g_frame_id = "imu_frame";

  imu_msg.header.stamp.sec = 1234;
  imu_msg.header.stamp.nanosec = 5678;
  imu_msg.header.frame_id = g_frame_id;
  imu_msg.orientation.x = 1;
  imu_msg.orientation.y = 2;
  imu_msg.orientation.z = 3;
  imu_msg.orientation.w = 4;
  imu_msg.angular_velocity.x = 5;
  imu_msg.angular_velocity.y = 6;
  imu_msg.angular_velocity.z = 7;
  imu_msg.linear_acceleration.x = readXData();
  imu_msg.linear_acceleration.y = readYData();
  imu_msg.linear_acceleration.z = readZData();
  for (int i = 0; i < 9; i++)
  {
    imu_msg.orientation_covariance[i] = 11 + i;
    imu_msg.angular_velocity_covariance[i] = 20 + i;
    imu_msg.linear_acceleration_covariance[i] = 29 + i;
  }

  static uint8_t __attribute__((aligned(4))) cdr[512] = {0};
  uint32_t cdr_len = serialize_sensor_msgs__imu(&imu_msg, cdr, sizeof(cdr));
  freertps_publish(pub_imu, (uint8_t *)cdr, cdr_len);
}

void imu_task(void *p)
{
  Report("Start Alfred IMU App...\r\n");
  GPIO_IF_LedOn(MCU_ORANGE_LED_GPIO);

  //Start the SimpleLink
  _i16 lMode = sl_Start(0, 0, 0);

  // Connecting to WLAN AP
  // After this call we will be connected and have IP address */
  WlanConnect();

  GPIO_IF_LedOff(MCU_ORANGE_LED_GPIO);

  freertps_system_init();
//  freertps_timer_set_freq(10, imu_cb);

  pub_imu = freertps_create_pub("/imu", sensor_msgs__imu__type.rtps_typename);
  freertps_start(); // all pubs/subs are created. let's start!

  GPIO_IF_LedOff(MCU_ORANGE_LED_GPIO);

  while (freertps_system_ok())
  {
    frudp_listen(1000000);
    frudp_disco_tick();

    imu_cb();
  }
  frudp_fini();

  return;
}
#endif

#ifdef MODE_LISTENER
#ifdef MODE_MSG
void chatter_cb(const void *msg)
{
  GPIO_IF_LedOn(MCU_ORANGE_LED_GPIO);

  static struct std_msgs__string* string_msg;
  bool result = deserialize_std_msgs__string(msg, 64, string_msg);
  Report("I heard: [%s]\n", string_msg->data);

  GPIO_IF_LedOff(MCU_ORANGE_LED_GPIO);
}
#else
void chatter_cb(const void *msg)
{
  GPIO_IF_LedOn(MCU_ORANGE_LED_GPIO);
  uint32_t str_len = *((uint32_t *) msg);
  char buf[128] = { 0 };
  int i;
  for ( i = 0; i < str_len && i < sizeof(buf) - 1; i++)
  {
    buf[i] = ((uint8_t *) msg)[4 + i];
  }
  Report("I heard: [%s]\r\n", buf);
  GPIO_IF_LedOff(MCU_ORANGE_LED_GPIO);
}
#endif

void listener_task(void *p)
{
  Report("Start Alfred listener App...\r\n");
  GPIO_IF_LedOn(MCU_ORANGE_LED_GPIO);

  //Start the SimpleLink
  _i16 lMode = sl_Start(0, 0, 0);

  // Connecting to WLAN AP
  // After this call we will be connected and have IP address */
  WlanConnect();

  GPIO_IF_LedOff(MCU_ORANGE_LED_GPIO);

  freertps_system_init();
#ifndef MODE_MSG
  freertps_create_sub("/chatter", "std_msgs::msg::dds_::String_", chatter_cb);
#else
  freertps_create_sub("/chatter", std_msgs__string__type.rtps_typename, chatter_cb);
#endif
  freertps_start(); // all pubs/subs are created. let's start!

  GPIO_IF_LedOff(MCU_ORANGE_LED_GPIO);

  while (freertps_system_ok())
  {
    frudp_listen(5000000);
    frudp_disco_tick();
  }
  frudp_fini();

  return;
}
#endif

#ifdef MODE_TALKER
frudp_pub_t *pub = NULL;
int pub_count = 0;

#ifndef MODE_MSG
void timer_cb(void)
{
  if (pub)
  {
    char msg[256] = {0};
    snprintf(&msg[4], sizeof(msg) - 4, "Hello World: %d", pub_count++);
    uint32_t rtps_string_len = strlen(&msg[4]) + 1;
    *((uint32_t *)msg) = rtps_string_len;
    freertps_publish(pub, (uint8_t *)msg, rtps_string_len + 4);
  }
}
#else
void timer_cb(void)
{
  if (pub)
  {
    struct std_msgs__string msg;
    char data_buf[256] = {0};
    msg.data = data_buf;

    uint8_t cdr[256] = {0};
    snprintf(msg.data, sizeof(data_buf), "Hello, world! %d", pub_count++);
    int cdr_len = serialize_std_msgs__string(&msg, cdr, sizeof(cdr));
    freertps_publish(pub, cdr, cdr_len);
  }
}
#endif

void talker_task(void *p)
{
  Report("Start Alfred talker App...\r\n");
  GPIO_IF_LedOn(MCU_ORANGE_LED_GPIO);

  //Start the SimpleLink
  _i16 lMode = sl_Start(0, 0, 0);

  // Connecting to WLAN AP
  // After this call we will be connected and have IP address */
  WlanConnect();

  GPIO_IF_LedOff(MCU_ORANGE_LED_GPIO);

  freertps_system_init();
//  freertps_timer_set_freq(10, timer_cb);

#ifndef MODE_MSG
  pub = freertps_create_pub("/chatter", "std_msgs::msg::dds_::String_", get_default_qos_reliable());
#else
  pub = freertps_create_pub("/chatter", std_msgs__string__type.rtps_typename, get_default_qos_reliable());
#endif
  freertps_start(); // all pubs/subs are created. let's start!

  GPIO_IF_LedOff(MCU_ORANGE_LED_GPIO);

  while (freertps_system_ok())
  {
    frudp_listen(1000000);
    frudp_disco_tick();

    timer_cb();
  }
  frudp_fini();

  return;
}
#endif

//*****************************************************************************
//
//!  main function handling the freertos_demo.
//!
//! \param  None
//!
//! \return none
//
//*****************************************************************************
int main( void )
{
  //
  // Initialize the board
  //
  BoardInit();

  PinMuxConfig();
  GPIO_IF_LedConfigure(LED1|LED2|LED3);
  GPIO_IF_LedOff(MCU_ALL_LED_IND);
  //
  // Initializing the terminal
  //
  InitTerm();

  //
  // Clearing the terminal
  //
  ClearTerm();
  GPIO_IF_LedOn(MCU_GREEN_LED_GPIO);

  //
  // Diasplay Banner
  //
  DisplayBanner(APP_NAME);

  Report("Start init...\r\n");
  //
  // Creating a queue for 10 elements.
  //
  xPrintQueue =xQueueCreate( 10, sizeof( unsigned portLONG ) );

  if( xPrintQueue == 0 )
  {
    // Queue was not created and must not be used.
    return 0;
  }
  VStartSimpleLinkSpawnTask(SPAWN_TASK_PRIORITY);

#ifdef MODE_LISTENER
  osi_TaskCreate( listener_task, ( signed portCHAR * ) "TASK_LISTENER", OSI_STACK_SIZE, NULL, 9, NULL);
#endif

#ifdef MODE_TALKER
  osi_TaskCreate( talker_task, ( signed portCHAR * ) "TASK_TALKER", OSI_STACK_SIZE, NULL, 9, NULL);
#endif

#ifdef MODE_IMU
  osi_TaskCreate( imu_task, ( signed portCHAR * ) "TASK_IMU", OSI_STACK_SIZE, NULL, 9, NULL);
#endif

  //
  // Start the task scheduler
  //
  osi_start();

  return 0;
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
