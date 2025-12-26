import java.io.File
import java.io.PrintWriter

fun main() {
    val br = File("input.txt").bufferedReader()
    val writer = PrintWriter(File("output.txt"))

    val line = br.readLine()
    val n = line.length

    val dp = Array(n) { IntArray(n) }

    for (i in n - 1 downTo 0) {
        dp[i][i] = 1
        for (j in i + 1 until n) {
            if (line[i] == line[j]) {
                dp[i][j] = (dp[i + 1][j - 1] + 2)
            } else {
                dp[i][j] = maxOf(dp[i + 1][j], dp[i][j - 1])
            }
        }
    }

    val maxLength = dp[0][n - 1]
    writer.println(maxLength)

    val result = CharArray(maxLength)
    var left = 0
    var right = n - 1
    var resLeft = 0
    var resRight = maxLength - 1

    while (left <= right) {
        if (line[left] == line[right]) {
            result[resLeft] = line[left]
            result[resRight] = line[right]
            left++
            right--
            resLeft++
            resRight--
        } else if (dp[left + 1][right] >= dp[left][right - 1]) {
            left++
        } else {
            right--
        }
    }

    writer.println(String(result))

    br.close()
    writer.close()
}