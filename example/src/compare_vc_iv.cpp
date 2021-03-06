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

static const char* strLine = "-------------------------------------------------------------------------------";
static const int OMP_NUM_THREADS = 32;
static const unsigned long LEN = OMP_NUM_THREADS * 1000 * 1000;

void fillf(float* p, const float& val);

void v_kernel();

void v_addf(float* a, float* b, float* c1, float* c2);
void v_subf(float* a, float* b, float* c1, float* c2);
void v_mulf(float* a, float* b, float* c1, float* c2);
void v_divf(float* a, float* b, float* c1, float* c2);

void v_poly1f(float* a, const float& scalar, float* c1, float* c2);
void v_poly2f(float* a, const float& scalar, float* c1, float* c2);
void v_poly3f(float* a, const float& scalar, float* c1, float* c2);

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
    cout << "Number of omp threads: " << OMP_NUM_THREADS << endl;
    cout << "Problem size: " << LEN << " float" << endl;
    cout << "Vector extension instruction set: ";
#if defined(__AVX512F__)
    cout << "AVX512F";
#elif defined(__AVX__)
    cout << "AVX";
#elif defined(__SSE__)
    cout << "SSE";
#else
    cout << "NONE";
#endif
    cout << endl;

    float *a, *b, *c1, *c2;

    a = new float[LEN];
    b = new float[LEN];
    c1 = new float[LEN];
    c2 = new float[LEN];

    cout << strLine << endl;
    cout << "this is warm-up computation, the result may be inaccurate!" << endl;
    v_addf(a, b, c1, c2);
    cout << strLine << endl;

    cout << strLine << endl;
    v_addf(a, b, c1, c2);
    cout << strLine << endl;

    cout << strLine << endl;
    v_subf(a, b, c1, c2);
    cout << strLine << endl;

    cout << strLine << endl;
    v_mulf(a, b, c1, c2);
    cout << strLine << endl;

    cout << strLine << endl;
    v_divf(a, b, c1, c2);
    cout << strLine << endl;

    cout << strLine << endl;
    cout << "this is warm-up computation, the result may be inaccurate!" << endl;
    v_poly2f(a, 1.0, c1, c2);
    cout << strLine << endl;

    cout << strLine << endl;
    v_poly2f(a, 1.0, c1, c2);
    cout << strLine << endl;

    cout << strLine << endl;
    v_poly3f(a, 1.0, c1, c2);
    cout << strLine << endl;

    if (c2 != NULL) delete c2;
    if (c1 != NULL) delete c1;
    if (b != NULL) delete b;
    if (a != NULL) delete a;
}

void v_addf(float* a, float* b, float* c1, float* c2)
{
    cout << "vector add with single precison:" << endl;
    if (a != NULL && b != NULL && c1 != NULL && c2 != NULL)
    {
        fillf(a, 2.0);
        fillf(b, 3.0);
        Timer timer;
        double elapsed_vc, elapsed_iv;
        const unsigned long tmplen = LEN - LEN % 16;

        /* vector class */
        Vec16f vc_a, vc_b;
        vc_a = vc_b + 1.0;

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

        elapsed_vc = timer.end_us();
        printf("vector class elapsed %.3f us\n", elapsed_vc);

        /* intrinsics vector */
        Vecf iv_a, iv_b;
        const int step = Vecf::size();

        timer.start();

        #pragma omp parallel for num_threads(OMP_NUM_THREADS) private(iv_a, iv_b)
        for (unsigned long i = 0; i < tmplen; i+=step)
        {
            iv_a.load(a + i);
            iv_b.load(b + i);
            iv_a = iv_a + iv_b;
            iv_a.store(c2 + i);
        }

        for (unsigned long i = tmplen; i < LEN; i++)
            c2[i] = a[i] + b[i];

        elapsed_iv = timer.end_us();
        printf("intrinsics vector elapsed %.3f us\n", elapsed_iv);
        printf("speedup of ic/vc: %.3f\n", elapsed_vc / elapsed_iv);
    }
    else
        cerr << "error: NULL pointer error!" << endl;
}

