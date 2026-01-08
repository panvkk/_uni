package org.example.controller

import org.example.service.AlgorithmService
import org.example.service.Node
import org.springframework.web.bind.annotation.*
import java.math.BigInteger

@RestController
@RequestMapping("/api/v1/algorithms")
class AlgorithmController(private val algorithmService: AlgorithmService) {

    @GetMapping("/fibonacci")
    fun getFibonacci(@RequestParam n: Int): List<BigInteger> =
        algorithmService.getFibonacciSequence(n)

    @GetMapping("/palindrome/{number}")
    fun checkPalindrome(@PathVariable number: Long): PalindromeResponse =
        PalindromeResponse(number, algorithmService.isPalindrome(number))

    @PostMapping("/list/reverse")
    fun reverseList(@RequestBody values: List<Int>): List<Int> {
        if (values.isEmpty()) return emptyList()

        val head = values.map { Node(it) }.reduceRight { node, acc ->
            node.apply { next = acc }
        }

        var current = algorithmService.reverseLinkedList(head)

        val result = mutableListOf<Int>()
        while (current != null) {
            result.add(current.value)
            current = current.next
        }
        return result
    }
}