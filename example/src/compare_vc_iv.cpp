/*************************************************
 * @brief Performace comparison between 
 * vector class and intrinsics vector.
 * @extend_instruction_set AVX512F
 * @author Bin Qu
 * @email benquickdenn@foxmail.com
*************************************************/

#include "../vector_class/version2/vectorclass.h"
#include "intrinsics_vector.hpp"

#include <omp.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>

using namespace std;

static const int OMP_NUM_THREADS = 32;
static const unsigned long LEN = OMP_NUM_THREADS * 1000 * 1000;

void fillf(float* p, const float& val);

void v_kernel();

void v_addf(float* a, float* b, float* c1, float* c2);
void v_subf(float* a, float* b, float* c1, float* c2);
void v_mulf(float* a, float* b, float* c1, float* c2);
void v_divf(float* a, float* b, float* c1, float* c2);

void v_poly1f();
void v_poly2f();
void v_poly3f();

int main()
{
    v_kernel();
    return 0;
}

void fillf(float* p, const float& val)
{
    const unsigned long len = sizeof(p) * sizeof(float);
    #pragma omp parallel for
    for (unsigned long i = 0; i < len; i++)
        p[i] = val;
}

void v_kernel()
{
    float *a, *b, *c1, *c2;

    a = new float[LEN];
    b = new float[LEN];
    c1 = new float[LEN];
    c2 = new float[LEN];

    v_addf(a, b, c1, c2);

    if (c2 != NULL) delete c2;
    if (c1 != NULL) delete c1;
    if (b != NULL) delete b;
    if (a != NULL) delete a;
}

void v_addf(float* a, float* b, float* c1, float* c2)
{
    if (a != NULL && b != NULL && c1 != NULL && c2 != NULL)
    {
        fillf(a, 2.0);
        fillf(b, 3.0);
        Timer timer;
        const unsigned long tmplen = LEN - LEN % 16;

        /* vector class */
        Vec16f vc_a, vc_b;

        timer.start();

        #pragma omp parallel for num_threads(OMP_NUM_THREADS) private(vc_a, vc_b)
        for (unsigned long i = 0; i < tmplen; i+=16)
        {
            vc_a.load(a + i);
            vc_b.load(b + i);
            vc_a = vc_a + vc_b;
            vc_a.store(c1 + i);
        }

        for (unsigned long i = tmplen; i < LEN; i++)
            c1[i] = a[i] + b[i];

        printf("v_addf vector class elapsed %.3f us\n", timer.end_us());

        /* intrinsics vector */
        Vecf iv_a, iv_b;

        timer.start();

        #pragma omp parallel for num_threads(OMP_NUM_THREADS) private(iv_a, iv_b)
        for (unsigned long i = 0; i < tmplen; i+=16)
        {
            iv_a.load(a + i);
            iv_b.load(b + i);
            iv_a = iv_a + iv_b;
            iv_a.store(c2 + i);
        }

        for (unsigned long i = tmplen; i < LEN; i++)
            c2[i] = a[i] + b[i];

        printf("v_addf intrinsics vector elapsed %.3f us\n", timer.end_us());
    }
    else
        cerr << "v_addf error: NULL pointer error!" << endl;
}