#include "freertps/psm/system.h"
#include "freertps/psm/udp.h"
#include "freertps/freertps.h"
#include "freertps/rtps/type/config.h"
#include "metal/metal.h"

void freertps_system_init(void)
{
  frudp_init();
  freertps_metal_enable_irq();
}

bool freertps_system_ok(void)
{
  return true;
}

bool frudp_init_participant_id(void)
{
  FREERTPS_INFO("frudp_init_participant_id()\r\n");
  g_frudp_config.participant_id = 0; // Only one participant in board.
  return true;
}
