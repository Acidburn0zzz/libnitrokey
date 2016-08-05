#ifndef DEVICE_H
#define DEVICE_H
#include <chrono>
#include <hidapi/hidapi.h>
#include "inttypes.h"

#define HID_REPORT_SIZE 65

// TODO !! SEMAPHORE

namespace nitrokey {
namespace device {
    using namespace std::chrono_literals;

enum class CommError {
  ERR_NO_ERROR = 0,
  ERR_NOT_CONNECTED = -1,
  ERR_WRONG_RESPONSE_CRC = -2,
  ERR_SENDING = -3,
  ERR_STATUS_NOT_OK = -4
};

enum class DeviceModel{
    PRO,
    STORAGE
};

class Device {

public:
  Device();

  // lack of device is not actually an error,
  // so it doesn't throw
  virtual bool connect();
  virtual bool disconnect();

  /*
   *	Sends packet of HID_REPORT_SIZE.
   */
  virtual int send(const void *packet);

  /*
   *	Gets packet of HID_REPORT_SIZE.
   *	Can sleep. See below.
   */
  virtual int recv(void *packet);

  int get_retry_count() const { return m_retry_count; };
  std::chrono::milliseconds get_retry_timeout() const { return m_retry_timeout; };
    std::chrono::milliseconds get_send_receive_delay() const {return m_send_receive_delay;}

    int get_last_command_status() const {return last_command_status;};
    void set_last_command_status(uint8_t _err) { last_command_status = _err;} ;
    bool last_command_sucessfull() const {return last_command_status == 0;};
    DeviceModel get_device_model() const {return m_model;}
private:
    uint8_t last_command_status;

 protected:
  uint16_t m_vid;
  uint16_t m_pid;
    DeviceModel m_model;

  /*
   *	While the project uses Signal11 portable HIDAPI
   *	library, there's no way of doing it asynchronously,
   *	hence polling.
   */
  int m_retry_count;
  std::chrono::milliseconds m_retry_timeout;
  std::chrono::milliseconds m_send_receive_delay;

  hid_device *mp_devhandle;
};

class Stick10 : public Device {
 public:
  Stick10();
};

class Stick20 : public Device {
 public:
  Stick20();
};
}
}
#endif
