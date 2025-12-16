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
        val tokenizer = st!!

        val token1 = tokenizer.nextToken().toInt()
        val token2 = tokenizer.nextToken().toInt()
        return Pair(token1, token2)
    }

    val m = Array(n) { nextPair() }
    val dp = Array(n) { Array(n, { Long.MAX_VALUE }) }

    for(i in 0..n - 1) {
        dp[i][i] = 0L
    }

    for(l in 2..n) {
        for(i in 0..n - l) {
            val j: Int = i + l - 1
            for(k in i..j - 1) {
                val current = dp[i][k] + dp[k+1][j] + m[i].first * m[k].second * m[j].second
                if(current < dp[i][j]) {
                    dp[i][j] = current
                }
            }
        }
    }
    writer.write(dp[0][n-1].toString())
    writer.close()
    reader.close()
}