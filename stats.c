#include <stdio.h>
#include <string.h>

int stream_stats(FILE* f)
{
    double fmin = 0;
    double fmax = 0;
    double current = 0;
    int ok = fscanf(f, "%lf", &current);
    if (ok == EOF) {
        printf("0 0\n");
        return 1;
    }
    fmin = current;
    fmax = current;

    while (1) {
        int ok = fscanf(f, "%lf", &current);
        if (ok == EOF) break;
        if (current < fmin) {
            fmin = current;
        } else if (current > fmax) {
            fmax = current;
        }
    }
    printf("%lf %lf\n", fmin, fmax);
    return 0;
}

int main(int argc, char **argv)
{
    stream_stats(stdin);
    return 0;
}
