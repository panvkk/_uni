package org.example

import java.io.BufferedReader
import java.io.File
import java.io.InputStreamReader
import java.io.PrintWriter
import java.util.Scanner
import java.util.PriorityQueue
import java.util.StringTokenizer


fun main() {
    val br = BufferedReader(InputStreamReader(File("input.txt").inputStream()))
    var st = StringTokenizer(br.readLine())
    val writer = PrintWriter(File("output.txt"))

    val n = st.nextToken().toInt()
    val q = st.nextToken().toInt()

    val parent = IntArray(n + 1) { it }
    val rank = IntArray(n + 1) { 0 }
    var componentsCount = n

    fun find(i: Int): Int {
        var root = i
        while (parent[root] != root) {
            root = parent[root]
        }
        var curr = i
        while (parent[curr] != root) {
            val next = parent[curr]
            parent[curr] = root
            curr = next
        }
        return root
    }

    fun union(i: Int, j: Int) {
        val rootI = find(i)
        val rootJ = find(j)

        if(rootI != rootJ) {
            if(rank[rootI] > rank[rootJ]) {
                parent[rootJ] = rootI
            } else if(rank[rootI] < rank[rootJ]) {
                parent[rootI] = rootJ
            } else {
                parent[rootI] = rootJ
                rank[rootI]++
            }
            componentsCount--
        }
    }

    for(i in 1..q) {
        st = StringTokenizer(br.readLine())
        val a = st.nextToken().toInt()
        val b = st.nextToken().toInt()

        union(a, b)

        writer.write(componentsCount.toString() + "\n")
    }

    br.close()

    writer.close()
}
