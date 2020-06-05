/*************************************************
 * @brief Performace comparison between 
 * vector class and intrinsics vector.
 * @author Bin Qu
 * @email benquickdenn@foxmail.com
*************************************************/

#include "../vector_class/version2/vectorclass.h"
#include "intrinsics_vector.hpp"

#include <omp.h>
#include <cstdlib>
#include <cstdio>

using namespace std;

static const int OMP_NUM_THREADS = 32;
static const unsigned long LEN = OMP_NUM_THREADS * 1000 * 1000;

void fillf(float* p, const float& val);

void v_addf();
void v_subf();
void v_mulf();
void v_divf();

void v_poly1f();
void v_poly2f();
void v_poly3f();

int main()
{
    return 0;
}

void fillf(float* p, const float& val)
{
    const unsigned long len = sizeof(p) * sizeof(float);
    #pragma omp parallel for
    for (unsigned long i = 0; i < len; i++)
        p[i] = val;
}

void v_addf()
{
    float *a, *b, *c1, *c2;
    a = new float[LEN];
    b = new float[LEN];
    c1 = new float[LEN];
    c2 = new float[LEN];
    if (a != NULL && b != NULL || c1 != NULL || c2 != NULL)
    {
        fillf(a, 2.0);
        fillf(b, 3.0);
        Timer timer;
        /* vector class */
    }
    if (c2 != NULL) delete c2;
    if (c1 != NULL) delete c1;
    if (b != NULL) delete b;
    if (a != NULL) delete a;
}