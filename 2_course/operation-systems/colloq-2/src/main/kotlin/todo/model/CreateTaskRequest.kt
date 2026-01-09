package todo.model

data class CreateTaskRequest(
    val title: String,
    val description: String?,
    val status: TaskStatus? = TaskStatus.todo
)