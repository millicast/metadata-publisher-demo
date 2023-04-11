#ifndef MILLICAST_API_FRAMES_H
#define MILLICAST_API_FRAMES_H

#include "millicast-sdk/exports.h"
#include "millicast-sdk/capabilities.h"

// video //////////////////////////////////////////////////////////////////////


namespace millicast
{

/**
 * @brief The VideoFrame class used to described a VideoFrame
 */

class MILLICAST_API VideoFrame
{
public:
  VideoFrame() = default;
  ~VideoFrame() = default;

  VideoFrame(const VideoFrame&) = delete;
  VideoFrame(VideoFrame&&) = delete;
  
  /**
   * @brief Get the width of the video frame
   * @return The width
   */

  virtual int width()  const = 0;

  /**
   * @brief Get the height of the video frame
   * @return The height
   */

  virtual int height() const = 0;

  virtual uint32_t timestamp() const = 0;

  /**
   * @brief Get the type (ex.: I420) of the video frame
   * @return The type
   */

  virtual VideoType frame_type() const = 0;

  /**
   * @brief Get the standard (ex.: BT709) used for image parameters of the video frame
   * @return The primary ID
   */

  virtual PrimaryID primaries() const = 0;

  /**
   * @brief Get the transfer function used for the video frame
   * @return The transfer function ID
   */

  virtual TransferID transfer() const = 0;

  /**
   * @brief Get the color matrix to use for conversion between color spaces
   * @return The color matrix ID
   */

  virtual MatrixID matrix() const = 0;

  /**
   * @brief Get the color range values of the video frame
   * @return The color range ID
   */

  virtual RangeID range() const = 0;

  /**
   * @brief Get the buffer size for the specified video type.
   * @param T The VideoType.
   */
  virtual uint32_t size(VideoType type) const = 0;

  /**
   * @brief Get the video frame buffer as the video type specified in parameter.
   * @param type The VideoType.
   * @param buffer The video frame buffer. It must be allocated before calling this.
   * @remark This method assume the buffer passed in parameter is allocated correctly
   * corresponding to the specified VideoType.
   */
  virtual void get_buffer(VideoType type, uint8_t * buffer) const = 0;
};

// audio //////////////////////////////////////////////////////////////////////

/**
 * @brief The AudioFrame struct used to described audio data.
 */

struct MILLICAST_API AudioFrame
{
  const void * data; /**< The audio data */
  int          bits_per_sample; /**< The number of bits per sample, 16 bits or 32 bits only */
  int          sample_rate; /**< The sample rate of the audio data, must be 48kHz */
  size_t       number_of_channels; /**< The number of channels used */
  size_t       number_of_frames; /**< The number of frames contained in this object. This is dependant on the length of each frame in time as well as the sample rate. 
                                      For example, if each frame equates to 10 milliseconds of playback, and the sample rate is 48 kHz, then the number of frames = 10 ms * 48 kHz = 480 frames*/
};

}

#endif /* FRAMES_H */
