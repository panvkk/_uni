package org.example.model;

public class Student {
    private int studentId;
    private String name;

    public Student(int studentId, String name) {
        this.studentId = studentId;
        this.name = name;
    }

    public int getStudentId() { return studentId; }
    public String getName() { return name; }

    @Override
    public String toString() {
        return "ID: " + studentId + " | Имя: " + name;
    }
}