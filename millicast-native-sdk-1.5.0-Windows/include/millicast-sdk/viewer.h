#ifndef MILLICAST_API_VIEWER_H
#define MILLICAST_API_VIEWER_H

/**
  * @file viewer.h
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

// Viewer /////////////////////////////////////////////////////////////////////

/**
 * @brief The Viewer class. Its purpose is to receive media
 * by subscribing to a millicast stream.
 * The stream must already exists and someone must publish media.
 */

class MILLICAST_API Viewer : public Client
{
public:

  struct Option : public Client::Option {
    /* multisource options */
    struct {
      std::optional<std::string> pinned_source_id; /**< The received source you want to pin */
      std::optional<uint8_t>     multiplexed_audio_track; /**< The number of received multiplexed audio tracks */
      std::vector<std::string>   excluded_source_id; /**< The source you don't want to receive */
    } multisource;
  };
  
  /**
   * @brief The Credentials struct represent the credentials need to be able to
   * connect and subscribe to a Millicast stream.
   * @sa https://dash.millicast.com/docs.html
   */

  struct Credentials
  {
    bool is_valid; /**< If the credentials set are valid. This is set after calling set_credentials */
    
    std::string stream_name; /**< The name of the stream you want to subscribe */
    /**< The subscribing token as described in the Millicast API (optional) */
    std::optional<std::string> token;
    std::string account_id; /**< Your millicast account ID */
    std::string api_url; /**< The subscribe API URL as described in the Millicast API */
  };

  /**
   * @brief The layer data is used to select a simulcast/svc layer.
   * by sending a command to the server using the select or project method.
   */
  struct LayerData
  {
    std::string encoding_id; /**< The id of the simulcast / SVC layer */
    int temporal_layer_id; /**< The id of the temporal layer */
    int spatial_layer_id; /**< The id of the spatial layer */
    std::optional<int> max_spatial_layer_id;
    std::optional<int> max_temporal_layer_id;
  };

  /**
   * @brief The projection data is used to project a video/audio track into a specific transceiver.
   * We send a command to the media server using the project method to choose which track to 
   * project
   */
  struct ProjectionData
  {
    std::string track_id; /**< The name of the track om the media server side (sent in the active event) */
    std::string media; /**< The kind of the media track, either "video" or "audio" */
    std::string mid; /**< The mid of the transceiver you want to project into */
    std::optional<LayerData> layer; /**< Optionally set a simulcast/SVC layer for this track */
  };

  /**
   * @brief The Listener struct for the Viewer class.
   * It adds the on_subscribed event on top of the Client listener
   * You must inherit this class and set a listener with set_listener
   * to be able to receive events from the Viewer.
   */

  struct Listener : public Client::Listener
  {
    /**
     * @brief on_subscribed is called when the subcription to the stream is complete.
     */

    virtual void on_subscribed() = 0;

    /**
     * @brief Called when an error occured while establishing the peerconnection
     * @param reason The reason of the error
     */
    virtual void on_subscribed_error(const std::string& error) = 0;
    
    /**
     * @brief on_track is called when a remote video track has been added.
     * @param track The remote video track.
     * @param mid The associated transceiver midif any
     */

    virtual void on_track(std::weak_ptr<VideoTrack> track,
			  const std::optional<std::string>& mid) = 0;

    /**
     * @brief on_track is called when a remote audio track has been added.
     * @param track The remote audio track.
     * @param mid The associated transceiver mid if any
     */

    virtual void on_track(std::weak_ptr<AudioTrack> track,
			  const std::optional<std::string>& mid) = 0;

    /* Broadcast events */

    /**
     * @brief Called when a new source has been publishing within the new stream
     * @param streamId The stream id.
     * @param tracks All the track ids within the stream
     * @param sourceId The source id if the publisher has set one.
     */
    virtual void on_active(const std::string& stream_id,
                           const std::vector<TrackInfo>& tracks,
                           const std::optional<std::string>& source_id) = 0;

