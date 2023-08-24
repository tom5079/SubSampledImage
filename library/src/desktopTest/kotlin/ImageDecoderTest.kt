import xyz.quaver.graphics.subsampledimage.FileImageSource
import xyz.quaver.graphics.subsampledimage.loadImageFromFile
import kotlin.test.Test
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

        image.use {
            val buffer = IntArray(100 * 100)

            image.readPixels(buffer, width = 100, height = 100)
        }
    }
}