package org.example.model;

public class Grade {
    private int studentId;
    private String subjectName;
    private int grade;

    public Grade(int studentId, String subjectName, int grade) {
        this.studentId = studentId;
        this.subjectName = subjectName;
        this.grade = grade;
    }

    public int getStudentId() { return studentId; }
    public int getGrade() { return grade; }
}