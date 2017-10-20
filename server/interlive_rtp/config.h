#ifndef __CONFIG_TMP_H
#define __CONFIG_TMP_H

#include "connection_manager/module_player.h"
#include "connection_manager/rtp_player_config.h"
#include "connection_manager/uploader_config.h"
#include "connection_manager/rtp_uploader_config.h"
#include "relay/module_backend.h"
#include "relay/rtp_backend_config.h"
#include "relay/forward_client_rtp_tcp.h"
#include "media_manager/cache_manager_config.h"
#include "network/base_http_server.h"
#include "target_config.h"
#include "relay/flv_publisher.h"

struct config
{
  TargetConfig    target_conf;
  player_config   player;
  RTPPlayerConfig rtp_player_config;
  uploader_config uploader;
  RTPUploaderConfig rtp_uploader_config;
  backend_config      backend;
  RTPBackendConfig    rtp_backend_config;
  media_manager::CacheManagerConfig cache_manager_config;
  FCRTPConfig         fcrtp_config;
  http::HTTPServerConfig http_ser_config;
  ModPublisherConfig  publisher_config;

  config& operator=(const config& rhv)
  {
    target_conf = rhv.target_conf;
    player = rhv.player;
    rtp_player_config = rhv.rtp_player_config;
    uploader = rhv.uploader;
    rtp_uploader_config = rhv.rtp_uploader_config;
    backend = rhv.backend;
    rtp_backend_config = rhv.rtp_backend_config;
    cache_manager_config = rhv.cache_manager_config;
    fcrtp_config = rhv.fcrtp_config;
    http_ser_config = rhv.http_ser_config;
    publisher_config = rhv.publisher_config;
    return *this;
  }
};

#endif
