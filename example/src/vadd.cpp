#include "intrinsics_vector.hpp"

#include <cstdio>

const int LEN = 16;

float C[LEN], A[LEN], B[LEN];

int main()
{
    using namespace std;

    /* initialization */
    for (int i = 0; i < LEN; i++)
    {
        A[i] = 2.0;
        B[i] = 3.0;
    }

    Vecf va, vb;
    va.load(A);
    vb.load(B);

    va = va + vb * vb  / va;
    va.store(C);

    for (int i = 0; i < LEN - 1; i++)
        printf("%.1f\t", C[i]);
    printf("%.1f\n", C[LEN - 1]);
}