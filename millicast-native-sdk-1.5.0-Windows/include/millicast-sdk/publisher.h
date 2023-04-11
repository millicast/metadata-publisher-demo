#ifndef MILLICAST_API_PUBLISHER_H
#define MILLICAST_API_PUBLISHER_H

/**
  * @file publisher.h
  * @author David Baldassin
  * @copyright Copyright 2021 CoSMoSoftware.
  * @date 02/2021
  */

#include "millicast-sdk/client.h"

namespace millicast
{

// Forward declarations ///////////////////////////////////////////////////////

class VideoTrack;
class AudioTrack;
class Track;

// Scalability mode ///////////////////////////////////////////////////////////

enum class ScalabilityMode
{
  L1T2,
  L1T2h,
  L1T3,
  L1T3h,
  L2T1,
  L2T1h,
  L2T1_KEY,
  L2T2,
  L2T3,
  L2T2h,
  L2T2_KEY,
  L2T2_KEY_SHIFT,
  L2T3h,
  L3T1,
  L3T2,
  L3T3,
  L3T3_KEY,
  S2T1,
  S2T2,
  S2T3,
  S3T1,
  S3T2,
  S3T3,
  S2T1h,
  S2T2h,
  S2T3h,
  S3T1h,
  S3T2h,
  S3T3h
};

// Publisher //////////////////////////////////////////////////////////////////

/**
 * @brief The Publisher class. Its purpose is to publish media to a Millicast stream.
 */

class MILLICAST_API Publisher : public Client
{
public:
  /**
   * @brief The Credentials struct represent the credentials need to be able to
   * connect and publish to a Millicast stream.
   * @sa https://dash.millicast.com/docs.html
   */

  struct Credentials
  {
    bool is_valid; /**< If the credentials set are valid. This is set after calling set_credentials */
    
    std::string stream_name; /**< The name of the stream we want to publish */
    std::string token;       /**< The publishing token as described in the Millicast API */
    std::string api_url;     /**< The publish API URL as described in the Millicast API */
  };

  /**
   * @brief The Listener struct for the Publisher class.
   * It adds the publishing event on top of the Client listener
   * You must inherit this class and set a listener with set_listener
   * to be able to receive events from the publisher.
   */

  struct Listener : public Client::Listener
  {
    /**
     * @brief on_publishing is called when a peerconnection has been established
     * with Millicast and the media exchange has started.
     */
    virtual void on_publishing() = 0;

    /** 
     * @brief Called when the was an error while establishing the peerconnection
     * @param reason The reason of the error
     */
    virtual void on_publishing_error(const std::string& reason) = 0;

    /* Broadcast events */

    /**
     * @brief This event is called when the first viewer starts viewing the stream
     */
    virtual void on_active() = 0;

    /**
     * @brief This event is called when the last viewer stops viewing the stream
     */
    virtual void on_inactive() = 0;

    /**
    * @brief Called after a frame has been encoded if you need to add data
    * to this frame before the frame is being passed to the RTP packetizer
    * @param ssrc Synchronization source of the frame
    * @param timestamp Timestamp of the frame
    * @param data Empty vector to be filled with user data that will be added to the end of the frame.
    */
    virtual void on_transformable_frame(uint32_t ssrc, uint32_t timestamp, std::vector<uint8_t>& data) = 0;
  };

  /**
   * @brief Option specific to the publisher
   */
  struct Option : public Client::Option {
    /* multisource options */
    struct {
      std::optional<std::string> source_id; /**< The source id/name you want to give to your publisher. */
    } multisource;
    
    struct {
      std::optional<std::string> video; /**< The video codec to use (VP8 by default)  */
      std::optional<std::string> audio; /**< The audio codec to use (opus by default) */
    } codecs;

    bool simulcast {false}; /**< Whether you want to enable simulcast (for H264 and VP8 only) */
    std::optional<ScalabilityMode> svc_mode; /** Setting a scalability mode will enable SVC. (VP9 and AV1) */
    BitrateSettings                       bitrate_settings; /**<Control the minimum, maximum and/or start bitrate */

  };
  
  Publisher() = default;
  virtual ~Publisher() = default;

  /**
   * @brief publish a stream to millicast
   * You must be connected first in order to publish a stream.
   * @return false if an error occured, true otherwise.
   * @remark returning true does not mean you are publishing.
   * You are publishing when the Listener's method on_publishing is called.
   */

  virtual bool publish() = 0;

  /**
   * @brief unpublish : stop sending media to millicast.
   * The websocket connection will still be active.
   * Call disconnect after if you want to close the websocket connection.
   * @return false if an error occured, true otherwise.
   */

  virtual bool unpublish() = 0;

  /**
   * @brief Tell if the publisher is publishing
   * @return true if the publisher is publishing, false otherwise.
   */

  virtual bool is_publishing() const = 0;

  /**
   * @brief Set the publisher credentials.
   * @param creds The credentials
   * @return true if the credentials are valid and set correctly, false otherwise.
   */

  virtual bool set_credentials(const Credentials& creds) = 0;
  virtual bool set_credentials(Credentials&& creds) = 0;

  /**
   * @brief Get the current publisher credentials.
   * @return The current credentials set in the publisher.
   */

  virtual const Credentials& get_credentials() const = 0;

  /**
   * @brief Add a track that will be used to publish media (audio or video).
   * @param track The track as a weak ptr.
   */

  virtual void add_track(std::weak_ptr<Track> track) = 0;
  virtual void add_track(std::weak_ptr<VideoTrack> track) = 0;
  virtual void add_track(std::weak_ptr<AudioTrack> track) = 0;

  /**
   * @brief clear_tracks will clear all track added to the publisher.
   */

  virtual void clear_tracks() = 0;

  /**
   * @brief Create a publisher object.
   * @return A publisher object.
   */

  static std::unique_ptr<Publisher> create();
};


}  // millicast

#endif /* MILLICAST_API_PUBLISHER_H */
