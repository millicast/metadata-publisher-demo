#ifndef MILLICAST_API_TRACK_H
#define MILLICAST_API_TRACK_H

/**
  * @file track.h
  * @author David Baldassin
  * @copyright Copyright 2021 CoSMoSoftware.
  * @date 02/2021
  */

#include <string>
#include "millicast-sdk/exports.h"

namespace millicast {

// Forward declarations ///////////////////////////////////////////////////////
class VideoRenderer;
class AudioRenderer;

// Track //////////////////////////////////////////////////////////////////////

/**
 * @brief The Track class represent a media sources.
 */

class MILLICAST_API Track
{ 
public:
  Track()          = default;
  virtual ~Track() = default;

  /**
   * @brief Get the track's id.
   * @return The track's id.
   */

  virtual const std::string& id()   const = 0;

  /**
   * @brief Get the track's kind.
   * @return The track's kind. Either audio or video.
   */

  virtual const std::string& kind() const = 0;

  /**
   * @brief Tell whether the track is enabled or not.
   * @return true if the track is enabled, false otherwise.
   */

  virtual bool is_enabled()         const = 0;

  /**
   * @brief enable or disable the track.
   * A disabled track will produce silence (if audio) or black frames (if video).
   * Can be disabled and re-enabled.
   * @param e true to enable, false to disable.
   */

  virtual void enable(bool e)             = 0;
};

// VideoTrack /////////////////////////////////////////////////////////////////

/**
 * @brief The VideoTrack class
 */

class MILLICAST_API VideoTrack : public Track
{
public:

  /**
   * @brief Video track content hint, used to override the source is_screencast property.
   */
  enum class ContentHint { NONE, FLUID, DETAILED, TEXT };
  
  VideoTrack()          = default;
  virtual ~VideoTrack() = default;

  /**
   * @brief Add a VideoRenderer to render this video track.
   * Several renderers can be added to the track.
   * Each one will be called when a new frame is available.
   * @param renderer The video renderer.
   */

  virtual void add_renderer(VideoRenderer* renderer) = 0;

  /**
   * @brief Remove a renderer from the renderer list.
   * @param renderer The renderer to remove.
   */

  virtual void remove_renderer(VideoRenderer* renderer) = 0;

  /**
   * @brief Set the content hint for this track
   * @param hint The hint to set
   */

  virtual void set_content_hint(ContentHint hint) noexcept = 0;

  /**
   * @brief Get the current content hint for this track
   * @return The current content hint
   */
  virtual ContentHint content_hint() const noexcept = 0;
};

// AudioTrack /////////////////////////////////////////////////////////////////

/**
 * @brief The AudioTrack class
 */

class MILLICAST_API AudioTrack : public Track
{
public:
  AudioTrack()          = default;
  virtual ~AudioTrack() = default;

  /**
   * @brief Add an audio renderer to render this track.
   * Several renderers can be added to the track.
   * Each one will be called when a new frame is available.
   * @param renderer The audio renderer.
   */

  virtual void add_renderer(AudioRenderer* renderer) = 0;

  /**
   * @brief Remove a renderer from the renderer list.
   * @param renderer The renderer to remove.
   */

  virtual void remove_renderer(AudioRenderer* renderer) = 0;

  /**
   * @brief Set the track's volume
   * @param volume The volume as a double between 0 and 1
   * @warning Only affect remote track
   */
  virtual void set_volume(double volume) = 0;
};


}  // millicast

#endif /* TRACK_H */
