// Copyright 2016 Open Source Robotics Foundation, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef RCLCPP__EXCEPTIONS_HPP_
#define RCLCPP__EXCEPTIONS_HPP_

#include <stdexcept>
#include <string>

#include "rcl/error_handling.h"
#include "rcl/types.h"
#include "rclcpp/visibility_control.hpp"

namespace rclcpp
{
namespace exceptions
{

/// Thrown when a method is trying to use a node, but it is invalid.
class InvalidNodeError : public std::runtime_error
{
public:
  InvalidNodeError()
  : std::runtime_error("node is invalid") {}
};

/// Thrown when a any kind of name (node, namespace, topic, etc.) is invalid.
class NameValidationError : public std::invalid_argument
{
public:
  NameValidationError(
    const char * name_type_,
    const char * name_,
    const char * error_msg_,
    size_t invalid_index_)
  : std::invalid_argument(format_error(name_type_, name_, error_msg_, invalid_index_)),
    name_type(name_type_), name(name_), error_msg(error_msg_), invalid_index(invalid_index_)
  {}

  static std::string
  format_error(
    const char * name_type,
    const char * name,
    const char * error_msg,
    size_t invalid_index);

  const std::string name_type;
  const std::string name;
  const std::string error_msg;
  const size_t invalid_index;
};

/// Thrown when a node name is invalid.
class InvalidNodeNameError : public NameValidationError
{
public:
  InvalidNodeNameError(const char * node_name, const char * error_msg, size_t invalid_index)
  : NameValidationError("node name", node_name, error_msg, invalid_index)
  {}
};

/// Thrown when a node namespace is invalid.
class InvalidNamespaceError : public NameValidationError
{
public:
  InvalidNamespaceError(const char * namespace_, const char * error_msg, size_t invalid_index)
  : NameValidationError("namespace", namespace_, error_msg, invalid_index)
  {}
};

/// Thrown when a topic name is invalid.
class InvalidTopicNameError : public NameValidationError
{
public:
  InvalidTopicNameError(const char * namespace_, const char * error_msg, size_t invalid_index)
  : NameValidationError("topic name", namespace_, error_msg, invalid_index)
  {}
};

/// Thrown when a service name is invalid.
class InvalidServiceNameError : public NameValidationError
{
public:
  InvalidServiceNameError(const char * namespace_, const char * error_msg, size_t invalid_index)
  : NameValidationError("service name", namespace_, error_msg, invalid_index)
  {}
};

/// Throw a C++ std::exception which was created based on an rcl error.
/**
 * Passing nullptr for reset_error is safe and will avoid calling any function
 * to reset the error.
 *
 * \param ret the return code for the current error state
 * \param prefix string to prefix to the error if applicable (not all errors have custom messages)
 * \param error_state error state to create exception from, if nullptr rcl_get_error_state is used
 * \param reset_error function to be called before throwing which whill clear the error state
 * \throws std::invalid_argument if ret is RCL_RET_OK
 * \throws std::runtime_error if the rcl_get_error_state returns 0
 * \throws RCLErrorBase some child class exception based on ret
 */
RCLCPP_PUBLIC
void
throw_from_rcl_error(
  rcl_ret_t ret,
  const std::string & prefix = "",
  const rcl_error_state_t * error_state = nullptr,
  void (*reset_error)() = rcl_reset_error);

class RCLErrorBase
{
public:
  RCLCPP_PUBLIC
  RCLErrorBase(rcl_ret_t ret, const rcl_error_state_t * error_state);
  virtual ~RCLErrorBase() {}

  rcl_ret_t ret;
  std::string message;
  std::string file;
  size_t line;
  std::string formatted_message;
};

/// Created when the return code does not match one of the other specialized exceptions.
class RCLError : public RCLErrorBase, public std::runtime_error
{
public:
  RCLCPP_PUBLIC
  RCLError(rcl_ret_t ret, const rcl_error_state_t * error_state, const std::string & prefix);
  RCLCPP_PUBLIC
  RCLError(const RCLErrorBase & base_exc, const std::string & prefix);
};

/// Created when the ret is RCL_RET_BAD_ALLOC.
class RCLBadAlloc : public RCLErrorBase, public std::bad_alloc
{
public:
  RCLCPP_PUBLIC
  RCLBadAlloc(rcl_ret_t ret, const rcl_error_state_t * error_state);
  RCLCPP_PUBLIC
  explicit RCLBadAlloc(const RCLErrorBase & base_exc);
};

/// Created when the ret is RCL_RET_INVALID_ARGUMENT.
class RCLInvalidArgument : public RCLErrorBase, public std::invalid_argument
{
public:
  RCLCPP_PUBLIC
  RCLInvalidArgument(
    rcl_ret_t ret,
    const rcl_error_state_t * error_state,
    const std::string & prefix);
  RCLCPP_PUBLIC
  RCLInvalidArgument(const RCLErrorBase & base_exc, const std::string & prefix);
};

/// Thrown when an invalid rclcpp::Event object or SharedPtr is encountered.
class InvalidEventError : public std::runtime_error
{
public:
  InvalidEventError()
  : std::runtime_error("event is invalid") {}
};

/// Thrown when an unregistered rclcpp::Event is encountered where a registered one was expected.
class EventNotRegisteredError : public std::runtime_error
{
public:
  EventNotRegisteredError()
  : std::runtime_error("event already registered") {}
};

}  // namespace exceptions
}  // namespace rclcpp

#endif  // RCLCPP__EXCEPTIONS_HPP_
