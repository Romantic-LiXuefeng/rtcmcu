﻿#ifndef __RTP_TRANS_MANAGER_
#define __RTP_TRANS_MANAGER_

#include "avformat/rtp.h"
#include <event.h>
#include <stdint.h>
#include <string>
#include <map>
#include <set>

const int MAX_RTP_LEN = (10 * 1024);

class RtpConnection;
class RtpCacheManager;
class RTPTransConfig;
class StreamId_Ext;

namespace avformat {
  class RtcpPacket;
}

class RTPTransManager {
  friend class RTPTrans;

public:
  static RTPTransManager* Instance();
  static void DestroyInstance();

  RTPTransManager();
  ~RTPTransManager();

  void Init(struct event_base *ev_base);

  RtpCacheManager* GetRtpCacheManager();

  int _open_trans(RtpConnection *c, const RTPTransConfig *config);
  void _close_trans(RtpConnection *c);

  bool HasPlayer(const StreamId_Ext& streamid);
  bool HasUploader(const StreamId_Ext& streamid);

  int OnRecvRtp(RtpConnection *c, const void *rtp, uint16_t len);
  int OnRecvRtcp(RtpConnection *c, const void *rtcp, uint32_t len);

  int32_t set_sdp_char(const StreamId_Ext& stream_id, const char* sdp, int32_t len);
  int32_t set_sdp_str(const StreamId_Ext& stream_id, const std::string& sdp);
  std::string get_sdp_str(const StreamId_Ext& stream_id);

protected:
  void _send_fec(RtpConnection *c, const avformat::RTP_FIXED_HEADER *pkt, uint32_t pkt_len);

  // called by rtp_trans
  int SendNackRtp(RtpConnection *c, bool video, uint16_t seq);   // send nack
  void _send_rtcp_cb(RtpConnection *c, const avformat::RtcpPacket *rtcp);

  avformat::RTP_FIXED_HEADER* _get_rtp_by_ssrc_seq(RtpConnection *c,
    bool video, uint16_t seq, uint16_t &len, int32_t& status_code);

  void ForwardRtp(const StreamId_Ext& streamid, avformat::RTP_FIXED_HEADER *rtp, uint32_t len);
  void ForwardRtcp(const StreamId_Ext& streamid, avformat::RTPAVType type, uint32_t ntp_secs, uint32_t ntp_frac, uint32_t rtp);

  static void OnTimer(evutil_socket_t fd, short flag, void *arg);
  void OnTimerImpl(evutil_socket_t fd, short flag, void *arg);

private:
  static const uint32_t MAX_TRANS_NUM = 5000;
  static const uint32_t MAX_RTP_ITEM_NUM = 25000;

  struct event *m_ev_timer;
  RtpCacheManager *m_rtp_cache;
  std::map<uint32_t, std::set<RtpConnection*> > m_stream_groups;

  static RTPTransManager* m_inst;
};

#endif
