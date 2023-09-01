#include <stdio.h>
#include <math.h>

#define N 2048

int main() {
    // init
    FILE *fp_mkl = fopen("result_fftw3_onemkl.txt", "r");
    FILE *fp_orig = fopen("result_fftw3_original.txt", "r");
    int i = 0;
    float mkl_real = 0.0, mkl_imag = 0.0, orig_real = 0.0, orig_imag = 0.0;
    double real_diff = 0.0, imag_diff = 0.0;

    // read 2d real-to-complex dft results, and calculate average diff
    for (i = 0; i < N * (N / 2 + 1); ++i) {
        fscanf(fp_mkl, "%f %f\n", &mkl_real, &mkl_imag);
        fscanf(fp_orig, "%f %f\n", &orig_real, &orig_imag);
        real_diff += fabs(mkl_real - orig_real);
        imag_diff += fabs(mkl_imag - orig_imag);
    }
    real_diff /= N * (N / 2 + 1);
    imag_diff /= N * (N / 2 + 1);
    printf("average real part diff = %lf\n", real_diff);
    printf("average imaginary part diff = %lf\n", imag_diff);
    if (real_diff < 0.0001 && imag_diff < 0.0001) {
        printf("congrats! the result is correct!\n");
    }
    else {
        printf("oops! the result might be incorrect!\n");
    }

    // cleanup
    fclose(fp_mkl);
    fclose(fp_orig);

    return 0;
}