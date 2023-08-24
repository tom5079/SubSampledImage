package xyz.quaver.graphics.subsampledimage

class FileImageSource(
    file: String
) : ImageSource {

    private var _vipsSource: VipsSourcePtr? = null
    override val vipsSource: VipsSourcePtr
        get() = _vipsSource ?: error("tried to access closed VipsSource")

    init {
        System.loadLibrary("ssi")
        _vipsSource = load(file) ?: error("failed to open image $file")
    }

    private external fun load(file: String): VipsSourcePtr?

    external override fun close()
}