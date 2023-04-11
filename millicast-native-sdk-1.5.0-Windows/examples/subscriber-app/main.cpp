#include <iostream>
#include <chrono>
#include <thread>
#include <sstream>

#include <millicast-sdk/media.h>
#include <millicast-sdk/track.h>
#include <millicast-sdk/viewer.h>

std::unique_ptr<millicast::Viewer> viewer = millicast::Viewer::create();

class ViewerListener : public millicast::Viewer::Listener
{
public:
    ViewerListener() = default;
    virtual ~ViewerListener() = default;

    // overrides from millicast::Viewer::Listener
    void on_connected() override { viewer->subscribe(); };
    void on_connection_error(int code, const std::string &message) override{};
    void on_stats_report(const millicast::StatsReport &) override{};
    void on_signaling_error(const std::string &reason) override{};

    void on_subscribed() override{};
    void on_subscribed_error(const std::string &error) override{};

    void on_track(std::weak_ptr<millicast::AudioTrack> track,
                  const std::optional<std::string> &mid) override{};
    void on_track(std::weak_ptr<millicast::VideoTrack> track,
                  const std::optional<std::string> &mid) override{};

    void on_vad(const std::string &mid, const std::optional<std::string> &source_id) override{};
    void on_stopped() override{};
    void on_active(const std::string &stream_id,
                   const std::vector<millicast::TrackInfo> &tracks,
                   const std::optional<std::string> &source_id) override{};
    void on_inactive(const std::string &stream_id, const std::optional<std::string> &source_id) override{};
    void on_layers(const std::string &mid,
                   const std::vector<millicast::Viewer::LayerData> &active_layers,
                   const std::vector<millicast::Viewer::LayerData> &inactive_layers) override{};

    void on_viewer_count(int count) override{};
};

int main()
{
    // Create a lambda use to output millicast logs
    auto logger = [](const std::string &msg, millicast::LogLevel) -> void
    {
        std::cout << "Logger : " << msg << std::endl;
    };

    millicast::Logger::set_logger(logger);

    auto listener = std::make_unique<ViewerListener>();
    viewer->set_listener(listener.get());

    auto credentials = viewer->get_credentials(); // Get the current credentials

    credentials.stream_name = "streamName"; // The name of the stream we want to subscribe to
    credentials.account_id = "accountId";   // id of your Dolby.io Streaming APIs account
    // credentials.token = "token"; // optionally set the subscribing token
    credentials.api_url = "https://director.millicast.com/api/director/subscribe"; // The subscribe API URL

    viewer->set_credentials(std::move(credentials)); // Set the new credentials

    // First, you must authenticate and create the websocket connection with a Dolby.io Streaming APIs server:
    viewer->connect();

    // If it is successful, on_subscribed will be called, otherwise on_subscribed_error will be called, with an error message.
    // Once you are subscribed, you will receive event in the listener to get the audio/video tracks that have been created.
    while (true)
    {
        /* code */
    }

    return 0;
}
