package org.example.controller

import org.junit.jupiter.api.Test
import org.springframework.beans.factory.annotation.Autowired
import org.springframework.boot.test.autoconfigure.web.servlet.AutoConfigureMockMvc
import org.springframework.boot.test.context.SpringBootTest
import org.springframework.http.MediaType
import org.springframework.test.web.servlet.MockMvc
import org.springframework.test.web.servlet.request.MockMvcRequestBuilders.get
import org.springframework.test.web.servlet.request.MockMvcRequestBuilders.post
import org.springframework.test.web.servlet.result.MockMvcResultMatchers.*

@SpringBootTest
@AutoConfigureMockMvc
class AlgorithmControllerTest @Autowired constructor(
    val mockMvc: MockMvc
) {

    @Test
    fun `GET fibonacci should return 200 and list`() {
        mockMvc.perform(get("/api/v1/algorithms/fibonacci").param("n", "3"))
            .andExpect(status().isOk)
            .andExpect(content().contentType(MediaType.APPLICATION_JSON))
            .andExpect(jsonPath("$[0]").value(1))
            .andExpect(jsonPath("$[1]").value(1))
            .andExpect(jsonPath("$[2]").value(2))
    }

    @Test
    fun `GET palindrome should return valid JSON`() {
        mockMvc.perform(get("/api/v1/algorithms/palindrome/121"))
            .andExpect(status().isOk)
            .andExpect(jsonPath("$.isPalindrome").value(true))
            .andExpect(jsonPath("$.number").value(121))
    }

    @Test
    fun `POST reverse should return reversed array`() {
        val inputJson = "[1, 2, 3, 4]"

        mockMvc.perform(post("/api/v1/algorithms/list/reverse")
            .contentType(MediaType.APPLICATION_JSON)
            .content(inputJson))
            .andExpect(status().isOk)
            .andExpect(content().string("[4,3,2,1]"))
    }

    @Test
    fun `GET fibonacci with invalid n should return 400`() {
        mockMvc.perform(get("/api/v1/algorithms/fibonacci").param("n", "-5"))
            .andExpect(status().isBadRequest)
            .andExpect(jsonPath("$.error").exists())
    }
}