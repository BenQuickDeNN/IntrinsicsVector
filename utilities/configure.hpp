/*************************************************
 * @author Bin Qu
 * @email benquickdenn@foxmail.com
*************************************************/

#ifndef CONFIGURE_HPP

#define CONFIGURE_HPP

#if defined(_WIN32)

#include <intrin.h> // Windows intrinsics header file
#include <Windows.h>

#elif defined(__linux__)

#include <x86intrin.h> // Linux intrinsics header file
#include <sys/time.h>

#endif

#include <cstdlib>

#endif