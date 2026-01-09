package todo.repository

import org.springframework.data.jpa.repository.JpaRepository
import org.springframework.stereotype.Repository
import todo.model.Task

@Repository
interface TaskRepository : JpaRepository<Task, Long>