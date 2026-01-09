package todo.controller

import todo.model.*
import todo.service.TaskService
import org.springframework.http.HttpStatus
import org.springframework.http.ResponseEntity
import org.springframework.web.bind.annotation.*

@RestController
@RequestMapping("/tasks")
class TaskController(private val service: TaskService) {

    @GetMapping
    fun getAll(): List<Task> = service.getAllTasks()

    @GetMapping("/{id}")
    fun getOne(@PathVariable id: Long): Task = service.getTaskById(id)

    @PostMapping
    fun create(@RequestBody request: CreateTaskRequest): ResponseEntity<Task> {
        val task = service.createTask(
            Task(title = request.title, description = request.description, status = request.status ?: TaskStatus.todo)
        )
        return ResponseEntity.status(HttpStatus.CREATED).body(task)
    }

    @PutMapping("/{id}")
    fun update(@PathVariable id: Long, @RequestBody request: CreateTaskRequest): ResponseEntity<Task> {
        val updated = service.updateTask(
            id,
            Task(title = request.title, description = request.description, status = request.status ?: TaskStatus.todo)
        )
        return ResponseEntity.ok(updated)
    }

    @PatchMapping("/{id}")
    fun patchStatus(@PathVariable id: Long, @RequestBody request: PatchTaskRequest): ResponseEntity<Task> {
        val updated = service.patchTaskStatus(id, request.status)
        return ResponseEntity.ok(updated)
    }

    @DeleteMapping("/{id}")
    fun delete(@PathVariable id: Long): ResponseEntity<Void> {
        service.deleteTask(id)
        return ResponseEntity.noContent().build()
    }

    @ExceptionHandler(NoSuchElementException::class)
    fun handleNotFound(e: NoSuchElementException): ResponseEntity<Map<String, String>> {
        return ResponseEntity.status(HttpStatus.NOT_FOUND).body(mapOf("error" to (e.message ?: "Not found")))
    }
}