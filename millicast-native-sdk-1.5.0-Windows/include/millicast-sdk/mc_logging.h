#ifndef MC_LOGGING_H
#define MC_LOGGING_H

/**
  * @file mc_logging.h
  * @author David Baldassin
  * @copyright Copyright 2021 CoSMoSoftware.
  * @date 02/2021
  */

#include <functional>
#include <string>

#include "millicast-sdk/exports.h"

namespace millicast {

/**
* @brief The LogLevel enum specify the severity of a log message.
*/

enum class LogLevel {
  MC_FATAL, /**< When a fatal error occured and the programm will exit */
  MC_ERROR, /**< When an error occured */
  MC_WARNING, /**< Warn the user about something, but does not prevent a normal utilisation */
  MC_LOG, /**< General info about what happen and which steps are performed */
  MC_DEBUG /**< Debug message */
};

class Logger {
  static std::function<void(const std::string&, LogLevel)> _func;
public:

  /**
   * @brief Set the object that will receive the logs from the SDK
   * @param f The logger object (lambda, functor, function pointer ...)
   */
  MILLICAST_API static void set_logger(std::function<void(const std::string&, LogLevel)> f);

  /**
   * @brief Send a log to the callback logger
   * @param msg The log message
   * @param lvl The severity level
   */
  MILLICAST_API static void log(const std::string& msg, LogLevel lvl);

  /**
   * @brief Log the version of the millicast SDK
   */
  MILLICAST_API static void log_version();

  /** 
   * @brief Get the version of the SDK as a std string
   * @return The version of the SDK
   */
  MILLICAST_API static std::string get_version();

  /**
   * @brief Disable libwebrtc log message, except the error
   */
  MILLICAST_API static void disable_rtc_logs();
};

}  // millicast

#endif // MC_LOGGING_H
