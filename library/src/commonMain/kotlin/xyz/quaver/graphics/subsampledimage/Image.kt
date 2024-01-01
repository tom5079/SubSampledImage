package xyz.quaver.graphics.subsampledimage

import androidx.compose.ui.graphics.ImageBitmap

typealias VipsImagePtr = Long

interface SSIImage : AutoCloseable {
    val hasAlpha: Boolean
    val height: Int
    val width: Int

    fun readRegion(
        startX: Int = 0,
        startY: Int = 0,
        width: Int = this.width,
        height: Int = this.height,
    ): ImageBitmap
}

internal interface VipsImage : SSIImage {
    val vipsImage: VipsImagePtr
}