import xyz.quaver.graphics.subsampledimage.FileImageSource
import xyz.quaver.graphics.subsampledimage.loadImageFromFile
import kotlin.test.Test
import kotlin.test.assertEquals
import kotlin.test.assertFails

class ImageDecoderTest {

    val imagePath = javaClass.classLoader.getResource("image.jpg")!!.path

    @Test
    fun test() {
        val source = FileImageSource(imagePath)

        source.use {

        }

        assertFails {
            source.vipsSource
        }
    }

    @Test
    fun `test load image`() {
        val image = loadImageFromFile(imagePath)

        val imageBitmap = image.use {
            image.readRegion(width = 500, height = 500)
        }

        val buffer = IntArray(500 * 500)
        imageBitmap.readPixels(buffer, width = 500, height = 500)

        assertEquals(15321002, buffer[0])
        assertEquals(0xeac7aa, buffer[500])
        assertEquals(15256492, buffer[25 * 500 + 25])
        assertEquals(15912111, buffer[234 * 500 + 57])
    }
}