package org.example

import java.io.File
import java.io.PrintWriter
import java.util.Scanner

fun main() {
    val scanner = Scanner(File("input.txt"))
    val writer = PrintWriter(File("output.txt"))

    val n = scanner.nextInt()
    val m = scanner.nextInt()
    val adjacencyList = List(n + 1) { mutableListOf<Int>() }

    for (i in 1..m) {
        val a = scanner.nextInt()
        val b = scanner.nextInt()
        adjacencyList[a].add(b)
        adjacencyList[b].add(a)
    }

    scanner.close()

    for(i in 1 until adjacencyList.size) {
        val row = adjacencyList[i]
        val answer = "${row.size} " + row.joinToString(" ")
        writer.println(answer)
    }

    writer.close()
}