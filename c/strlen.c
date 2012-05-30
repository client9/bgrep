#include <stdio.h>
#include <string.h>

int stream_strlen(FILE* f)
{
    char buf[1024];
    while (fgets(buf, 1024, f)) {
        printf("%lu\n", strlen(buf));
    }
    return 0;
}
int main(int argc, char **argv)
{
    stream_strlen(stdin);
    return 0;
}
