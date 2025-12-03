import java.io.BufferedReader
import java.io.BufferedWriter
import java.io.FileReader
import java.io.FileWriter
import java.util.StringTokenizer

fun main() {
    val reader = BufferedReader(FileReader("input.txt"))
    val writer = BufferedWriter(FileWriter("output.txt"))
    var st: StringTokenizer? = null

    val n = reader.readLine().toInt()

    fun nextPair() : Pair<Int, Int> {
        while(st == null || !st!!.hasMoreTokens()) {
            st = StringTokenizer(reader.readLine())
        }
        val token1 = st.nextToken().toInt()
        val token2 = st.nextToken().toInt()
        return Pair(token1, token2)
    }

    val arr = Array<Pair<Int, Int>>(n) { nextPair() }


}

fun operations(matrix1: Pair<Int, Int>, matrix2: Pair<Int, Int>): Int {
    return matrix1.first * matrix1.second * matrix2.second
}