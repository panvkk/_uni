package org.example

import java.awt.List
import java.io.File
import java.io.PrintWriter
import java.util.Scanner

fun main() {
    val scanner = Scanner(File("input.txt"))
    val writer = PrintWriter(File("output.txt"))

    val n = scanner.nextLine().toInt()
    val queue = ArrayDeque<Int>()
    val marks = IntArray(n) { 0 }
    val adjacencyMatrix = Array(n) { IntArray(n) }

    for(i in 0 until n) {
        val line = scanner.nextLine()
        val row = line.split(" ").map { it.toInt() }
        adjacencyMatrix[i] = row.toIntArray()
    }

    var counter = 1
    for(k in 0 until n) {
        if(marks[k] == 0) {
            queue.add(k)
            while (queue.isNotEmpty()) {
                val current = queue.removeFirst()
                marks[current] = counter
                for(i in 0 until n) {
                    if(adjacencyMatrix[current][i] == 1 && marks[i] == 0 && !queue.contains(i)) {
                        queue.add(i)
                    }
                }
                counter++
            }
        }
    }

    val answer = marks.joinToString(" ")
    scanner.close()
    writer.write(answer + "\n")
    writer.close()
}