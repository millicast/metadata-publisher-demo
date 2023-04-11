#ifndef MILLICAST_API_STATS_H
#define MILLICAST_API_STATS_H

/**
 * @file stats.h
 * @author David Baldassin
 * @copyright Copyright 2021 CoSMoSoftware.
 * @date 02/2021
 */

#include <string>
#include <memory>
#include <vector>
#include <map>
#include <optional>

#include "millicast-sdk/exports.h"

namespace millicast
{

  namespace rtcstats
  {

    enum class Type
    {
      CODEC,
      OUTBOUND_RTP,
      INBOUND_RTP,
      REMOTE_INBOUND_RTP,
      REMOTE_OUTBOUND_RTP,
      MEDIA_TRACK,
      AUDIO_TRACK,
      VIDEO_TRACK,
      MEDIA_SOURCE
    };

    struct Stats
    {

      /**
       * @brief The timestamp in Milliseconds since Unix Epoch (Jan 1, 1970 00:00:00 UTC).
       */
      int64_t timestamp;
      std::string id;
      Type type;

      template <typename T>
      const T *cast_to() const { return static_cast<const T *>(this); }
    };

    struct Codecs : public Stats
    {
      enum class Type
      {
        ENCODE,
        DECODE
      };
      static constexpr auto STYPE = ::millicast::rtcstats::Type::CODEC;

      unsigned long payload_type;
      std::string transport_id;
      std::string mime_type;

      std::optional<Type> codec_type;
      std::optional<unsigned long> clock_rate;
      std::optional<unsigned long> channels;
      std::optional<std::string> sdp_fmtp_line;
    };

    struct RtpStream : public Stats
    {
      unsigned long ssrc;
      std::string kind;
      std::optional<std::string> transport_id;
      std::optional<std::string> codec_id;
    };

    struct ReceivedRtpStream : public RtpStream
    {
      static constexpr auto STYPE = Type::INBOUND_RTP;

      std::optional<unsigned long long> packets_received;
      std::optional<double> jitter;
      std::optional<long long> packets_lost;
      std::optional<unsigned long long> frames_dropped;
    };

    struct InboundRtpStream : public ReceivedRtpStream
    {
      static constexpr auto STYPE = Type::INBOUND_RTP;

      std::optional<std::string> remote_id;
      std::optional<unsigned long> frames_decoded;
      std::optional<unsigned long> nack_count;
      std::optional<unsigned long> frames_received;
      std::optional<unsigned long> frame_width;
      std::optional<unsigned long> frame_height;
      std::optional<double> frames_per_second;
      std::optional<unsigned long long> bytes_received;
      std::optional<double> audio_level;
      std::optional<double> total_audio_energy;
      std::optional<double> total_samples_duration;
      std::optional<std::string> decoder_implementation;
    };

    struct SentRtpStream : public RtpStream
    {
      static constexpr auto STYPE = Type::OUTBOUND_RTP;
      unsigned long packets_sent;
      unsigned long long bytes_sent;
    };

    struct OutboundRtpStream : public SentRtpStream
    {
      static constexpr auto STYPE = Type::OUTBOUND_RTP;
      std::optional<std::string> sender_id;
      std::optional<std::string> remote_id;
      std::optional<double> target_bitrate;
      std::optional<unsigned long> frame_width;
      std::optional<unsigned long> frame_height;
      std::optional<double> frames_per_second;
      std::optional<unsigned long> frames_sent;
      std::optional<unsigned long> frames_encoded;
      std::optional<unsigned long> nack_count;
      std::optional<std::string> encoder_implementation;
    };

    struct RemoteOutboundRtpStream : public SentRtpStream
    {
      static constexpr auto STYPE = Type::REMOTE_OUTBOUND_RTP;
      std::string local_id;
      double remote_timestamp;
      unsigned long long reports_sent;
      double round_trip_time;
      unsigned long long round_trip_time_measurements;
      double total_round_trip_time;
    };

