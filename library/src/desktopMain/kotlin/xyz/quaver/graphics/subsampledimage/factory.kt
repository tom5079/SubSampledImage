package xyz.quaver.graphics.subsampledimage

fun loadImageFromFile(file: String): Image =
    FileImageSource(file).use { source ->
        VipsImageImpl(source)
    }