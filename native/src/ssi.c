#include "ssi.h"

int ssi_test() {
    if (vips_init("ssi_test")) {
        vips_error_exit(NULL);
    }

    return 0;
}