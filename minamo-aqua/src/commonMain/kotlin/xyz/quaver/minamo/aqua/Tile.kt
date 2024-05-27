package xyz.quaver.minamo.aqua

import xyz.quaver.minamo.*
import kotlin.math.min

class Tile(
    val image: MinamoImage,
    val region: MinamoRect,
    val level: Int,
    var tile: MinamoNativeImage? = null
) {
    fun load() {
        if (tile != null) return
        tile = image.decode(region.scale(1 / (1 shl level).toFloat(), origin = MinamoIntOffset.Zero))
    }

    fun unload() {
        tile = null
    }
}

class TileCache {
    var image: MinamoImage? = null
        set(value) {
            if (field === value) return

            field = value
            tiles.clear()
            level = 0
        }
    var level = -1
        set(value) {
            val sanitized = value.coerceAtLeast(0)
            if (field == sanitized) return
            field = sanitized

            resizedImage?.close()
            resizedImage = if (level > 0) image?.resize(1 / (1 shl value).toFloat()) else null
            populateTiles()
        }

    private var resizedImage: MinamoImage? = null

    private val tileSize = 8

    val tiles = mutableListOf<Tile>()

    val tileCount: Pair<Int, Int>
        get() {
            val width = image?.width ?: 0
            val height = image?.height ?: 0

            return (width + (1 shl (level + tileSize)) - 1).ushr(level + tileSize) to (height + (1 shl (level + tileSize)) - 1).ushr(
                level + tileSize
            )
        }

    private fun populateTiles() {
        val (tileCountX, tileCountY) = tileCount
        val image = image ?: return

        tiles.clear()

        for (y in 0 until tileCountY) {
            for (x in 0 until tileCountX) {

                val imageRegion = MinamoRect(
                    x shl (level + tileSize),
                    y shl (level + tileSize),
                    min(1 shl (level + tileSize), image.width - (x shl (level + tileSize))),
                    min(1 shl (level + tileSize), image.height - (y shl (level + tileSize)))
                )

                tiles.add(Tile(resizedImage ?: image, imageRegion, level, null))
            }
        }
    }
}