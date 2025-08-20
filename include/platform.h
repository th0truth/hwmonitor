#pragma once

// Unix OS
#if defined(_UNIX) || defined(__unix) || defined(__unix__)
#define HWMONITOR_UNIX
#endif

// for both 32-bit and 64-bit Windows OS
#if defined(_WIN32)
#define HWMONITOR_WIN
#endif