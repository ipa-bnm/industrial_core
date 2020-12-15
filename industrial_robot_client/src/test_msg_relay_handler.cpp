/*
 * Software License Agreement (BSD License)
 *
 * Copyright (c) 2012, Southwest Research Institute
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 	* Redistributions of source code must retain the above copyright
 * 	notice, this list of conditions and the following disclaimer.
 * 	* Redistributions in binary form must reproduce the above copyright
 * 	notice, this list of conditions and the following disclaimer in the
 * 	documentation and/or other materials provided with the distribution.
 * 	* Neither the name of the Southwest Research Institute, nor the names
 *	of its contributors may be used to endorse or promote products derived
 *	from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "industrial_robot_client/test_msg_relay_handler.h"
#include "simple_message/log_wrapper.h"
#include <std_msgs/Float32.h>

using namespace industrial::shared_types;
using namespace industrial::smpl_msg_connection;
using namespace industrial::simple_message;
using namespace industrial::test_message;

namespace industrial_robot_client
{
namespace test_msg_relay_handler
{

bool TestMsgRelayHandler::init(SmplMsgConnection* connection)
{
  this->pub_test_msg_ = this->node_.advertise<std_msgs::Float32>("test", 1);
  return init((int)StandardMsgTypes::TEST_MSG, connection);
}

bool TestMsgRelayHandler::internalCB(SimpleMessage& in)
{
  TestMessage test_msg;

  if (!test_msg.init(in))
  {
    LOG_ERROR("Failed to initialize status message");
    return false;
  }

  return internalCB(test_msg);
}

bool TestMsgRelayHandler::internalCB(TestMessage & in)
{
  std_msgs::Float32 msg;

  msg.data = in.test_;
  bool rtn = true;
  
  this->pub_test_msg_.publish(msg);

  // Reply back to the controller if the sender requested it.
  if (CommTypes::SERVICE_REQUEST == in.getCommType())
  {
    SimpleMessage reply;
    in.toReply(reply, rtn ? ReplyTypes::SUCCESS : ReplyTypes::FAILURE);
    this->getConnection()->sendMsg(reply);
  }

  return rtn;
}

}
}

