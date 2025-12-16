import java.io.File
import java.io.PrintWriter
import java.util.*

private const val ALPHABET_SIZE = 26

fun charToIndex(c: Char): Int {
    return c - 'a'
}
class GraphData(val n: Int) {
    val adj = Array(ALPHABET_SIZE) { mutableListOf<String>() }
    val outgoingDegree = IntArray(ALPHABET_SIZE)
    val incomingDegree = IntArray(ALPHABET_SIZE)
    val usedNodes = BooleanArray(ALPHABET_SIZE)
}

fun buildGraph(scanner: Scanner, n: Int): GraphData {
    val graph = GraphData(n)
    for (i in 0 until n) {
        val word = scanner.next()

        val startChar = word.first()
        val endChar = word.last()

        val startIndex = charToIndex(startChar)
        val endIndex = charToIndex(endChar)

        graph.adj[startIndex].add(word)

        graph.outgoingDegree[startIndex]++
        graph.incomingDegree[endIndex]++

        graph.usedNodes[startIndex] = true
        graph.usedNodes[endIndex] = true
    }
    return graph
}
fun checkEulerCondition(graph: GraphData): Boolean {
    for (i in 0 until ALPHABET_SIZE) {
        if (graph.outgoingDegree[i] != graph.incomingDegree[i]) {
            return false
        }
    }
    return true
}
fun checkConnectivity(graph: GraphData): Boolean {
    val n = graph.n

    var startNode = -1
    for (i in 0 until ALPHABET_SIZE) {
        if (graph.outgoingDegree[i] > 0) {
            startNode = i
            break
        }
    }

    if (startNode == -1) return n == 0

    val visitedNodes = BooleanArray(ALPHABET_SIZE)
    var edgesCount = 0
    val stack = Stack<Int>()
    stack.push(startNode)
    visitedNodes[startNode] = true

    while (stack.isNotEmpty()) {
        val u = stack.pop()

        edgesCount += graph.outgoingDegree[u]

        for (word in graph.adj[u]) {
            val v = charToIndex(word.last())
            if (!visitedNodes[v]) {
                visitedNodes[v] = true
                stack.push(v)
            }
        }
    }

    for (i in 0 until ALPHABET_SIZE) {
        if (graph.outgoingDegree[i] > 0 && !visitedNodes[i]) {
            return false
        }
    }

    return edgesCount == n
}


fun main() {
    val inputFile = File("input.txt")
    val outputFile = File("output.txt")

    val scanner = Scanner(inputFile)
    val printWriter = PrintWriter(outputFile)

    val n = scanner.nextInt()
    val graph = buildGraph(scanner, n)
    val eulerConditionMet = checkEulerCondition(graph)

    if (!eulerConditionMet) {
        printWriter.println("No")
        scanner.close()
        printWriter.close()
        return
    }

    val isConnected = checkConnectivity(graph)

    if (isConnected) {
        printWriter.println("Yes")
    } else {
        printWriter.println("No")
    }
    scanner.close()
    printWriter.close()
}