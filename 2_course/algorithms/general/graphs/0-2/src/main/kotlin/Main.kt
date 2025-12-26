package org.example

import java.io.File
import java.io.PrintWriter
import java.util.Scanner

fun main() {
    val scanner = Scanner(File("input.txt"))
    val writer = PrintWriter(File("output.txt"))

    val n = scanner.nextInt()
    val graph = IntArray(n)

    for (i in 1 until n) {
        val a = scanner.nextInt()
        val b = scanner.nextInt()
        graph[b - 1] = a
    }

    scanner.close()
    val answer = graph.joinToString(separator = " ")
    writer.write(answer)
    writer.close()
}