package org.example

import java.io.File
import java.io.PrintWriter
import java.util.*

fun main() {
    val br = File("input.txt").bufferedReader()
    var st = StringTokenizer(br.readLine())
    val writer = PrintWriter(File("output.txt"))

    val n = st.nextToken().toInt()

    val arr = IntArray(n)
    st = StringTokenizer(br.readLine())
    for (i in 0 until n) {
        arr[i] = st.nextToken().toInt()
    }

    val tails = IntArray(n)
    var size = 0

    for (x in arr) {
        var i = 0
        var j = size
        while (i != j) {
            val m = (i + j) / 2
            if (tails[m] < x) {
                i = m + 1
            } else {
                j = m
            }
        }

        tails[i] = x
        if (i == size) size++
    }

    writer.println(size)
    br.close()
    writer.close()
}