package org.example;

import org.example.model.Grade;
import org.example.model.Student;

import java.util.*;


public class StudentSystem {
    private static Map<Integer, Student> students = new HashMap<>();
    private static List<Grade> grades = new ArrayList<>();
    private static Scanner scanner = new Scanner(System.in);

    public static void main(String[] args) {
        while (true) {
            System.out.println("\n--- Меню ---");
            System.out.println("1. Добавить студента");
            System.out.println("2. Добавить оценку студенту");
            System.out.println("3. Средний балл студента");
            System.out.println("4. Студенты со средним баллом выше заданного");
            System.out.println("5. Выход");
            System.out.print("Выберите пункт: ");

            int choice = scanner.nextInt();
            scanner.nextLine();

            switch (choice) {
                case 1 -> addStudent();
                case 2 -> addGrade();
                case 3 -> showAverageGrade();
                case 4 -> showTopStudents();
                case 5 -> {
                    System.out.println("Выход из программы...");
                    return;
                }
                default -> System.out.println("Неверный выбор!");
            }
        }
    }

    private static void addStudent() {
        System.out.print("Введите ID студента: ");
        int id = scanner.nextInt();
        scanner.nextLine();
        System.out.print("Введите имя студента: ");
        String name = scanner.nextLine();

        students.put(id, new Student(id, name));
        System.out.println("Студент добавлен.");
    }

    private static void addGrade() {
        System.out.print("Введите ID студента: ");
        int id = scanner.nextInt();
        if (!students.containsKey(id)) {
            System.out.println("Студент с таким ID не найден!");
            return;
        }
        scanner.nextLine();
        System.out.print("Введите название предмета: ");
        String subject = scanner.nextLine();
        System.out.print("Введите оценку: ");
        int score = scanner.nextInt();

        grades.add(new Grade(id, subject, score));
        System.out.println("Оценка добавлена.");
    }

    private static double calculateAverage(int studentId) {
        return grades.stream()
                .filter(g -> g.getStudentId() == studentId)
                .mapToInt(Grade::getGrade)
                .average()
                .orElse(0.0);
    }

    private static void showAverageGrade() {
        System.out.print("Введите ID студента: ");
        int id = scanner.nextInt();
        if (students.containsKey(id)) {
            double avg = calculateAverage(id);
            System.out.printf("Средний балл студента %s: %.2f\n", students.get(id).getName(), avg);
        } else {
            System.out.println("Студент не найден.");
        }
    }

    private static void showTopStudents() {
        System.out.print("Введите минимальный средний балл: ");
        double threshold = scanner.nextDouble();

        System.out.println("Студенты с баллом выше " + threshold + ":");
        boolean found = false;
        for (Student s : students.values()) {
            double avg = calculateAverage(s.getStudentId());
            if (avg > threshold) {
                System.out.printf("- %s (Средний балл: %.2f)\n", s.getName(), avg);
                found = true;
            }
        }
        if (!found) System.out.println("Таких студентов нет.");
    }
}