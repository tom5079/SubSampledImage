package xyz.quaver.graphics.subsampledimage

import androidx.compose.ui.graphics.ImageBitmapConfig
import androidx.compose.ui.graphics.colorspace.ColorSpaces

class VipsImageImpl internal constructor(
    source: ImageSource
) : VipsImage {

    private var _vipsImage: VipsImagePtr? = null
    override val vipsImage: VipsImagePtr
        get() = _vipsImage ?: error("tried to access closed VipsImage")

    override val colorSpace = ColorSpaces.Srgb
    override val config = ImageBitmapConfig.Argb8888

    override val hasAlpha: Boolean
        get() = hasAlpha(vipsImage)
    override val height: Int
        get() = getHeight(vipsImage)
    override val width: Int
        get() = getWidth(vipsImage)

    init {
        System.loadLibrary("ssi")
        _vipsImage = load(source.vipsSource) ?: error("error decoding file")
    }

    override fun prepareToDraw() = Unit

    external override fun readPixels(
        buffer: IntArray,
        startX: Int,
        startY: Int,
        width: Int,
        height: Int,
        bufferOffset: Int,
        stride: Int
    )

    private external fun hasAlpha(image: VipsImagePtr): Boolean
    private external fun getHeight(image: VipsImagePtr): Int
    private external fun getWidth(image: VipsImagePtr): Int

    private external fun load(image: VipsImagePtr): VipsImagePtr?
    external override fun close()
}