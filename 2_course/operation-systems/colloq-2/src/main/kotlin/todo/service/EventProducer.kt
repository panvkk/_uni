package todo.service

import org.springframework.amqp.rabbit.core.RabbitTemplate
import org.springframework.stereotype.Service

@Service
class EventProducer(private val rabbitTemplate: RabbitTemplate) {
    fun sendTaskEvent(message: String) {
        rabbitTemplate.convertAndSend("task-events", message)
        println("Message sent to queue: $message")
    }
}