void v_subf(float* a, float* b, float* c1, float* c2)
{
    cout << "vector sub with single precison:" << endl;
    if (a != NULL && b != NULL && c1 != NULL && c2 != NULL)
    {
        fillf(a, 5.0);
        fillf(b, 3.0);
        Timer timer;
        double elapsed_vc, elapsed_iv;
        const unsigned long tmplen = LEN - LEN % 16;

        /* vector class */
        Vec16f vc_a, vc_b;

        timer.start();

        #pragma omp parallel for num_threads(OMP_NUM_THREADS) private(vc_a, vc_b)
        for (unsigned long i = 0; i < tmplen; i+=16)
        {
            vc_a.load(a + i);
            vc_b.load(b + i);
            vc_a = vc_a - vc_b;
            vc_a.store(c1 + i);
        }

        for (unsigned long i = tmplen; i < LEN; i++)
            c1[i] = a[i] - b[i];

        elapsed_vc = timer.end_us();
        printf("vector class elapsed %.3f us\n", elapsed_vc);

        /* intrinsics vector */
        Vecf iv_a, iv_b;
        const int step = Vecf::size();

        timer.start();

        #pragma omp parallel for num_threads(OMP_NUM_THREADS) private(iv_a, iv_b)
        for (unsigned long i = 0; i < tmplen; i+=step)
        {
            iv_a.load(a + i);
            iv_b.load(b + i);
            iv_a = iv_a - iv_b;
            iv_a.store(c2 + i);
        }

        for (unsigned long i = tmplen; i < LEN; i++)
            c2[i] = a[i] - b[i];

        elapsed_iv = timer.end_us();
        printf("intrinsics vector elapsed %.3f us\n", elapsed_iv);
        printf("speedup of ic/vc: %.3f\n", elapsed_vc / elapsed_iv);
    }
    else
        cerr << "error: NULL pointer error!" << endl;
}

void v_mulf(float* a, float* b, float* c1, float* c2)
{
    cout << "vector mul with single precison:" << endl;
    if (a != NULL && b != NULL && c1 != NULL && c2 != NULL)
    {
        fillf(a, 2.0);
        fillf(b, 3.0);
        Timer timer;
        double elapsed_vc, elapsed_iv;
        const unsigned long tmplen = LEN - LEN % 16;

        /* vector class */
        Vec16f vc_a, vc_b;

        timer.start();

        #pragma omp parallel for num_threads(OMP_NUM_THREADS) private(vc_a, vc_b)
        for (unsigned long i = 0; i < tmplen; i+=16)
        {
            vc_a.load(a + i);
            vc_b.load(b + i);
            vc_a = vc_a * vc_b;
            vc_a.store(c1 + i);
        }

        for (unsigned long i = tmplen; i < LEN; i++)
            c1[i] = a[i] * b[i];

        elapsed_vc = timer.end_us();
        printf("vector class elapsed %.3f us\n", elapsed_vc);

        /* intrinsics vector */
        Vecf iv_a, iv_b;
        const int step = Vecf::size();

        timer.start();

        #pragma omp parallel for num_threads(OMP_NUM_THREADS) private(iv_a, iv_b)
        for (unsigned long i = 0; i < tmplen; i+=step)
        {
            iv_a.load(a + i);
            iv_b.load(b + i);
            iv_a = iv_a * iv_b;
            iv_a.store(c2 + i);
        }

        for (unsigned long i = tmplen; i < LEN; i++)
            c2[i] = a[i] * b[i];

        elapsed_iv = timer.end_us();
        printf("intrinsics vector elapsed %.3f us\n", elapsed_iv);
        printf("speedup of ic/vc: %.3f\n", elapsed_vc / elapsed_iv);
    }
    else
        cerr << "error: NULL pointer error!" << endl;
}

void v_divf(float* a, float* b, float* c1, float* c2)
{
    cout << "vector div with single precison:" << endl;
    if (a != NULL && b != NULL && c1 != NULL && c2 != NULL)
    {
        fillf(a, 2.0);
        fillf(b, 3.0);
        Timer timer;
        double elapsed_vc, elapsed_iv;
        const unsigned long tmplen = LEN - LEN % 16;

        /* vector class */
        Vec16f vc_a, vc_b;

        timer.start();

        #pragma omp parallel for num_threads(OMP_NUM_THREADS) private(vc_a, vc_b)
        for (unsigned long i = 0; i < tmplen; i+=16)
        {
            vc_a.load(a + i);
            vc_b.load(b + i);
            vc_a = vc_a / vc_b;
            vc_a.store(c1 + i);
        }

        for (unsigned long i = tmplen; i < LEN; i++)
            c1[i] = a[i] / b[i];

        elapsed_vc = timer.end_us();
        printf("vector class elapsed %.3f us\n", elapsed_vc);

        /* intrinsics vector */
        Vecf iv_a, iv_b;
        const int step = Vecf::size();

        timer.start();

        #pragma omp parallel for num_threads(OMP_NUM_THREADS) private(iv_a, iv_b)
        for (unsigned long i = 0; i < tmplen; i+=step)
        {
            iv_a.load(a + i);
            iv_b.load(b + i);
            iv_a = iv_a / iv_b;
            iv_a.store(c2 + i);
        }

        for (unsigned long i = tmplen; i < LEN; i++)
            c2[i] = a[i] / b[i];

        elapsed_iv = timer.end_us();
        printf("intrinsics vector elapsed %.3f us\n", elapsed_iv);
        printf("speedup of ic/vc: %.3f\n", elapsed_vc / elapsed_iv);
    }
    else
        cerr << "error: NULL pointer error!" << endl;
}

