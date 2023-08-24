package xyz.quaver.graphics.subsampledimage

import androidx.compose.ui.graphics.ImageBitmap

typealias VipsImagePtr = Long

interface Image : ImageBitmap, AutoCloseable

internal interface VipsImage : Image {
    val vipsImage: VipsImagePtr
}