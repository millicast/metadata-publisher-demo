#include <iostream>
#include <stdexcept>
#include <sstream>
#include <algorithm>

#include <millicast-sdk/publisher.h>
#include <millicast-sdk/media.h>

std::string get_env(const char* var) 
{
    const char* ret = std::getenv(var);
    return (ret) ? ret : "";
}

const millicast::Publisher::Credentials& get_stream_credentials() 
{
    static const millicast::Publisher::Credentials credentials = {
      .stream_name = get_env("TEST_STREAM_NAME"), // stream_name
      .token =  get_env("TEST_PUB_TOKEN"), // pub_token
      .api_url =  "https://director.millicast.com/api/director/publish", // publish_url
    };

    if (credentials.stream_name.length() == 0 || credentials.token.length() == 0) 
    {
        throw std::runtime_error("Invalid credentials for publishing. Values must be non-empty.");
    }

    return credentials;
}

class MetadataPublisher : public millicast::Publisher::Listener
{
    std::unique_ptr<millicast::Publisher> _publisher{ nullptr };
    int32_t width, height;
    int32_t pos_x, pos_y;
    int8_t dir_x, dir_y;

public:

    MetadataPublisher() noexcept : pos_x{0}, pos_y{0}, dir_x{1}, dir_y{ 1 }
    {
        _publisher = millicast::Publisher::create();
        _publisher->set_listener(this);
    }

    void run()
    {
        auto video_sources = millicast::Media::get_video_sources();
        if (video_sources.empty()) return;

        auto video_source = video_sources.front();
        auto video_track = video_source->start_capture();
        auto credentials = get_stream_credentials();

        _publisher->set_credentials(credentials);
        _publisher->add_track(video_track);
        _publisher->enable_frame_transformer(true);
        _publisher->connect();

        auto cap = video_source->capability();
        width = cap.width;
        height = cap.height;

        pos_x = width / 2;
        pos_y = height / 2;

        [[maybe_unused]] auto _ = std::getchar();
    }

    /* Publisher::Listener overrides */
    void on_connected() override
    {
        _publisher->publish();
    }

    void on_connection_error(int status, const std::string& reason) override 
    {
        millicast::Logger::log(std::to_string(status) + " " + reason,
            millicast::LogLevel::MC_ERROR);
    }

    void on_signaling_error(const std::string& message) override 
    {
        millicast::Logger::log(message, millicast::LogLevel::MC_ERROR);
    }

    void on_stats_report(const millicast::StatsReport&) override {}

    void on_viewer_count(int count) override 
    {
        millicast::Logger::log("Viewer Count : " + std::to_string(count), millicast::LogLevel::MC_LOG);
    }

    void on_publishing() override 
    {
        millicast::Logger::log("Publishing", millicast::LogLevel::MC_LOG);
    }

    void on_publishing_error(const std::string& reason) override 
    {
        millicast::Logger::log(reason, millicast::LogLevel::MC_ERROR);
    }

    void on_active() override {}
    void on_inactive() override {}

    void encode(int32_t value, std::vector<uint8_t>& data)
    {
        data.push_back((value >> 24) & 0xff);
        data.push_back((value >> 16) & 0xff);
        data.push_back((value >> 8) & 0xff);
        data.push_back(value & 0xff);
    }

    void on_transformable_frame([[maybe_unused]] uint32_t ssrc, [[maybe_unused]] uint32_t timestamp, std::vector<uint8_t>& data) override
    {
        constexpr uint8_t SPEED = 10;

        if (pos_x == width || pos_x == 0)
        {
            dir_x *= -1;
        }

        if (pos_y == height || pos_y == 0)
        {
            dir_y *= -1;
        }

        pos_x += dir_x * SPEED;
        pos_y += dir_y * SPEED;

        pos_x = std::clamp(pos_x, 0, width);
        pos_y = std::clamp(pos_y, 0, height);

        encode(pos_x, data);
        encode(pos_y, data);
    }

};

void print_logs(const std::string& msg, millicast::LogLevel lvl)
{
    std::ostringstream oss;

    oss << "[MillicastSDK:";

    switch (lvl)
    {
    case millicast::LogLevel::MC_DEBUG: oss << "Debug]"; break;
    case millicast::LogLevel::MC_LOG: oss << "Log]"; break;
    case millicast::LogLevel::MC_ERROR: oss << "Error]"; break;
    case millicast::LogLevel::MC_FATAL: oss << "Fatal]"; break;
    case millicast::LogLevel::MC_WARNING: oss << "Warning]"; break;
    }

    oss << " " << msg;

    std::cout << oss.str() << std::endl;
}

int main()
{
#ifdef DEBUG_BUILD
  millicast::Logger::disable_rtc_logs();
#endif
  millicast::Logger::set_logger([](const std::string& msg, millicast::LogLevel lvl) -> void { print_logs(msg, lvl); });

  {
      MetadataPublisher publisher;
      publisher.run();
  }

  millicast::Client::cleanup();
  
  return 0;
}
