#ifndef MILLICAST_EXPORTS_H
#define MILLICAST_EXPORTS_H

#ifdef MILLICAST_API_EXPORT

#if defined MILLICAST_SDK_MAC || defined MILLICAST_SDK_LINUX
#define MILLICAST_API __attribute__((visibility ("default")))
#elif defined MILLICAST_SDK_WIN
#define MILLICAST_API __declspec(dllexport)
#endif
#else
#ifdef MILLICAST_SDK_WIN
#define MILLICAST_API __declspec(dllimport)
#else
#define MILLICAST_API
#endif
#endif

#endif // EXPORTS_H
