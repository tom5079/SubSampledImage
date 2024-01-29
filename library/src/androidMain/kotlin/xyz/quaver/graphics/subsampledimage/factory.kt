package xyz.quaver.graphics.subsampledimage

import android.content.Context
import android.net.Uri

fun loadImageFromLocalUri(context: Context, uri: Uri): SSIImage =
    LocalUriImageSource(context, uri).use {
        VipsImageImpl(it)
    }