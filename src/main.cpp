#include <millicast-sdk/client.h>

int main()
{
#ifdef DEBUG_BUILD
  millicast::Logger::disable_rtc_logs();
#endif

  millicast::Client::cleanup();
  
  return 0;
}
