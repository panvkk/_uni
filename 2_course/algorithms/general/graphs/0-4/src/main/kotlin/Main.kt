package org.example

import java.io.File
import java.io.PrintWriter
import java.util.Scanner

fun main() {
    val scanner = Scanner(File("input.txt"))
    val writer = PrintWriter(File("output.txt"))

    val n = scanner.nextLine().toInt()
    val canonicalForm = IntArray(n)

    for (i in 0 until n) {
        val line = scanner.nextLine()
        val row = line.split(' ').map { it.toInt() }
        for(k in 0 until n) {
            if(row[k] == 1) {
                canonicalForm[k] = i + 1
            }
        }
    }
    scanner.close()

    val answer = canonicalForm.joinToString(" ")
    writer.write(answer)
    writer.close()
}