void v_poly2f(float* a, const float& scalar, float* c1, float* c2)
{
    cout << "vector 2nd-order polynomial with single precison:" << endl;
    if (a != NULL && c1 != NULL && c2 != NULL)
    {
        fillf(a, 2.0);
        Timer timer;
        double elapsed_vc, elapsed_iv;
        const unsigned long tmplen = LEN - LEN % 16;

        /* vector class */
        Vec16f vc_a;

        timer.start();

        #pragma omp parallel for num_threads(OMP_NUM_THREADS) private(vc_a)
        for (unsigned long i = 0; i < tmplen; i+=16)
        {
            vc_a.load(a + i);
            vc_a = vc_a * vc_a + vc_a; // + scalar
            vc_a.store(c1 + i);
        }

        for (unsigned long i = tmplen; i < LEN; i++)
            c1[i] = a[i] * a[i] + a[i]; // + scalar

        elapsed_vc = timer.end_us();
        printf("vector class elapsed %.3f us\n", elapsed_vc);

        /* intrinsics vector */
        Vecf iv_a;
        const int step = Vecf::size();

        timer.start();

        #pragma omp parallel for num_threads(OMP_NUM_THREADS) private(iv_a)
        for (unsigned long i = 0; i < tmplen; i+=step)
        {
            iv_a.load(a + i);
            iv_a = iv_a * iv_a + iv_a; // + scalar
            iv_a.store(c2 + i);
        }

        for (unsigned long i = tmplen; i < LEN; i++)
            c2[i] = a[i] * a[i] + a[i]; // + scalar

        elapsed_iv = timer.end_us();
        printf("intrinsics vector elapsed %.3f us\n", elapsed_iv);
        printf("speedup of ic/vc: %.3f\n", elapsed_vc / elapsed_iv);
    }
    else
        cerr << "error: NULL pointer error!" << endl;
}

void v_poly3f(float* a, const float& scalar, float* c1, float* c2)
{
    cout << "vector 3rd-order polynomial with single precison:" << endl;
    if (a != NULL && c1 != NULL && c2 != NULL)
    {
        fillf(a, 2.0);
        Timer timer;
        double elapsed_vc, elapsed_iv;
        const unsigned long tmplen = LEN - LEN % 16;

        /* vector class */
        Vec16f vc_a;

        timer.start();

        #pragma omp parallel for num_threads(OMP_NUM_THREADS) private(vc_a)
        for (unsigned long i = 0; i < tmplen; i+=16)
        {
            vc_a.load(a + i);
            vc_a = vc_a * vc_a * vc_a + vc_a * vc_a + vc_a; // + scalar
            vc_a.store(c1 + i);
        }

        for (unsigned long i = tmplen; i < LEN; i++)
            c1[i] = a[i] * a[i] * a[i] + a[i] * a[i] + a[i]; // + scalar

        elapsed_vc = timer.end_us();
        printf("vector class elapsed %.3f us\n", elapsed_vc);

        /* intrinsics vector */
        Vecf iv_a;
        const int step = Vecf::size();

        timer.start();

        #pragma omp parallel for num_threads(OMP_NUM_THREADS) private(iv_a)
        for (unsigned long i = 0; i < tmplen; i+=step)
        {
            iv_a.load(a + i);
            iv_a = iv_a * iv_a * iv_a + iv_a * iv_a + iv_a; // + scalar
            iv_a.store(c2 + i);
        }

        for (unsigned long i = tmplen; i < LEN; i++)
            c2[i] = a[i] * a[i] * a[i] + a[i] * a[i] + a[i]; // + scalar

        elapsed_iv = timer.end_us();
        printf("intrinsics vector elapsed %.3f us\n", elapsed_iv);
        printf("speedup of ic/vc: %.3f\n", elapsed_vc / elapsed_iv);
    }
    else
        cerr << "error: NULL pointer error!" << endl;
}