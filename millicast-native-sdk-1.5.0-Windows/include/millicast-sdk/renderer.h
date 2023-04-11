#ifndef MILLICAST_API_RENDERER_H
#define MILLICAST_API_RENDERER_H

/**
  * @file renderer.h
  * @author David Baldassin
  * @copyright Copyright 2021 CoSMoSoftware.
  * @date 02/2021
  */

#include <cstddef> // size_t
#include <cstdint> // uint32_t

#include "millicast-sdk/frames.h"
#include "millicast-sdk/source.h"

namespace millicast {

// base ///////////////////////////////////////////////////////////////////////

/**
* @brief The Renderer base class. A renderer is used to render video on screen
* or render audio.
*/

class Renderer
{
public:
  Renderer() = default;
  virtual ~Renderer() = default;

  /**
   * @brief init the renderer
   */

  virtual void init() = 0;
};

// Video //////////////////////////////////////////////////////////////////////

/**
 * @brief The VideoRenderer class
 * Inherits this class to receive video frames and render them in your application.
 */

class VideoRenderer : public Renderer
{
public:
  VideoRenderer() = default;
  virtual ~VideoRenderer() = default;

  /**
   * @brief on_frame is called when a new video frame is available
   * ( either captured or received from a peer )
   * @param frame The video frame
   */

  virtual void on_frame(const VideoFrame& frame) = 0;
};

// Audio //////////////////////////////////////////////////////////////////////

/**
 * @brief The AudioRenderer class
 * inherits this if you want to render audio in a specific way in your application.
 * @remark The recommended method to render audio is to use AudioPlayback
 * @see AudioPlayback
 */

class AudioRenderer
{
public:
  AudioRenderer() = default;
  virtual ~AudioRenderer() = default;

  /**
   * @brief on_frame is called when a new audio frame is available.
   * @param frame The audio frame.
   */

  virtual void on_frame(const AudioFrame& frame) = 0;
};

// decklink ///////////////////////////////////////////////////////////////////

/**
 * @brief The DeckLinkRenderer class is used to render video on a DeckLink device.
 * @remark For now, this class does not render audio on decklink devices,
 * use AudioPlayback with a DeckLink device instead.
 */

class DeckLinkRenderer : public VideoRenderer
{
public:
  DeckLinkRenderer() = default;
  virtual ~DeckLinkRenderer() = default;

  /**
   * @brief Set the decklink device to used to render video.
   * @param device The DeckLink device.
   */

  MILLICAST_API virtual void set_source(VideoSource::Ptr device) = 0;
  MILLICAST_API virtual void stop() = 0;

  /**
   * @brief Get all decklink devices that can output video.
   * @return An array of decklink output devices.
   */
  
  MILLICAST_API static std::vector<VideoSource::Ptr> get_decklink_devices();

  /**
   * @brief Create a DeckLink renderer.
   * @return A DeckLink renderer object.
   */

  MILLICAST_API static DeckLinkRenderer* create();
};


// ndi ////////////////////////////////////////////////////////////////////////

/**
 * @brief The NdiRenderer class is used to render video as an ndi source.
 * @remark For now, this class does not render audio,
 * use AudioPlayback with Ndi output instead.
 */

class NdiRenderer : public VideoRenderer
{
public:
  NdiRenderer() = default;
  virtual ~NdiRenderer() = default;

  /**
   * @brief Set the name of the ndi source.
   * This is the name that will be displayed to other ndi application when they
   * search for ndi sources.
   * @param name The name of the source.
   */

  MILLICAST_API virtual void set_name(const std::string&) = 0;

  /**
   * @brief Create an Ndi renderer.
   * @return An Ndi renderer object.
   */

  MILLICAST_API static NdiRenderer* create();
};

#if defined MILLICAST_SDK_TVOS || defined MILLICAST_SDK_IOS

class MILLICAST_API IosVideoRenderer : public VideoRenderer
{
public:

  IosVideoRenderer() = default;
  virtual ~IosVideoRenderer() = default;

  virtual void * get_uiview() = 0;
  virtual float width() = 0;
  virtual float height() = 0;

  static IosVideoRenderer * create();

  std::function<void(float, float)> did_change_video_size;
};

#endif

}  // millicast

#endif /* MILLICAST_API_RENDERER_H */
