import java.io.BufferedReader
import java.io.File
import java.io.FileReader

fun main() {
    BufferedReader(FileReader("input.txt")).use { reader ->
        val size: Int = reader.readText().toInt()
        
    }
}
