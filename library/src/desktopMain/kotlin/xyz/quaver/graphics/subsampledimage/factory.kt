package xyz.quaver.graphics.subsampledimage

fun loadImageFromFile(file: String): SSIImage =
    FileImageSource(file).use { source ->
        VipsImageImpl(source)
    }