package org.example

import java.io.BufferedReader
import java.io.File
import java.io.InputStreamReader
import java.io.PrintWriter
import java.util.Scanner
import java.util.PriorityQueue
import java.util.StringTokenizer

class Node(val vertex: Int, val distance: Long) : Comparable<Node> {
    override fun compareTo(other: Node): Int = this.distance.compareTo(other.distance)
}
class Edge(val to: Int, val weight: Int)

fun main() {
    val br = BufferedReader(InputStreamReader(File("input.txt").inputStream()))
    var st = StringTokenizer(br.readLine())
    val writer = PrintWriter(File("output.txt"))

    val n = st.nextToken().toInt()
    val m = st.nextToken().toInt()
    val adj = Array(n + 1) { mutableListOf<Edge>() }

    for (i in 0 until m) {
        st = StringTokenizer(br.readLine())
        val u = st.nextToken().toInt()
        val v = st.nextToken().toInt()
        val w = st.nextToken().toInt()

        adj[u].add(Edge(v, w))
        adj[v].add(Edge(u, w))
    }

    val dist = LongArray(n + 1) { Long.MAX_VALUE }
    dist[1] = 0

    val pq = PriorityQueue<Node>()
    pq.add(Node(1, 0))

    while (pq.isNotEmpty()) {
        val curr = pq.poll()
        val u = curr.vertex
        val d = curr.distance

        if (d > dist[u]) {
            continue
        }
        if (u == n) {
            break
        }
        for (edge in adj[u]) {
            if (dist[u] + edge.weight < dist[edge.to]) {
                dist[edge.to] = dist[u] + edge.weight
                pq.add(Node(edge.to, dist[edge.to]))
            }
        }
    }
    br.close()

    writer.write(dist[n].toString())
    writer.close()
}