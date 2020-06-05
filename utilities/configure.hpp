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

// single and double type definition
#if defined(__AVX512F__)
    typedef __m512 typef;
    typedef __m512d typed;
#elif defined(__AVX__)
    typedef __m256 typef;
    typedef __m256d typed;
#elif defined(__SSE__)
    typedef __m128 typef;
    typedef __m128d typed;
#else
    typedef float typef;
    typedef double typed;
#endif

#endif