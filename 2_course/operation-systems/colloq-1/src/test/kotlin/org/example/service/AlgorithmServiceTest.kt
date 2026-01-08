package org.example.service

import org.assertj.core.api.Assertions
import org.junit.jupiter.api.Test
import org.junit.jupiter.api.assertThrows
import java.math.BigInteger

class AlgorithmServiceTest {

    private val service = AlgorithmService()

    @Test
    fun `fibonacci should return correct sequence for n=5`() {
        val expected = listOf(1, 1, 2, 3, 5).map { BigInteger.valueOf(it.toLong()) }
        val result = service.getFibonacciSequence(5)
        Assertions.assertThat(result).isEqualTo(expected)
    }

    @Test
    fun `fibonacci should throw exception for negative n`() {
        val exception = assertThrows<IllegalArgumentException> {
            service.getFibonacciSequence(-1)
        }
        Assertions.assertThat(exception.message).contains("должно быть положительным")
    }

    @Test
    fun `palindrome should identify symmetrical numbers`() {
        Assertions.assertThat(service.isPalindrome(121)).isTrue
        Assertions.assertThat(service.isPalindrome(12321)).isTrue
        Assertions.assertThat(service.isPalindrome(7)).isTrue
    }

    @Test
    fun `palindrome should return false for non-symmetrical numbers`() {
        Assertions.assertThat(service.isPalindrome(123)).isFalse
        Assertions.assertThat(service.isPalindrome(10)).isFalse
    }

    @Test
    fun `reverseLinkedList should correctly reverse a sequence`() {
        val head = Node(1, Node(2, Node(3)))

        val reversedHead = service.reverseLinkedList(head)

        Assertions.assertThat(reversedHead?.value).isEqualTo(3)
        Assertions.assertThat(reversedHead?.next?.value).isEqualTo(2)
        Assertions.assertThat(reversedHead?.next?.next?.value).isEqualTo(1)
        Assertions.assertThat(reversedHead?.next?.next?.next).isNull()
    }
}