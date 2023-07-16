import xyz.quaver.graphics.subsampledimage.ImageDecoder
import kotlin.test.Test
import kotlin.test.assertEquals

class ImageDecoderTest {

    @Test
    fun test() {
        val decoder = ImageDecoder()

        assertEquals(2124, decoder.test())
    }
}