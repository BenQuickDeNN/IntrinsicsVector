/*************************************************
 * @author Bin Qu
 * @email benquickdenn@foxmail.com
*************************************************/

#ifndef VECF_HPP

#define VECF_HPP

#include "vec.hpp"

static inline Vecf operator + (const Vecf& vec1, const Vecf& vec2);
static inline Vecf operator - (const Vecf& vec1, const Vecf& vec2);
static inline Vecf operator * (const Vecf& vec1, const Vecf& vec2);
static inline Vecf operator / (const Vecf& vec1, const Vecf& vec2);

/**
 * Vec class with single precision
 */
class Vecf : public Vec
{

private:
    typef val;

public:
    Vecf() {}
    Vecf(const typef& val) : val(val) {}

    inline Vecf & operator = (const Vecf& vec) { this->val = vec.Val(); return *this; }
    inline Vecf & operator = (const typef& val) { this->val = val; return *this; }

    inline typef Val() const { return val; }
    inline void setVal(const typef& val) { this->val = val; }

    void load(const float* arr);
    void store(float* arr);

    static int size();
};

#if defined(__AVX512F__)

void Vecf::load(const float* arr) { val = _mm512_loadu_ps(arr); }
void Vecf::store(float* arr) { _mm512_storeu_ps(arr, val); }
int Vecf::size() { return 16; }

static inline Vecf operator + (const Vecf& vec1, const Vecf& vec2) 
{ return Vecf(_mm512_add_ps(vec1.Val(), vec2.Val())); }

static inline Vecf operator - (const Vecf& vec1, const Vecf& vec2)
{ return Vecf(_mm512_sub_ps(vec1.Val(), vec2.Val())); }

static inline Vecf operator * (const Vecf& vec1, const Vecf& vec2)
{ return Vecf(_mm512_mul_ps(vec1.Val(), vec2.Val())); }

static inline Vecf operator / (const Vecf& vec1, const Vecf& vec2)
{ return Vecf(_mm512_div_ps(vec1.Val(), vec2.Val())); }

#elif defined(__AVX__)

void Vecf::load(const float* arr) { val = _mm256_loadu_ps(arr); }
void Vecf::store(float* arr) { _mm256_storeu_ps(arr, val); }
int Vecf::size() { return 8; }

static inline Vecf operator + (const Vecf& vec1, const Vecf& vec2) 
{ return Vecf(_mm256_add_ps(vec1.Val(), vec2.Val())); }

static inline Vecf operator - (const Vecf& vec1, const Vecf& vec2)
{ return Vecf(_mm256_sub_ps(vec1.Val(), vec2.Val())); }

static inline Vecf operator * (const Vecf& vec1, const Vecf& vec2)
{ return Vecf(_mm256_mul_ps(vec1.Val(), vec2.Val())); }

static inline Vecf operator / (const Vecf& vec1, const Vecf& vec2)
{ return Vecf(_mm256_div_ps(vec1.Val(), vec2.Val())); }

#elif defined(__SSE__)

void Vecf::load(const float* arr) { val = _mm_loadu_ps(arr); }
void Vecf::store(float* arr) { _mm_storeu_ps(arr, val); }
int Vecf::size() { return 4; }

static inline Vecf operator + (const Vecf& vec1, const Vecf& vec2) 
{ return Vecf(_mm_add_ps(vec1.Val(), vec2.Val())); }

static inline Vecf operator - (const Vecf& vec1, const Vecf& vec2)
{ return Vecf(_mm_sub_ps(vec1.Val(), vec2.Val())); }

static inline Vecf operator * (const Vecf& vec1, const Vecf& vec2)
{ return Vecf(_mm_mul_ps(vec1.Val(), vec2.Val())); }

static inline Vecf operator / (const Vecf& vec1, const Vecf& vec2)
{ return Vecf(_mm_div_ps(vec1.Val(), vec2.Val())); }

#else

void Vecf::load(const float* arr) { val = *arr; }
void Vecf::store(float* arr) { *arr = val; }
int Vecf::size() { return 1; }

static inline Vecf operator + (const Vecf& vec1, const Vecf& vec2) 
{ return Vecf(vec1.Val() + vec2.Val()); }

static inline Vecf operator - (const Vecf& vec1, const Vecf& vec2)
{ return Vecf(vec1.Val() - vec2.Val()); }

static inline Vecf operator * (const Vecf& vec1, const Vecf& vec2)
{ return Vecf(vec1.Val() * vec2.Val()); }

static inline Vecf operator / (const Vecf& vec1, const Vecf& vec2)
{ return Vecf(vec1.Val() / vec2.Val()); }

#endif

#endif