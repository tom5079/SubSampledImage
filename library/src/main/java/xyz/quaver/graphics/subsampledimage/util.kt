/*
 * Copyright 2021 tom5079
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package xyz.quaver.graphics.subsampledimage

import androidx.compose.ui.geometry.Offset
import androidx.compose.ui.geometry.Size
import androidx.compose.ui.unit.IntOffset
import androidx.compose.ui.unit.IntSize
import kotlin.math.min

fun calculateSampleSize(scale: Float): Int {
    var sampleSize = 1

    while (scale <= 1f / (sampleSize * 2)) sampleSize *= 2

    return sampleSize
}

fun getMaxSampleSize(canvasSize: Size, imageSize: Size): Int {
    val minScale =
        min(canvasSize.width / imageSize.width, canvasSize.height / imageSize.height)
    return calculateSampleSize(minScale)
}

fun Offset.toIntOffset() = IntOffset(this.x.toInt(), this.y.toInt())
fun Size.toIntSize() = IntSize(this.width.toInt(), this.height.toInt())