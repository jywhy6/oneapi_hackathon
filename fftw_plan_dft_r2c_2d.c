#include <stdio.h>
#include <time.h>
#include "fftw3.h"

#define N 2048
#define LOOP 1000

int main() {
    // init
    float *input = fftwf_alloc_real(N * N);
    fftwf_complex *output = fftwf_alloc_complex(N * (N / 2 + 1));
    FILE *fp_read = fopen("rand.txt", "r");
    clock_t t = 0;
    double time_s = 0.0;

// use macro defined by cmake to differentiate output
#if USE_ONEMKL
    printf("using oneMKL provided fftw3 API\n");
    FILE *fp_write = fopen("result_fftw3_onemkl.txt", "w");
#else
    printf("using original fftw3 API\n");
    FILE *fp_write = fopen("result_fftw3_original.txt", "w");
#endif

    int i = 0;
    fftwf_plan r2c = fftwf_plan_dft_r2c_2d(N, N, input, output, FFTW_MEASURE);

    // read generated random numbers
    for (i = 0; i < N * N; ++i) {
        fscanf(fp_read, "%f\n", &input[i]);
    }

    // execute 2d real-to-complex dft loop, and calculate average time
    t = clock();
    for (i = 0; i < LOOP; ++i) {
        fftwf_execute(r2c);
    }
    t = clock() - t;
    time_s = (double)t / CLOCKS_PER_SEC / LOOP;
    printf("average time used: %lf seconds", time_s);

    // write output to file
    for (i = 0; i < N * (N / 2 + 1); ++i) {
        fprintf(fp_write, "%f %f\n", output[i][0], output[i][1]);
    }

    // cleanup
    fftwf_free(input);
    fftwf_free(output);
    fftwf_destroy_plan(r2c);
    fclose(fp_read);
    fclose(fp_write);

    return 0;
}