    struct RemoteInboundRtpStream : public ReceivedRtpStream
    {
      static constexpr auto STYPE = Type::REMOTE_INBOUND_RTP;
      std::string local_id;
      double round_trip_time;
      double total_round_trip_time;
      unsigned long round_trip_time_measurements;
      double fraction_lost; // fraction packet loss
    };

    struct MediaStreamTrack : public Stats
    {
      static constexpr auto STYPE = Type::MEDIA_TRACK;
      std::string track_identifier;
      std::string kind;
      std::string media_source_id;
    };

    struct VideoStreamTrack : public MediaStreamTrack
    {
      static constexpr auto STYPE = Type::VIDEO_TRACK;
      std::optional<unsigned long> width;
      std::optional<unsigned long> height;
      std::optional<unsigned long> bit_depth;
      std::optional<unsigned long> frames_sent;
      std::optional<unsigned long> frames_received;
    };

    struct AudioStreamTrack : public MediaStreamTrack
    {
      static constexpr auto STYPE = Type::AUDIO_TRACK;
      std::optional<double> audio_level;
      std::optional<double> total_audio_energy;
      std::optional<double> total_samples_duration;
    };

    struct MediaSource : public Stats
    {
      static constexpr auto STYPE = Type::MEDIA_SOURCE;
      std::string track_identifier;
      std::string kind;
    };

    struct VideoSource : public MediaSource
    {
      static constexpr auto STYPE = Type::MEDIA_SOURCE;
      std::optional<unsigned long> width;
      std::optional<unsigned long> height;
      std::optional<unsigned long> frames;
      std::optional<double> frames_per_second;
    };

    struct AudioSource : public MediaSource
    {
      static constexpr auto STYPE = Type::MEDIA_SOURCE;
      std::optional<double> audio_level;
      std::optional<double> total_audio_energy;
      std::optional<double> total_samples_duration;
      std::optional<double> echo_return_loss;
      std::optional<double> echo_return_loss_enhancement;
    };
  }

  using StatObject = std::unique_ptr<const rtcstats::Stats>;

  class MILLICAST_API StatsReport
  {
  public:
    class const_iterator
    {
      std::map<std::string, StatObject>::const_iterator _it;

    public:
      const_iterator(std::map<std::string, StatObject>::const_iterator it) : _it(it) {}
      const_iterator(const_iterator &&other) = default;
      ~const_iterator() = default;

      const_iterator &operator++()
      {
        ++_it;
        return *this;
      }
      const_iterator &operator++(int) { return ++(*this); }
      const rtcstats::Stats &operator*() const { return *_it->second.get(); }
      const rtcstats::Stats *operator->() const { return _it->second.get(); }
      bool operator==(const const_iterator &other) const { return _it == other._it; }
      bool operator!=(const const_iterator &other) const { return !(*this == other); }
    };

    StatsReport();
    virtual ~StatsReport() = default;

    virtual void add_stats(StatObject stats) = 0;
    virtual const rtcstats::Stats *get(const std::string &id) const = 0;
    template <typename T>
    const T *get_as(const std::string &id) const
    {
      const rtcstats::Stats *stats = get(id);
      if (!stats || stats->type != T::STYPE)
        return nullptr;

      return stats->cast_to<const T>();
    }

    virtual const_iterator begin() const = 0;
    virtual const_iterator end() const = 0;

    template <typename T>
    std::vector<const T *> get_stats_of_type() const
    {
      std::vector<const T *> stats_of_type;
      for (const auto &stats : *this)
      {
        if (stats.type == T::STYPE)
          stats_of_type.push_back(stats.cast_to<const T>());
      }
      return stats_of_type;
    }

    // Removes the stats object from the report, returning ownership of it or null
    // if there is no object with `id`.
    virtual StatObject take(const std::string &id) = 0;

    virtual size_t size() const = 0;
  };

} // millicast

#endif /* MILLICAST_API_STATS_H */
