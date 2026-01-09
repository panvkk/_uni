package todo.service

import org.springframework.cache.annotation.CacheEvict
import org.springframework.cache.annotation.CachePut
import org.springframework.cache.annotation.Cacheable
import org.springframework.stereotype.Service
import org.springframework.transaction.annotation.Transactional
import io.github.resilience4j.circuitbreaker.annotation.CircuitBreaker
import todo.model.Task
import todo.model.TaskStatus
import todo.repository.TaskRepository

@Service
class TaskService(
    private val repository: TaskRepository,
    private val eventProducer: EventProducer
) {

    @Cacheable(value = ["tasks"], key = "'all'")
    fun getAllTasks(): List<Task> = repository.findAll()

    @Cacheable(value = ["task"], key = "#id")
    @CircuitBreaker(name = "taskService", fallbackMethod = "getTaskFallback")
    fun getTaskById(id: Long): Task {
        return repository.findById(id).orElseThrow { NoSuchElementException("Task not found") }
    }

    fun getTaskFallback(id: Long, t: Throwable): Task {
        return Task(id = id, title = "Fallback Task", description = "Service Unavailable", status = TaskStatus.todo)
    }

    @Transactional
    @CacheEvict(value = ["tasks"], key = "'all'")
    fun createTask(task: Task): Task {
        val saved = repository.save(task)
        eventProducer.sendTaskEvent("Task created with ID: ${saved.id}")
        return saved
    }

    @Transactional
    @CachePut(value = ["task"], key = "#id")
    @CacheEvict(value = ["tasks"], key = "'all'")
    fun updateTask(id: Long, updatedTask: Task): Task {
        return repository.findById(id).map {
            it.title = updatedTask.title
            it.description = updatedTask.description
            it.status = updatedTask.status
            repository.save(it)
        }.orElseThrow { NoSuchElementException("Task not found") }
    }

    @Transactional
    @CachePut(value = ["task"], key = "#id")
    @CacheEvict(value = ["tasks"], key = "'all'")
    fun patchTaskStatus(id: Long, status: TaskStatus): Task {
        val task = repository.findById(id).orElseThrow { NoSuchElementException("Task not found") }
        task.status = status
        return repository.save(task)
    }

    @CacheEvict(value = ["task", "tasks"], allEntries = true)
    fun deleteTask(id: Long) {
        if (repository.existsById(id)) {
            repository.deleteById(id)
            eventProducer.sendTaskEvent("Task deleted with ID: $id")
        } else {
            throw NoSuchElementException("Task not found")
        }
    }
}