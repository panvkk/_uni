package org.example

import java.awt.List
import java.io.File
import java.io.PrintWriter
import java.util.Scanner
import java.util.Stack

fun main() {
    val scanner = Scanner(File("input.txt"))
    val writer = PrintWriter(File("output.txt"))

    val n = scanner.nextLine().toInt()
    val stack = Stack<Int>()
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
            stack.push(k)
            while(stack.isNotEmpty()) {
                val curr = stack.pop()
                if(marks[curr] == 0) {
                    marks[curr] = counter++
                    for(i in n - 1 downTo 0) {
                        if(adjacencyMatrix[curr][i] == 1 && marks[i] == 0) {
                            stack.push(i)
                        }
                    }
                }
            }
        }
    }

    val answer = marks.joinToString(" ")
    scanner.close()
    writer.write(answer + "\n")
    writer.close()
}