#ifndef MILLICAST_API_CAPABILITIES_H
#define MILLICAST_API_CAPABILITIES_H

/**
 * @file capabilities.h
 * @author David Baldassin
 * @copyright Copyright 2021 CoSMoSoftware.
 * @date 02/2021
 */

#include <string>

#include "millicast-sdk/exports.h"

namespace millicast
{

  /**
   * @brief The VideoType enum represent the pixel format used for video frames.
   */

  enum class VideoType
  {
    UNKNOWN,
    I420,
    I444,
    I210,
    IYUV,
    RGB24,
    ARGB,
    RGB565,
    YUY2,
    YV12,
    NV12,
    UYVY,
    MJPEG,
    BGRA,
  };

  /**
   * @brief The PrimaryID enum represents the standard of image parameters used for video frames.
   */

  enum class PrimaryID : uint8_t
  {
    // The indices are equal to the values specified in T-REC H.273 Table 2.
    kBT709 = 1,
    kUnspecified = 2,
    kBT470M = 4,
    kBT470BG = 5,
    kSMPTE170M = 6, // Identical to BT601
    kSMPTE240M = 7,
    kFILM = 8,
    kBT2020 = 9,
    kSMPTEST428 = 10,
    kSMPTEST431 = 11,
    kSMPTEST432 = 12,
    kJEDECP22 = 22, // Identical to EBU3213-E
    // When adding/removing entries here, please make sure to do the
    // corresponding change to kPrimaryIds.
  };

  /**
   * @brief The TransferID enum represents the transfer function used for video frames.
   */

  enum class TransferID : uint8_t
  {
    // The indices are equal to the values specified in T-REC H.273 Table 3.
    kBT709 = 1,
    kUnspecified = 2,
    kGAMMA22 = 4,
    kGAMMA28 = 5,
    kSMPTE170M = 6,
    kSMPTE240M = 7,
    kLINEAR = 8,
    kLOG = 9,
    kLOG_SQRT = 10,
    kIEC61966_2_4 = 11,
    kBT1361_ECG = 12,
    kIEC61966_2_1 = 13,
    kBT2020_10 = 14,
    kBT2020_12 = 15,
    kSMPTEST2084 = 16,
    kSMPTEST428 = 17,
    kARIB_STD_B67 = 18,
    // When adding/removing entries here, please make sure to do the
    // corresponding change to kTransferIds.
  };

  /**
   * @brief The MatrixID enum represents the color matrix to use for conversion between color spaces.
   */

  enum class MatrixID : uint8_t
  {
    // The indices are equal to the values specified in T-REC H.273 Table 4.
    kRGB = 0,
    kBT709 = 1,
    kUnspecified = 2,
    kFCC = 4,
    kBT470BG = 5,
    kSMPTE170M = 6,
    kSMPTE240M = 7,
    kYCOCG = 8,
    kBT2020_NCL = 9,
    kBT2020_CL = 10,
    kSMPTE2085 = 11,
    kCDNCLS = 12,
    kCDCLS = 13,
    kBT2100_ICTCP = 14,
    // When adding/removing entries here, please make sure to do the
    // corresponding change to kMatrixIds.
  };

  /**
   * @brief The RangeID enum represents the color range values.
   */

  enum class RangeID
  {
    // The indices are equal to the values specified at
    // https://www.webmproject.org/docs/container/#colour for the element Range.
    kInvalid = 0,
    // Limited Rec. 709 color range with RGB values ranging from 16 to 235.
    kLimited = 1,
    // Full RGB color range with RGB valees from 0 to 255.
    kFull = 2,
    // Range is defined by MatrixCoefficients/TransferCharacteristics.
    kDerived = 3,
    // When adding/removing entries here, please make sure to do the
    // corresponding change to kRangeIds.
  };

  /**
   * @brief video_type_to_str convert a VideoType enum value to a std::string
   * @param t The video type as described in VideoType enum.
   * @return The video type name
   */

  MILLICAST_API std::string video_type_to_str(VideoType t);

  /**
   * @brief The VideoCapabilities struct
   */

  class VideoCapabilities
  {
  public:
    int width{};                                   /**< The width of the captured video frame */
    int height{};                                  /**< The height of the captured video frame */
    int fps{};                                     /**< The frame rate at which the capture device shall capture */
    VideoType format{VideoType::UNKNOWN};          /**< The pixel format to use for the capture */
    PrimaryID primary{PrimaryID::kUnspecified};    /**< The standard of image parameters to use for the capture */
    TransferID transfer{TransferID::kUnspecified}; /**< The transfer function to use for the capture */
    MatrixID matrix{MatrixID::kUnspecified};       /**< The color matrix to use for conversion between color spaces */
    RangeID range{RangeID::kInvalid};              /**< The color range values to use for the capture */

    /**
     * @brief Specify the capabilities with a video standard such as 1080p30
     * Implemented only for decklink devices
     */
    std::string video_standard;

    MILLICAST_API std::string format_as_str() const; /**< Get the pixel format as a std::string */
  };

} // millicast

#endif /* MILLICAST_API_CAPABILITIES_H */
