#pragma once

#ifdef __WIN32
#define CE_WIN32
#elif defined(__linux__)
#define CE_LINUX
#elif defined(__APPLE__)
#define CE_MACOS
#else
#define CE_UNKNOWN_PLATFORM
#endif

#ifdef _DEBUG
// Whether or not its in debug mode.
#define CE_DEBUG_MODE
#endif

// TODO: Version
// The current version
#define CE_VERSION "0.0.0.1"

// Define CE_API macros

#ifdef CE_WIN32
// The dll export
#define CE_API __declspec(dllexport)
#else
// The dll import
#define CE_API __declspec(dllimport)
#endif

//? Common types

typedef unsigned char CeU8;
typedef unsigned short CeU16;
typedef unsigned int CeU32;
typedef unsigned long long CeU64;

typedef signed char CeI8;
typedef signed short CeI16;
typedef signed int CeI32;
typedef signed long long CeI64;
