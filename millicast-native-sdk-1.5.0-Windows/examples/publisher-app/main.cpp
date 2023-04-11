#include <iostream>
#include <chrono>
#include <thread>

#include <millicast-sdk/media.h>
#include <millicast-sdk/publisher.h>

// Create publisher
std::unique_ptr<millicast::Publisher> publisher = millicast::Publisher::create();

class PubListener : public millicast::Publisher::Listener
{
public:
  PubListener() = default;
  virtual ~PubListener() = default;

  void on_connected() override { publisher->publish(); }
  void on_connection_error(int, const std::string &) override {}
  void on_signaling_error(const std::string &) override {}
  void on_publishing() override {}
  void on_publishing_error(const std::string &) override {}
  void on_stats_report(const millicast::StatsReport &report) override {}
  void on_active() override {}
  void on_inactive() override {}
  void on_viewer_count(int) override {}
};

int main()
{
  // Create a lambda use to output millicast logs
  auto logger = [](const std::string &msg, millicast::LogLevel) -> void
  {
    std::cout << "Logger : " << msg << std::endl;
  };

  millicast::Logger::set_logger(logger);

  auto audio_src = millicast::Media::get_audio_sources();

  // Starting the capture, returning an audio track, to add it to the publisher later
  auto audio_src_selected = audio_src[0]; // get the first available source
  auto audio_track = audio_src_selected->start_capture();
  publisher->add_track(audio_track);

  auto video_src = millicast::Media::get_video_sources();

  // Starting the capture, to get the returned track and keep it for adding it to the publisher later.

  for (int i = 0; i < video_src.size(); i++)
  {
    if (video_src[i]->name() == "Integrated Camera")
    {
      auto video_src_selected = video_src[i];
      auto video_track = video_src_selected->start_capture();
      publisher->add_track(video_track);
    }
    else
    {
      auto video_src_selected = video_src[i]; // get the first available source
    }
  }

  // Display all the sources found
  std::cout << "Audio sources : " << std::endl;
  for (auto src : audio_src)
    std::cout << src->name() << "\n";
  std::cout << "Video sources : " << std::endl;
  for (auto src : video_src)
    std::cout << src->name() << "\n";

  auto listener = std::make_unique<PubListener>();

  publisher->set_listener(listener.get());

  // Fill the credentials
  auto creds = publisher->get_credentials();
  creds.api_url = "some_url";
  creds.stream_name = "some_stream";
  creds.token = "le_token";

  bool r = publisher->set_credentials(std::move(creds));

  // Check if credentials have been set in the publisher
  if (r)
    std::cout << "credentials set sucessfully"
              << "\n";
  else
    std::cout << "credentials are not valid"
              << "\n";

  // Configuring the Publisher
  millicast::Publisher::Option options;

  // List of the supported codec names
  auto audio_codecs = millicast::Client::get_supported_audio_codecs();
  auto video_codecs = millicast::Client::get_supported_video_codecs();

  // Setting the first audio codec of the list
  options.codecs.video = video_codecs.front();
  options.codecs.audio = audio_codecs.front();

  publisher->set_options(options);
  
  publisher->connect();

  while (true)
  {
  }

  return 0;
}
