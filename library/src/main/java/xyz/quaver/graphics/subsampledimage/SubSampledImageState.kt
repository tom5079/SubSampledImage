package xyz.quaver.graphics.subsampledimage

import androidx.compose.animation.core.*
import androidx.compose.runtime.*
import androidx.compose.ui.geometry.Offset
import androidx.compose.ui.geometry.Rect
import androidx.compose.ui.geometry.Size
import kotlinx.coroutines.*

@Composable
fun rememberSubSampledImageState(scaleType: ScaleType = ScaleTypes.CENTER_INSIDE, bound: Bound = Bounds.FORCE_OVERLAP_OR_CENTER) = remember {
    SubSampledImageState(scaleType, bound)
}

class SubSampledImageState(var scaleType: ScaleType, var bound: Bound) {
    var canvasSize by mutableStateOf<Size?>(null)
        internal set

    var imageSize by mutableStateOf<Size?>(null)
        internal set

    /**
     * Represents the area the image will occupy in canvas's coordinate
     */
    var imageRect by mutableStateOf<Rect?>(null)
        private set

    fun setImageRectWithBound(rect: Rect) {
        zoomAnimationJob?.cancel()

        canvasSize?.let { canvasSize ->
            imageRect = bound(rect, canvasSize)
        }
    }

    private var zoomAnimationJob: Job? = null
    /**
     * Enlarge [imageRect] by [amount] centered around [centroid]
     *
     * For example, [amount] 0.2 inflates [imageRect] by 20%
     *              [amount] -0.2 deflates [imageRect] by 20%
     */
    suspend fun zoom(amount: Float, centroid: Offset, isAnimated: Boolean = false) = coroutineScope {
        zoomAnimationJob?.cancelAndJoin()

        zoomAnimationJob = launch {
            imageRect?.let { imageRect ->
                val animationSpec: AnimationSpec<Float> = if (isAnimated) spring() else snap()

                val anim = AnimationState(
                    initialValue = 0f,
                    initialVelocity = 0f
                )

                anim.animateTo(
                    targetValue = amount,
                    animationSpec = animationSpec,
                    sequentialAnimation = false
                ) {
                    if (!this@launch.isActive) cancelAnimation()

                    this@SubSampledImageState.imageRect = Rect(
                        imageRect.left + (imageRect.left - centroid.x) * value,
                        imageRect.top + (imageRect.top - centroid.y) * value,
                        imageRect.right + (imageRect.right - centroid.x) * value,
                        imageRect.bottom + (imageRect.bottom - centroid.y) * value
                    )
                }
            }
        }
    }

    fun resetImageRect() {
        imageSize?.let { imageSize ->
        canvasSize?.let { canvasSize ->
            setImageRectWithBound(scaleType.invoke(canvasSize, imageSize))
        } }
    }
}