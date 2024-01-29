package xyz.quaver.subsampledimage.android

import android.content.Intent
import android.os.Bundle
import androidx.activity.compose.setContent
import androidx.activity.result.contract.ActivityResultContracts
import androidx.appcompat.app.AppCompatActivity
import androidx.compose.foundation.Image
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.material.Button
import androidx.compose.material.MaterialTheme
import androidx.compose.material.Text
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.setValue
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.ImageBitmap
import xyz.quaver.graphics.subsampledimage.loadImageFromLocalUri

class MainActivity : AppCompatActivity() {

    private var image: ImageBitmap? by mutableStateOf(null)

    private val requestFileLauncher =
        registerForActivityResult(ActivityResultContracts.StartActivityForResult()) { result ->
            val uri = result.data?.data ?: return@registerForActivityResult

            loadImageFromLocalUri(this, uri).use { ssiImage ->
                image = ssiImage.readRegion()
            }
        }

    override fun onCreate(savedInstanceState: Bundle?) {
        val a = java.lang.Long.valueOf(3)
        super.onCreate(savedInstanceState)
        setContent {
            MaterialTheme {
                if (image == null) {
                    Button(
                        onClick = {
                            val intent = Intent(Intent.ACTION_OPEN_DOCUMENT).apply {
                                addCategory(Intent.CATEGORY_OPENABLE)
                                type = "image/*"
                            }

                            requestFileLauncher.launch(intent)
                        }
                    ) {
                        Text(
                            "Choose an image ${a}"
                        )
                    }
                } else {
                    Image(image!!, null, Modifier.fillMaxSize())
                }
            }
        }
    }
}