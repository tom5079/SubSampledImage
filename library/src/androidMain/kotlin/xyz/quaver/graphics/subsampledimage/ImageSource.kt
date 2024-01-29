package xyz.quaver.graphics.subsampledimage

import android.content.Context
import android.content.ContextWrapper
import android.net.Uri

class LocalUriImageSource(
    context: Context,
    uri: Uri
) : ImageSource, ContextWrapper(context) {
    private var _vipsSource: VipsSourcePtr? = null

    override val vipsSource: VipsSourcePtr
        get() = _vipsSource ?: error("tried to access closed VipsSource")

    private val descriptor = runCatching {
        contentResolver.openFileDescriptor(uri, "r")
    }.getOrNull() ?: error("failed to open image $uri")

    init {
        System.loadLibrary("ssi")
        _vipsSource = load(descriptor.fd) ?: error("failed to open image $uri")
    }

    private external fun load(descriptor: Int): VipsSourcePtr?
    private external fun closeSource()

    override fun close() {
        closeSource()
        descriptor.close()
    }
}