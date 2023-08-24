#include "ssi.h"

int ssi_test(const char *file) {
    VipsImage *in;
    int width;

    if (VIPS_INIT("ssi_test")) {
        vips_error_exit(NULL);
    }

    if (!(in = vips_image_new_from_file(file))) {
        vips_error_exit(NULL);
    }

    width = vips_image_get_width(in);

    g_object_unref(in);

    return width;
}