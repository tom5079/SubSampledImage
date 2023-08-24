#include <ssi.h>
#include <stdio.h>

int main(int argc, char **argv) {
    int width;

    if (argc != 2) {
        return 1;
    }

    width = ssi_test(argv[1]);

    if (width != 2304) {
        return 1;
    }

    return 0;
}