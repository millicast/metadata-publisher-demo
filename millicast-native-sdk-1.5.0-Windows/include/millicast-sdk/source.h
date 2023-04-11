#ifndef MILLICAST_API_SOURCE_H
#define MILLICAST_API_SOURCE_H

/**
  * @file source.h
  * @author David Baldassin
  * @copyright Copyright 2021 CoSMoSoftware.
  * @date 02/2021
  */

#include <string>
#include <vector>
#include <optional>
#include <memory>

#include "millicast-sdk/mc_logging.h"
#include "millicast-sdk/capabilities.h"
#include "millicast-sdk/frames.h"

namespace millicast {

// Forward declarations ///////////////////////////////////////////////////////
class Track;


// Source /////////////////////////////////////////////////////////////////////

/**
 * @brief The Source base class.
 */

class Source
{
  friend class AudioPlayback;
public:

  /**
   * @brief The Source type
   */

  enum class Type {
    DEVICE, /**< Hardware sources, camera, playback devices, ... */
    MONITOR, /**< Fullscreen capture source */
    APP, /**< Application screen capture source */
    MIC, /**< Microphone devices source */
    NDI, /**< Ndi sources (input and output) */
    DECKLINK, /**< DeckLink devices sources (input and output) */
    TVOS, /**< tvOS devices sources (output only) */
    CUSTOM /**< Provide your own audio/video data */
  };

  template<typename T>
  using SourcePtr = std::shared_ptr<T>;

  using Ptr = SourcePtr<Source>;

  /**
   * @brief The source builder is used to build a source object.
   * It uses a CRTP design to build sources according to their real type
   * (derived from Source)
   * @tparam T The source type builder used to build
   * a source object derived from the Source class
   */

  template<typename T>
  struct SourceBuilder
  {
    Type        type;      /**< Type of the source */
    int         id;        /**< The id of the source */
    std::string name;      /**< The display name of the source */
    std::string unique_id; /**< The unique id of the source */

    /**
      * @brief Build the source from the type, id, name and unique_id.
      * @return The source object.
      */

    decltype(auto) build() noexcept {
      decltype(auto) source = T::create();

      source->_type      = std::move(type);
      source->_id        = std::move(id);
      source->_name      = std::move(name);
      source->_unique_id = std::move(unique_id);

      auto& underlying = static_cast<T&>(*this);
      underlying.build_extra(*source);

      return source;
    }
  };

  /**
   * @brief Get the video source's type.
   * @tparam T Get the source's type in the specified type.
   * Supported value are std::string or Source::Type. The default is Source::Type
   */
  template<typename T=Type>
  T   type() const;

  template<>
  MILLICAST_API Source::Type type<Source::Type>() const;

  template<>
  MILLICAST_API std::string type<std::string>() const;

  /**
   * @brief Get the id of the source
   * @return The id of the source
   */

  MILLICAST_API int id()   const;

  /**
   * @brief Get the name of the source
   * @return The name of the source
   */

  MILLICAST_API const std::string& name()      const;

  /**
   * @brief Get the source's unique_id
   * @return The unique id of the source
   */

  MILLICAST_API const std::string& unique_id() const;

  /**
   * @brief Set the name of the track. The name of the track is the name used to identify the track in the SDP.
   * @param track_name Name of the track
   */
  MILLICAST_API void set_track_name(std::string track_name);

  /**
   * @brief Get the name of the track. If you did not set one, it will be the default one.
   * @return The name of the track.
   */
  MILLICAST_API const std::string& track_name() const;

  /**
   * @brief Start a capture from this source,
   * this will init and start the capture device and create
   * the corresponding track.
   * @return The track corresponding to this source.
   * The source's must keep the ownership of the track pointer,
   * that is why the track is returned as a weak_ptr
   */

  MILLICAST_API virtual std::weak_ptr<Track> start_capture() = 0;

  /**
   * @brief Stop a capture adn release the track and the underlying devices.
   * @remark Since a weak_ptr is returned when starting a capture,
   * you will be able to test if the track has been released (if you store it somewhere)
   * you won't have to deal with dangling pointer.
   */

  MILLICAST_API virtual void   stop_capture() = 0;

  /**
   * @brief Tell is the source is currently capturing.
   * @return true is the source is capturing, false otherwise.
   */

  MILLICAST_API virtual bool   is_capturing() const = 0;

  MILLICAST_API virtual ~Source() = default;

protected:
  Source() noexcept;

private:
  Type        _type;
  int         _id;
  std::string _name;
  std::string _unique_id;
  std::string _track_name;
};

// Video //////////////////////////////////////////////////////////////////////

/**
 * @brief The VideoSource class
 */

class VideoSource : public Source
{
protected:
  std::vector<VideoCapabilities> _capabilities;
  VideoCapabilities              _capability;

  explicit VideoSource() = default;
public:
  using Ptr = Source::SourcePtr<VideoSource>;

  /**
   * @brief The Builder struct to build a video source
   */

  struct Builder : public SourceBuilder<Builder>
  {
    std::vector<VideoCapabilities> capabilities; /**< Capabilities of the video source */

    /**
     * @brief Create a new video source
     * @return A video source object as VideoSource::Ptr
     */

    MILLICAST_API static Ptr create();

    /**
     * @brief Extra build steps for source's derived class.
     * This is called by the build method (Source::SourceBuilder::build()).
     * @param src The video source to configure.
     */

    void build_extra(VideoSource& src) {
      src._capabilities = std::move(capabilities);
      if(!src._capabilities.empty()) src._capability = src._capabilities.front();
    }
  };

  /**
   * @brief Get the source's capabilities.
   * @return The source's capabilities.
   */

