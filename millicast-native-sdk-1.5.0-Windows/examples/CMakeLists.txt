cmake_minimum_required( VERSION 3.17 )

project( MillicastSDKExamples )

find_package( MillicastSDK REQUIRED )

## For publisher

add_executable( publisherapp publisher-app/main.cpp )
target_link_libraries( publisherapp PUBLIC Millicast::MillicastSDK )
set_target_properties( publisherapp PROPERTIES CXX_STANDARD 17 )

## For subscriber

add_executable( subscriberapp subscriber-app/main.cpp )
target_link_libraries( subscriberapp PUBLIC Millicast::MillicastSDK )
set_target_properties( subscriberapp PROPERTIES CXX_STANDARD 17 )