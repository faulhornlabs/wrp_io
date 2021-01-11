/*
 * async_can.hpp
 *
 * Created on: Sep 10, 2020 13:22
 * Description:
 *
 * Note: CAN TX is not buffered and only the latest frame will be transmitted.
 *  Buffered transmission will need to be added if a message has to be divided 
 *  into multiple frames and in applications where no frame should be dropped.
 *
 * Copyright (c) 2020 Ruixiang Du (rdu)
 */

#ifndef ASYNC_CAN_HPP
#define ASYNC_CAN_HPP

#include <linux/can.h>

#include <memory>

#ifndef _MSC_VER
#include "asio/posix/basic_stream_descriptor.hpp"
#else
#include "asio/posix/win_basic_stream_descriptor.hpp"
#endif
#include "wrp_io/async_listener.hpp"
#include <functional>
#include <string>

namespace westonrobot {
class AsyncCAN : public AsyncListener,
                 public std::enable_shared_from_this<AsyncCAN> {
 public:
  using ReceiveCallback = std::function<void(can_frame *rx_frame)>;

 public:
  AsyncCAN(std::string can_port = "can0");

  void StopService() override;

  void SetReceiveCallback(ReceiveCallback cb) { rcv_cb_ = cb; }
  void SendFrame(const can_frame &frame);

 private:
  int can_fd_;
  ReceiveCallback rcv_cb_ = nullptr;
  asio::posix::basic_stream_descriptor<> socketcan_stream_;

  struct can_frame rcv_frame_;
  void ReadFromPort(struct can_frame& rec_frame,
      asio::posix::basic_stream_descriptor<>& stream);
  bool SetupPort();
  void DefaultReceiveCallback(can_frame *rx_frame);
};
}  // namespace westonrobot

#endif /* ASYNC_CAN_HPP */
