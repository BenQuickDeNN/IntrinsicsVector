/*************************************************
 * @author Bin Qu
 * @email benquickdenn@foxmail.com
*************************************************/

#ifndef VECF_HPP

#define VECF_HPP

#include "vec.hpp"

/**
 * Vec class with single precision
 */
class Vecf : Vec
{
private:
#if defined(__AVX512F__)
    __m512 val;
#elif defined(__AVX__)
    __m256 val;
#elif defined(__SSE__)
    __m128 val;
#else
    float val;
#endif

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

#if defined(__AVX512F__)
    inline Vecf & operator = (const __m512& val) { this->val = val; return *this; }

    inline __m512& Val() { return val; }
    inline void setVal(const __m512& val) { this->val = val; }
#elif defined(__AVX__)
    inline Vecf & operator = (const __m256& val) { this->val = val; return *this; }

    inline __m256& Val() { return val; }
    inline void setVal(const __m256& val) { this->val = val; }
#elif defined(__SSE__)
    inline Vecf & operator = (const __m128& val) { this->val = val; return *this; }

    inline __m128& Val() { return val; }
    inline void setVal(const __m128& val) { this->val = val; }
#else
    inline Vecf & operator = (const float& val) { this->val = val; return *this; }

    inline float& Val() { return val; }
    inline void setVal(const float& val) { this->val = val; }
#endif
};

static inline Vecf & operator + (Vecf& vec1, Vecf& vec2);
static inline Vecf & operator - (Vecf& vec1, Vecf& vec2);
static inline Vecf & operator * (Vecf& vec1, Vecf& vec2);
static inline Vecf & operator / (Vecf& vec1, Vecf& vec2);

static inline Vecf & operator + (Vecf& vec1, Vecf& vec2)
{
    Vecf ret;
#if defined(__AVX512F__)
    ret.setVal(_mm512_add_ps(vec1.Val(), vec2.Val()));
#elif defined(__AVX__)
    ret.setVal(_mm256_add_ps(vec1.Val(), vec2.Val()));
#elif defined(__SSE__)
    ret.setVal(_mm_add_ps(vec1.Val(), vec2.Val()));
#else
    ret.setVal(vec1.Val() + vec2.Val());
#endif
    return ret;
}

#endif