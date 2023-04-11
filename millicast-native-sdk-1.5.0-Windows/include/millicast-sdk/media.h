#ifndef MILLICAST_API_MEDIA_H
#define MILLICAST_API_MEDIA_H

/**
  * @file media.h
  * @author David Baldassin
  * @copyright Copyright 2021 CoSMoSoftware.
  * @date 02/2021
  */

#include <vector>

#include "millicast-sdk/mc_logging.h"
#include "millicast-sdk/source.h"

namespace millicast {

/**
 * @brief Namespace where you can find constant for the codec names.
 */
namespace Codec
{
  /* Video */
  constexpr char H264_CODEC_NAME[] = "H264";
  constexpr char VP8_CODEC_NAME[] = "VP8";
  constexpr char VP9_CODEC_NAME[] = "VP9";
  constexpr char AV1_CODEC_NAME[] = "AV1";

  /* Audio */
  constexpr char OPUS_CODEC_NAME[] = "opus";
  constexpr char MULTIOPUS_CODEC_NAME[] = "multiopus";
}

/**
* @brief The Media class is used to manage media sources.
*/

class MILLICAST_API Media
{
public:

  /**
   * @brief get_video_sources get all the available video source (device, desktop, ...)
   * this funcion will return only the video sources already found.
   * To fetch new video sources, call refresh first.
   * @see VideoSource
   * @return An array of all found video source
   */

  static std::vector<VideoSource::Ptr> get_video_sources();

  /**
   * @brief get_audio_sources get all the available audio source (device, ndi, ...)
   * this funcion will return only the audio sources already found.
   * To fetch new audio sources, call refresh first.
   * @see AudioSource
   * @return An array of all found audio source
   */

  static std::vector<AudioSource::Ptr> get_audio_sources();

  /**
   * @brief get_playback_devices, get all the available audio playback (device, ndi, ...)
   * this funcion will return only the audio playback already found.
   * To fetch new audio playback, call refresh first.
   * @see AudioPlayback
   * @return An array of all found audio playback devices
   */

  static std::vector<AudioPlayback::Ptr> get_playback_devices();

  /**
   * @brief is_ndi_available, check if ndi support is enabled in the sdk
   * @return true if ndi is available false otherwise.
   */

  static bool is_ndi_available();

  /**
   * @brief is_decklink_available, check if decklink devices support is enabled in the sdk
   * @return true if decklink is available false otherwise.
   */

  static bool is_decklink_available();
};


}  // millicast

#endif /* MILLICAST_API_MEDIA_H */
