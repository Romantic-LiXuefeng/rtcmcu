/*
 * interfaces of forward client and forward server
 * author: hechao@youku.com
 * create: 2013
 *
 */

#ifndef MODULE_BACKEND_
#define MODULE_BACKEND_

#include "config_manager.h"
#include <stdint.h>
#include <event.h>
#include <event2/http.h>

class StreamId_Ext;

class backend_config : public ConfigModule {
private:
  bool inited;

public:
  char backend_listen_ip[32];
  uint16_t backend_listen_port;
  uint16_t backend_asn;
  uint16_t backend_region;
  uint16_t backend_level;
  uint16_t stream_timeout;
  uint32_t forward_client_capability;
  uint32_t forward_client_receive_buf_max_len;
  uint32_t forward_server_max_session_num;
  uint32_t fs_big_session_num;
  uint32_t fs_big_session_size;
  uint32_t fs_connection_buffer_size;

public:
  backend_config();
  virtual ~backend_config();
  backend_config& operator=(const backend_config& rhv);
  virtual void set_default_config();
  virtual bool load_config(xmlnode* xml_config);
  virtual bool reload() const;
  virtual const char* module_name() const;
  virtual void dump_config() const;

private:
  bool load_config_unreloadable(xmlnode* xml_config);
  bool load_config_reloadable(xmlnode* xml_config);
  bool resove_config();
};

int backend_init(struct event_base *mainbase, const backend_config *backend_conf);

void backend_fini();

int32_t backend_start_stream_rtp(const StreamId_Ext& stream_id);
int32_t backend_stop_stream_rtp(const StreamId_Ext& stream_id);

void backend_del_stream_from_tracker_v3(const StreamId_Ext& stream_id, int level);

class RelayManager {
public:
  static RelayManager* Instance();
  static void DestroyInstance();

  int Init(struct event_base *ev_base);

  int StartPullRtp(const StreamId_Ext& stream_id);
  int StopPullRtp(const StreamId_Ext& stream_id);
  int StartPushRtp(const StreamId_Ext& stream_id);
  int StopPushRtp(const StreamId_Ext& stream_id);

protected:
  RelayManager();
  ~RelayManager();

  struct event_base* m_ev_base;
  static RelayManager* m_inst;
};

class HttpServerManager {
public:
  static HttpServerManager* Instance();
  static void DestroyInstance();

  int Init(struct event_base *ev_base, unsigned short port);
  int AddHandler(const char *path,
    void(*cb)(struct evhttp_request *, void *), void *cb_arg);

protected:
  HttpServerManager();
  ~HttpServerManager();
  static void DefaultHandler(struct evhttp_request *requset, void *arg);

  struct event_base* m_ev_base;
  struct evhttp *m_ev_http;
  static HttpServerManager* m_inst;
};

#endif
