package org.example

import java.io.BufferedReader
import java.io.InputStreamReader
import java.io.PrintWriter
import java.util.StringTokenizer

fun main() {
    val br = BufferedReader(InputStreamReader(System.`in`))
    var st = StringTokenizer(br.readLine())
    val out = PrintWriter(System.`out`)

    if (!st.hasMoreTokens()) return
    val n = st.nextToken().toInt()

    val tree = LongArray(n + 1)
    st = StringTokenizer(br.readLine())
    for (i in 0 until n) {
        val value = st.nextToken().toLong()

        var idx = i + 1
        while (idx <= n) {
            tree[idx] += value
            idx += idx and -idx
        }
    }

    val qLine = br.readLine()
    val q = qLine.trim().toInt()

    repeat(q) {
        st = StringTokenizer(br.readLine())
        val type = st.nextToken()

        if (type == "Add") {
            val i = st.nextToken().toInt()
            val x = st.nextToken().toLong()

            var idx = i + 1
            while (idx <= n) {
                tree[idx] += x
                idx += idx and -idx
            }
        } else if (type == "FindSum") {
            val l = st.nextToken().toInt()
            val r = st.nextToken().toInt()

            var sumR = 0L
            var idxR = r
            while (idxR > 0) {
                sumR += tree[idxR]
                idxR -= idxR and -idxR
            }

            var sumL = 0L
            var idxL = l
            while (idxL > 0) {
                sumL += tree[idxL]
                idxL -= idxL and -idxL
            }
            out.println(sumR - sumL)
        }
    }

    out.flush()
    out.close()
    br.close()
}