    /**
     * @brief Called when a source has been unpublished within the stream
     * @param streamId The stream id.
     * @param sourceId The source id set by the publisher if any.
     */
    virtual void on_inactive(const std::string& stream_id,
			     const std::optional<std::string>& source_id) = 0;

    /**
     * @brief onStopped callback is not currently used, but is reserved for future usage.
     */
    virtual void on_stopped() = 0;

    /**
     * @brief Called when a source id is being multiplexed into the audio track based on the voice activity level.
     * @param mid The media id.
     * @param sourceId The source id.
     */
    virtual void on_vad(const std::string& mid, const std::optional<std::string>& source_id) = 0;

    /**
     * @brief Called when simulcast/svc layers are available
     * @param mid The mid associated to the track
     * @param activeLayers Active simulcast/SVC layers
     * @param inactiveLayers inactive simulcast/SVC layers
     */
    virtual void on_layers(const std::string& mid,
			   const std::vector<LayerData>& active_layers,
			   const std::vector<LayerData>& inactive_layers) = 0;

    /**
    * @brief Called when a frame is received and not yet decoded
    * Extract metadata embedded in the frame if any
    * @param ssrc Synchronization source of the frame
    * @param timestamp Timestamp of the frame
    * @param data vector filled with user data coming from the publisher.
    */
    virtual void on_frame_metadata(uint32_t ssrc, uint32_t timestamp, const std::vector<uint8_t>& data) = 0;
  };
  
  Viewer() = default;

  virtual ~Viewer() = default;

  /**
   * @brief subscribe to a millicast stream.
   * You must be connected first in order to subscribe to a stream.
   * @return false if an error occured, true otherwise.
   * @remark returning true does not mean you are subscribed.
   * You are subscribed when the Listener's method on_subscribed is called.
   */

  virtual bool subscribe() = 0;

  /**
   * @brief unsubscribe from the stream and stop receiving media.
   * @return false if an error occured, true otherwise.
   */

  virtual bool unsubscribe() = 0;

  /**
   * @brief Tell whether the viewer is subscribed or not.
   * @return true if the viewer if subscribed, false otherwise.
   */

  virtual bool is_subscribed() const = 0;

  /**
   * @brief Send a command to the media server to forward a media into a specific transceiver.
   * @param source_id The id of the source publishing media (see multisource).
   * @param pdata The projection data to send to the server.
   * @return true if success false otherwise
   */
  
  virtual bool project(const std::string& source_id,
		       const std::vector<ProjectionData>& pdata) = 0;

  /**
   * @brief Send a command to stop the projection of a source
   * @param mids The transceivers mids to stop projecting.
   * @return true if success false otherwise
   */
  
  virtual bool unproject(const std::vector<std::string>& mids) = 0;

  /**
   * @brief Send a command to select a simulcast layer (if simulcast is enabled).
   * @param layer The layer to select. Leave the optional empty if you want to let the server choose automatically the layer.
   * @return true if success false otherwise.
   */
  
  virtual bool select(std::optional<LayerData> layer) = 0;

  /**
   * @brief Dynamically add another track to the peerconnection adn renegociate SDP locally 
   * When the track is created, the listener's method on track will be called.
   * @param kind Whether it is a video or audio track. Accepted value are "video" or "audio".
   * @return true if success. False otherwise.
   */
  
  virtual bool add_remote_track(std::string_view kind) = 0;

  /**
   * @brief Set the viewer credentials.
   * @param creds The credentials
   * @return true if the credentials are valid and set correctly, false otherwise.
   */

  virtual bool set_credentials(const Credentials& creds) = 0;
  virtual bool set_credentials(Credentials&& creds) = 0;

  /**
   * @brief Get the current viewer credentials.
   * @return The current credentials set in the viewer.
   */

  virtual const Credentials& get_credentials() const = 0;

  /**
   * @brief Create a new viewer.
   * @return A new Viewer object.
   */

  static std::unique_ptr<Viewer> create();
};


}  // millicast


#endif /* MILLICAST_API_VIEWER_H */
