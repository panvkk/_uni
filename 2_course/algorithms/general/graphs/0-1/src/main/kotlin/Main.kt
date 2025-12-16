package org.example

import java.io.File
import java.io.PrintWriter
import java.util.Scanner

fun main() {
    val scanner = Scanner(File("input.txt"))
    val writer = PrintWriter(File("output.txt"))

    val n = scanner.nextInt()
    val m = scanner.nextInt()

    val matrix = Array(n) { IntArray(n) { 0 } }

    for(i in 0 until m) {
        val u = scanner.nextInt() - 1
        val v = scanner.nextInt() - 1

        matrix[u][v] = 1
        matrix[v][u] = 1
    }
    scanner.close()
    matrix.forEach {
        val row = it
        val rowString = row.joinToString(" ")
        writer.println(rowString)
    }
    writer.close()
}