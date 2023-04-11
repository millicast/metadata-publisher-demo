# Millicast native SDK examples

The SDK comes a simple app to check if the SDK installation is correct

## Build

### Mac

In the example folder from the SDK, create a build directory :

```bash

mkdir build && cd build

cmake .. -DMillicastSDK_DIR=/path_to_millicastSDK/lib/cmake

cmake --build .

```

### IOS

IOS build is done on a mac computer.

If you want to build an app in objective C using only XCode (not CMake), see ObjCReadme.md.

### Linux

The required dependencies are X11, Xtst, Xfixes, Xdamage, Xrandr, Xcomposite,
avahi-common, avahi-client and libcurl : 

```bash

sudo apt install -y libx11-dev libxfixes-dev libxdamage-dev libxcomposite-dev libxtst-dev \
  libxrandr-dev libcurl4-openssl-dev libavahi-client3 libavahi-common3

```

Make sure you compile with clang and have the libc++-dev and libc++abi-dev installed.

```bash

export CC=/usr/bin/clang

export CXX=/usr/bin/clang++

# Add path to libndi.so.5 shared library
export LD_LIBRARY_PATH=/usr/lib/x86_64-linux-gnu/millicast-sdk/

```

Then to build the examples :

```bash

mkdir build && cd build

cmake .. -DMillicastSDK_DIR=/path_to/MillicastSDK/lib/cmake

make -j4

```

### Windows

To use the Millicast SDK on Windows, you will need to build with Visual Studio 2022.

In order to build the examples open a Visual Studio developer console and then,
in the *example* directory, run:

```bash

mkdir build && cd build

cmake .. -DMillicastSDK_DIR=/path_to_millicastSDK/lib/cmake

cmake --build . --config RelWithDebInfo

```

Before running an application, you might want to add the bin directory to your path.
It contains the MillicastSDK and ndi DLLs. If they are not in your path environment variable,
the application will not start.
