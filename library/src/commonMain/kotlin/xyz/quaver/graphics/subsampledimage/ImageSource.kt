package xyz.quaver.graphics.subsampledimage

typealias VipsSourcePtr = Long

internal interface ImageSource : AutoCloseable {
    val vipsSource: VipsSourcePtr
}