  MILLICAST_API const std::vector<VideoCapabilities>& capabilities() const;
  MILLICAST_API void set_capabilities(std::vector<VideoCapabilities> &capabilities);

  /**
   * @brief Get the current capability of the source.
   * @return The current capability.
   */

  MILLICAST_API const VideoCapabilities& capability() const;

  /**
   * @brief Set the capability to use for the capture.
   * @param capability The capability.
   * @remark Must be called before start_capture.
   */

  MILLICAST_API void set_capability(const VideoCapabilities& capability);
  MILLICAST_API void set_capability(VideoCapabilities&& capability);
  MILLICAST_API virtual void change_video_source(bool ascending, const std::string& unique_device_id = "") {};

  ~VideoSource() = default;

};

// Audio //////////////////////////////////////////////////////////////////////

/**
 * @brief The AudioControl class
 */

class MILLICAST_API AudioControl
{
public:

  /**
   * @brief Set the microphone / speaker volume.
   * @param v The volume as an integer.
   */

  virtual void set_volume(uint32_t v) = 0;

  /**
   * @brief Set the number of channels to use
   * @param n The number of channels.
   * @warning Only implemented for Source::Type::Device for now.
   * Possible values are 1 or 2.
   */

  virtual void set_num_channel(uint8_t n) = 0;

  /**
   * @brief Mute the microphone or the speakers.
   * @param m true if you want to mute, false if you want to unmute
   */

  virtual void mute(bool m) = 0;

  /**
   * @brief Get the current volume.
   * @return The current volume.
   */

  virtual uint32_t get_volume() = 0;

  /**
   * @brief Tell whether the mic / speaker is muted or not.
   * @return true if muted, false otherwise.
   */

  virtual bool is_muted() const = 0;
};

/**
 * @brief The AudioSource class
 */

class AudioSource : public Source, public AudioControl
{
protected:
  explicit AudioSource() = default;
public:
  using Ptr = Source::SourcePtr<AudioSource>;

  /**
   * @brief The Builder struct to build an audio source.
   */

  struct Builder : public SourceBuilder<Builder>
  {
    /**
     * @brief Create a new audio source object.
     * @return The audio source as AudioSource::Ptr
     */

    MILLICAST_API static Ptr create();

    /**
     * @brief Extra configuration specific to the AudioSource class.
     * this method is called by the Source::SourceBuilder::build method.
     * @brief src The audio source to configure.
     */

    void build_extra(AudioSource&) {}
  };

  ~AudioSource() = default;
};

/**
 * @brief The AudioPlayback class
 * @remark This class inherits the Source class, however this is not
 * a capture source. That is why most of the capture interface is overrided
 * in private here.
 */

class AudioPlayback : public Source, public AudioControl
{
  /* Not a capture source */
  std::weak_ptr<Track> start_capture()      override = 0;
  void                 stop_capture()       override = 0;
  bool                 is_capturing() const override = 0;

public:
  using Ptr = Source::SourcePtr<AudioPlayback>;
  explicit AudioPlayback() = default;

  /**
   * @brief The Builder struct to build an AudioPlayback
   */

  struct Builder : public SourceBuilder<Builder>
  {
    /**
     * @brief Create a new audio playback object.
     * @return The audio playback as AudioPlayback::Ptr
     */

    MILLICAST_API static Ptr create();

    /**
     * @brief Extra configuration specific to the AudioPlayback class.
     * this method is called by the Source::SourceBuilder::build method.
     * @param playback The audio playback to configure.
     */

    void build_extra(AudioPlayback&) {}
  };

  /**
   * @brief Init the playback device.
   */

  MILLICAST_API virtual void init_playback() = 0;

  /**
   * @brief Tell if the playback device is playing
   * @return true if it is playing, false otherwise
   */

  MILLICAST_API virtual bool is_playing() const = 0;
};

// Custom source ////////////////////////////////////////////////////////////////////

/**
 * @brief CustomSources let you provide your own video and audio data to the webrtc stream.
 */
class CustomSource : public Source
{
  std::weak_ptr<Track> start_capture() override = 0;

public:
  using Ptr = Source::SourcePtr<CustomSource>;
  
  CustomSource() = default;
  virtual ~CustomSource() = default;

  /**
   * @brief Builder struct to create a CustomSource
   */
  struct Builder : public SourceBuilder<Builder>
  {
    Builder();

    /**
     * @brief Create the CustomSource object
     * @return A CustomSource pointer
     */
    MILLICAST_API static Ptr create();
    void build_extra(CustomSource&) {}
  };

  /**
   * @brief Create the audio track.
   * @return The audio track
   */
  virtual std::weak_ptr<Track> start_audio_capture() = 0;
  /**
   * @brief Create he video track
   * @return The video track
   */
  virtual std::weak_ptr<Track> start_video_capture() = 0;
  
  /**
   * @brief Call this at the video your own frame rate to provide video frame to the stream.
   * @param The video frame. You must provide support for I420 buffer (or I444 if using vp9 profile 1)
   */
  virtual void on_video_frame(const millicast::VideoFrame& video_frame) = 0;
  virtual void on_audio_frame(const millicast::AudioFrame& audio_frame) = 0;

  /**
   * @brief Get the current capability of the video source
   */
  MILLICAST_API virtual  const VideoCapabilities& capability() const = 0;

  /**
   * @brief Set the capability to use for the capture.
   * @param capability The capability.
   * @remark Must be called before start_capture.
   */
  MILLICAST_API virtual void set_capability(const VideoCapabilities& capability) = 0;
  MILLICAST_API virtual void set_capability(VideoCapabilities&& capability)      = 0;
};


}  // millicast

#endif /* MILLICAST_API_SOURCE_H */
