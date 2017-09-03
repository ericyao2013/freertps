#include "freertps/rtps/type/qos.h"

frudp_qos_reliability_t *get_default_qos_reliable()
{
	frudp_qos_reliability_t *reliability = malloc(sizeof(frudp_qos_reliability_t));
	reliability->kind = FRUDP_QOS_RELIABILITY_KIND_BEST_EFFORT;
	return reliability;
}
