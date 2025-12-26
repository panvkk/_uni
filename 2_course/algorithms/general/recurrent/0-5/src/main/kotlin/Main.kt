import java.io.PrintWriter
import java.util.Scanner

fun main() {
    val sc = Scanner(System.`in`)
    val writer = PrintWriter(System.out)

    val n = sc.nextInt()
    val A = IntArray(n)
    for (i in 0 until n) A[i] = sc.nextInt()

    val B = IntArray(n)
    for (i in 0 until n) B[i] = sc.nextInt()

    val dp = Array(n + 1) { IntArray(n + 1) }


    for (i in 1..n) {
        for (j in 1..n) {
            if (A[i - 1] == B[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1
            } else {
                dp[i][j] = maxOf(dp[i - 1][j], dp[i][j - 1])
            }
        }
    }

    val k = dp[n][n]
    writer.write(k.toString() + "\n")

    val indicesA = mutableListOf<Int>()
    val indicesB = mutableListOf<Int>()

    var i = n
    var j = n
    while (i > 0 && j > 0) {
        if (A[i - 1] == B[j - 1]) {
            indicesA.add(i - 1)
            indicesB.add(j - 1)
            i--
            j--
        } else if (dp[i - 1][j] >= dp[i][j - 1]) {
            i--
        } else {
            j--
        }
    }

    writer.write(indicesA.reversed().joinToString(" ") + "\n")
    writer.write(indicesB.reversed().joinToString(" "))
    writer.close()
}