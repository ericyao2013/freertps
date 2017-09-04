#include "freertps/rtps/type/sub_message.h"

#include "freertps/rtps/constant/sub_message_id.h"

const char *frudp_submsg(const uint16_t id)
{
  switch (id)
  {
    case FRUDP_SUBMSG_ID_PAD:               return "Pad";
    case FRUDP_SUBMSG_ID_ACKNACK:           return "AckNack";
    case FRUDP_SUBMSG_ID_HEARTBEAT:         return "HeartBeat";
    case FRUDP_SUBMSG_ID_GAP:               return "Gap";
    case FRUDP_SUBMSG_ID_INFO_TS:           return "InfoTimestamp";
    case FRUDP_SUBMSG_ID_INFO_SRC:          return "InfoSource";
    case FRUDP_SUBMSG_ID_INFO_REPLY_IP4:    return "InfoReplyIp4";
    case FRUDP_SUBMSG_ID_INFO_DEST:         return "InfoDestination";
    case FRUDP_SUBMSG_ID_INFO_REPLY:        return "InfoReply";
    case FRUDP_SUBMSG_ID_NACK_FRAG:         return "NackFrag";
    case FRUPG_SUBMSG_ID_HEARTBEAT_FRAG:    return "HeartbeatFrag";
    case FRUDP_SUBMSG_ID_DATA:              return "Data";
    case FRUDP_SUBMSG_ID_DATA_FRAG:         return "DataFrag";
    default:                                return "unknown";
  }
}

