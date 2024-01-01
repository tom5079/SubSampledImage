package xyz.quaver.graphics.subsampledimage

import androidx.compose.ui.graphics.ImageBitmap
import org.jetbrains.skia.*

class VipsImageImpl internal constructor(
    source: ImageSource
) : VipsImage {

    private var _vipsImage: VipsImagePtr? = null
    override val vipsImage: VipsImagePtr
        get() = _vipsImage ?: error("tried to access closed VipsImage")

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

    override fun readRegion(
        startX: Int,
        startY: Int,
        width: Int,
        height: Int
    ): ImageBitmap {
        val bitmap = Bitmap()
        val alphaType = if (hasAlpha) ColorAlphaType.PREMUL else ColorAlphaType.OPAQUE
        val colorInfo = ColorInfo(ColorType.RGBA_8888, alphaType, ColorSpace.sRGB)
        val imageInfo = ImageInfo(colorInfo, width, height)
        bitmap.allocPixels(imageInfo)

        val buffer = ByteArray(width * height * 4)
        readPixels(buffer, startX, startY, width, height, 0, 1)

        bitmap.installPixels(buffer)

        val imageBitmapClass = Class.forName("androidx.compose.ui.graphics.SkiaBackedImageBitmap")
        val constructor = imageBitmapClass.getConstructor(Bitmap::class.java)
        constructor.isAccessible = true
        return constructor.newInstance(bitmap) as ImageBitmap
    }

    private external fun readPixels(
        buffer: ByteArray,
        startX: Int,
        startY: Int,
        width: Int,
        height: Int,
        bufferOffset: Int,
        stride: Int
    ): Int

    private external fun hasAlpha(image: VipsImagePtr): Boolean
    private external fun getHeight(image: VipsImagePtr): Int
    private external fun getWidth(image: VipsImagePtr): Int

    private external fun load(image: VipsImagePtr): VipsImagePtr?
    external override fun close()
}