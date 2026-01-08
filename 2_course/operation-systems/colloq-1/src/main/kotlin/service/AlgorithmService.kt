package org.example.service

import java.math.BigInteger
import org.springframework.stereotype.Service

@Service
class AlgorithmService {

    fun getFibonacciSequence(n: Int): List<BigInteger> {
        require(n > 0) { "Количество чисел n должно быть положительным" }

        val sequence = mutableListOf<BigInteger>()
        var a = BigInteger.ZERO
        var b = BigInteger.ONE

        for (i in 1..n) {
            sequence.add(b)
            val next = a.add(b)
            a = b
            b = next
        }
        return sequence
    }

    fun isPalindrome(number: Long): Boolean {
        if (number < 0) return false
        if (number < 10) return true

        var reversed = 0L
        var temp = number
        while (temp > 0) {
            reversed = reversed * 10 + temp % 10
            temp /= 10
        }
        return number == reversed
    }

    fun <T> reverseLinkedList(head: Node<T>?): Node<T>? {
        var prev: Node<T>? = null
        var current = head
        var next: Node<T>?

        while (current != null) {
            next = current.next
            current.next = prev
            prev = current
            current = next
        }
        return prev
    }
}