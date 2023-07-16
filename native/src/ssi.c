#include "ssi.h"

int test() {
    if (vips_init(NULL)) {
        vips_error_exit(NULL);
    }

    return 0;
}