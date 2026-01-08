package org.example.service

data class Node<T>(
    val value: T,
    var next: Node<T>? = null
)