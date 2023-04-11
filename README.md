# Millicast Metadata Publisher

This project is to show how to embed metadata into video frames when publishing with the millicast native sdk.

It will fetch a video device and send a XY position along with the frame.

You can see the related player and how we can move a logo according to these positions : https://github.com/millicast/millicast-player-ue5-example/tree/extract-metadata

For now only windows is supported.

## Build

You need to have cmake and visual studio 2022 installed.

```
make build

cd build

cmake .. -DMillicastSDK=cmake .. -DMillicastSDK_DIR=millicast-native-sdk-1.5.0-Windows\lib\cmake

cmake --build . --config Release

```

## Run

You need to set your credentials through the following environment variable

* TEST_STREAM_NAME : your stream name
* TEST_PUB_TOKEN : your publishing token

Then in the build directory run 

```
.\src\Release\metadata-publisher.exe
```

This will start publishing the first video source of your computer and wait for an input to close the program.