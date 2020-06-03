/*************************************************
 * @author Bin Qu
 * @email benquickdenn@foxmail.com
*************************************************/

#ifndef VECF_HPP

#define VECF_HPP

#include "vec.hpp"

#if defined(__AVX512F__)
    typedef __m512 typef;
#elif defined(__AVX__)
    typedef __m256 typef;
#elif defined(__SSE__)
    typedef __m128 typef;
#else
    typedef float typef;
#endif

/**
 * Vec class with single precision
 */
class Vecf : Vec
{

private:
    typef val;

public:
    inline Vecf & operator = (Vecf& vec) { this->val = vec.Val(); return *this; }

    void load(const float* arr)
    {
#if defined(__AVX512F__)
        val = _mm512_load_ps(arr);
#elif defined(__AVX__)
        val = _mm256_load_ps(arr);
#elif defined(__SSE__)
        val = _mm_load_ps(arr);
#else
        val = *arr;
#endif
    }

    void store(float* arr)
    {
#if defined(__AVX512F__)
        _mm512_store_ps(arr, val);
#elif defined(__AVX__)
        _mm256_store_ps(arr, val);
#elif defined(__SSE__)
        _mm_store_ps(arr, val);
#else
        *arr = val;
#endif    
    }

    int size()
    {
#if defined(__AVX512F__)
        return 16;
#elif defined(__AVX__)
        return 8;
#elif defined(__SSE__)
        return 4;
#else
        return 1;
#endif
    }

    inline Vecf & operator = (const typef& val) { this->val = val; return *this; }
    inline typef& Val() { return val; }
    inline void setVal(const typef& val) { this->val = val; }
};

static inline typef operator + (Vecf& vec1, Vecf& vec2);
static inline typef operator - (Vecf& vec1, Vecf& vec2);
static inline typef operator * (Vecf& vec1, Vecf& vec2);
static inline typef operator / (Vecf& vec1, Vecf& vec2);

static inline typef operator + (Vecf& vec1, Vecf& vec2)
{
#if defined(__AVX512F__)
    return _mm512_add_ps(vec1.Val(), vec2.Val());
#elif defined(__AVX__)
    return _mm256_add_ps(vec1.Val(), vec2.Val());
#elif defined(__SSE__)
    return _mm_add_ps(vec1.Val(), vec2.Val());
#else
    return vec1.Val() + vec2.Val();
#endif
}

#endif