#include <stdio.h>

#include "mkl_vsl.h"

#define SEED    114514
#define BRNG    VSL_BRNG_MT19937
#define METHOD  VSL_RNG_METHOD_UNIFORM_STD
#define N       2048

int main() {
    // init
    float r[N];
    VSLStreamStatePtr stream;
    int i = 0, j = 0;
    float a = 0.0, b = 1.0;
    FILE *fp = fopen("rand.txt", "w");

    vslNewStream(&stream, BRNG, SEED);

    // generate random numbers and write to file
    for (i = 0; i < N; ++i) {
        for (j = 0; j < N; ++j) {
            vsRngUniform(METHOD, stream, N, r, a, b);
            fprintf(fp, "%f\n", r[j]);
        }
    }

    // cleanup
    fclose(fp);
    vslDeleteStream(&stream);

    return 0;
}
