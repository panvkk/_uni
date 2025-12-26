package org.example;

import java.io.File
import java.io.PrintWriter
import java.util.*

fun main() {
    val br = File("input.txt").bufferedReader()
    val out = PrintWriter(File("output.txt"))

    val stParams = StringTokenizer(br.readLine())
    val m = stParams.nextToken().toInt()
    val c = stParams.nextToken().toInt()
    val n = stParams.nextToken().toInt()

    val table = LongArray(m) { -1L }

    repeat(n) {
        val line = br.readLine()
        val x = line.toLong()

        val initialPos = ((x % m + m) % m).toInt()

        for (step in 0 until m) {
            val pos = ((initialPos.toLong() + step.toLong() * c) % m).toInt()

            if (table[pos] == x) {
                break
            }
            if (table[pos] == -1L) {
                table[pos] = x
                break
            }
        }
    }

    val sb = StringBuilder()
    for (i in 0 until m) {
        sb.append(table[i])
        if (i < m - 1) sb.append(" ")
    }
    out.println(sb.toString())

    out.close()
    br.close()
}