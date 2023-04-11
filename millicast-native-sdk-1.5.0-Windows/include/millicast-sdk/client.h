#ifndef MILLICAST_API_CLIENT_H
#define MILLICAST_API_CLIENT_H

/**
 * @file client.h
 * @author David Baldassin
 * @copyright Copyright 2021 CoSMoSoftware.
 * @date 02/2021
 */

#include <optional>
#include <string>
#include <functional>
#include <list>
#include <vector>
#include <memory>

#include "millicast-sdk/mc_logging.h"
#include "millicast-sdk/exports.h"

namespace millicast
{
// Forward declaration
class StatsReport;

/**
 * @brief The DegradationPreferences enum
 * @brief Based on the spec in https://w3c.github.io/webrtc-pc/#idl-def-rtcdegradationpreference.
 */
enum class DegradationPreferences
{
  DISABLED, /**< Don't take any actions based on over-utilization signals. */
  MAINTAIN_RESOLUTION, /**< On over-use, request lower resolution, possibly causing down-scaling. */
  MAINTAIN_FRAMERATE, /**< On over-use, request lower frame rate, possibly causing frame drops. */
  BALANCED /**< Try to strike a "pleasing" balance between frame rate or resolution. */
};


/**
 * @brief Allows setting the desired minimum, maximum and or start bitrates
 * when publishing a stream. You can also use `disable_bwe` to disable the internal
 * bandwidth estimation and utilise the maximum bitrate provided as the sending
 * bitrate.
 * 
 * @remark setting the minimum bitrate to a high value may result in inconsistent
 * streams. Consider your network bandwidth as a factor.
 *
 * @remark An exception will be thrown if disabling bandwidth estimation is set
 * without setting the maximum bitrate.
 */
struct BitrateSettings {
  
  bool 		      disable_bwe { false };
  std::optional<int>  start_bitrate_kbps;
  std::optional<int>  min_bitrate_kbps;
  std::optional<int>  max_bitrate_kbps;
};

/**
 * @brief Useful informations about the available tracks in the media server. 
 * These informations are received in the active event of the Viewer
 */
struct TrackInfo
{
  std::string media; /**< The kind of the track, either "video" or "audio" */
  std::string track_id; /**< The id of the track (on the sender side) */
};

/**
 * @brief The Client base class.
 * @brief This is the base class to handle a connection with the Millicast platform.
 */
class MILLICAST_API Client
  {
 
  public:
  /**
   * @brief The Client Listener struct which contains methods that will be called
   * on specific events from a Client object.
   */

  struct Listener
  {
    Listener() = default;

    /**
     * @brief on_connected is called when the WebSocket connection to Millicast is opened
     */

    virtual void on_connected() = 0;

    /**
     * @brief on_connection_error is called when we try to connect to millicast but it fails
     */
    virtual void on_connection_error(int status, const std::string& reason) = 0;

    /**
     * @brief Called when millicast sends back an error message in response to a websocket command
     * @param message The error message received from millicast
     */ 
    virtual void on_signaling_error(const std::string& message) = 0;
    
    /**
     * @brief on_stats_report is called when a new rtc stats report has been collected. You must
     * not perform long blocking operations on this callback.
     * @remarks You must enable the stats to be able to receive a report.
     * @see enable_stats
     */

    virtual void on_stats_report(const StatsReport&) = 0;

    /**
     * @brief Called when a new viewer join the stream or when a viewer quit the stream
     * @param count The current number of viewers.
     */
    virtual void on_viewer_count(int count) = 0;
    
    virtual ~Listener() = default;
  };

  /**
   * @brief The Option struct allows to setup the millicast connection.
   */
  struct Option
  {    
    std::optional<bool>                   stereo; /**< Whether you want to enable stereo or not */
    std::optional<bool>                   dtx; /**< enable discontinuous transmission on the publishing side, so audio data is only sent when a user’s voice is detected. */
    int                                   stats_delay_ms; /**< The rate at which you want to receive stats report (not implemented yet) */
    std::optional<DegradationPreferences> degradation; /**< Which strategy the use in order to limit the bandwidth usage */
    bool                                  hardware_support{false};
  };


  /**
   * @brief Authentication data returned by the director api in order to open a websocket connection
   */
  struct JsonData
  {
    std::string ws_url; /**< The WebSocket URL */
    std::string jwt; /**< The JSON Web Token */
  };

  Client() = default;
  virtual ~Client() = default;

  /**
   * @brief Set the client option to configure the publisher/subscriber
   * @param options The option object containing all the informations.
   */
  virtual void set_options(const Option& options) = 0;

  /**
   * @brief Get the current options.
   * @return The options object.
   */
  virtual const Option& get_options() const = 0;

  /**
   * @brief connect and open a websocket connection with the millicast platform.
   * @return false if an error occured, true otherwise.
   * @remarks You must set valid credentials before using this method.
   * @remarks returning true does not mean you are connected.
   * You are connected when the Listener's method on_connected is called.
   */

  virtual bool connect() = 0;

  /**
   * @brief Connect to the media server directly using the websocket url and the JWT.
   * @param data The JSON data returned by the director api.
   * @return false if an error occured, true otherwise.
   * @remarks returning true does not mean you are connected.
   * You are connected when the Listener's method on_connected is called.
   */
  virtual bool connect(const JsonData& data) = 0;

  /**
   * @brief is_connected
   * @return return true if the client is connected to millicast, false otherwise.
   */

  virtual bool is_connected() const = 0;

  /**
   * @brief disconnect from millicast.
   * @return false if an error occured, true otherwise.
   */

  virtual bool disconnect() = 0;

  /**
   * @brief set_listener : set the client listener to receive event from the client.
   * @param listener The Client listener
   */

  virtual void set_listener(Listener * listener) = 0;

  /**
   * @brief Enable the rtc stats collecting.
   * The stats are collected once the client is either publishing or subscribed.
   * @param enable true to enable the stats, false to disable the stats.
   */

  virtual void enable_stats(bool enable) = 0;

  /**
   * @brief Get the transceiver mid associated to a track if any.
   * @param track_id The id of the track you want to retrieve the mid
   * @return The transceiver mid if it exists
   */
  virtual std::optional<std::string> get_mid(const std::string& track_id) = 0;

  /**
  * @brief Add frame transformer so you can add metadata to frame
  * When a new transformable frame is available, the listener method on_transformable_frame will be called
  * @param enable true to enable the frame transformer false to disable it
  */
  virtual void enable_frame_transformer(bool enable) = 0;

  /**
   * @brief get_supported_video_codecs returns the list of the supported video codecs.
   * @return The list of the supported video codecs.
   */

  static std::list<std::string> get_supported_video_codecs();

  /**
   * @brief get_supported_audio_codecs returns the list of the supported audio codecs.
   * @return The list of the supported audio codecs.
   */

  static std::list<std::string> get_supported_audio_codecs();

  /**
   * @brief Clean and free the memory of dynamic objects.
   * @remarks Call this after all sdk objects have been destroyed.
   */

  static void cleanup();
  };


}  // millicast

#endif /* MILLICAST_API_CLIENT_H */
