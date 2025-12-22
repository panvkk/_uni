package org.example;

import java.io.*;
import java.util.*;
import java.util.regex.*;
import java.util.stream.Collectors;

public class Main {
    private static final String INPUT_FILE = "input.txt";
    private static final String OUTPUT_FILE = "output.txt";

    public static void main(String[] args) throws IOException {
        List<GradeBook> grade_books = new ArrayList<>();
        Map<String, GradeBook> grade_book_map = new HashMap<>();

        try {
            readInputFile(grade_books, grade_book_map);
        } catch (IOException e) {
            System.err.println("Error reading " + INPUT_FILE + ": " + e.getMessage());
            return;
        }

        System.out.println("Count of students: " + grade_books.size());

        // Вывод списка студентов через Stream
        System.out.println(grade_books.stream()
                .map(GradeBook::getFullName)
                .collect(Collectors.joining("\n")));

        // Расчет средних баллов для всех студентов
        for (GradeBook gb : grade_books) {
            gb.calculateAllAverages();
        }

        // Поиск студента
        findStudent(grade_book_map);

        // Вывод списка отличников в консоль и файл
        try {
            writeOutput(grade_books);
        } catch (IOException e) {
            System.err.println("Error writing to file " + OUTPUT_FILE + ": " + e.getMessage());
        }
    }

    private static void readInputFile(List<GradeBook> grade_books, Map<String, GradeBook> grade_book_map) throws IOException {
        try (BufferedReader br = new BufferedReader(new FileReader(INPUT_FILE))) {
            String line;
            GradeBook current_book = null;

            outer: while ((line = br.readLine()) != null) {
                line = line.trim();

                if (line.isEmpty()) {
                    continue;
                }

                String[] arguments = line.split("\\s+");

                // 1. Попытка парсинга как новой зачетной книжки (GradeBook)
                if (arguments.length == 5) {
                    try {
                        String surname = arguments[0];
                        String name = arguments[1];
                        String middle_name = arguments[2];
                        byte course = parseByte(arguments[3]);
                        byte group = parseByte(arguments[4]);

                        if (isString(surname) && isString(name) && isString(middle_name)) {
                            current_book = new GradeBook(surname, name, middle_name, course, group);
                            grade_books.add(current_book);
                            String key = current_book.getFullName();
                            grade_book_map.put(key, current_book);
                        } else {
                            System.out.println("The string could not be recognized as a new student (name error): " + line);
                        }
                        // ИСПРАВЛЕНИЕ ОШИБКИ: Ловим только суперкласс IllegalArgumentException
                    } catch (IllegalArgumentException e) {
                        System.out.println("The string could not be recognized as a new student (parsing/validation error): " + line);
                    }
                }
                // 2. Попытка парсинга как дисциплины
                else if (arguments.length >= 4) {
                    if (current_book == null) {
                        System.out.println("Discipline record found without a preceding student: " + line);
                        continue;
                    }

                    try {
                        byte session_number = parseByte(arguments[0]);
                        byte mark = parseByte(arguments[arguments.length - 1]);
                        boolean is_exam = parseBoolean(arguments[arguments.length - 2]);

                        // Собираем название дисциплины
                        String discipline_name = Arrays.stream(arguments)
                                .skip(1)
                                .limit(arguments.length - 3)
                                .collect(Collectors.joining(" "));

                        String[] name_parts = discipline_name.split("\\s+");
                        if (discipline_name.isEmpty() || isByte(name_parts[0]) || isBoolean(name_parts[0])) {
                            System.out.println("The string could not be recognized as a discipline (name error): " + line);
                            continue;
                        }

                        GradeBook.Session current_session = current_book.getSession(session_number);
                        current_session.addDiscipline(discipline_name, is_exam, mark);

                        // ИСПРАВЛЕНИЕ ОШИБКИ: Ловим только суперкласс IllegalArgumentException
                    } catch (IllegalArgumentException e) {
                        System.out.println("The string could not be recognized as a discipline (parsing/validation error): " + line);
                    }
                }
                else {
                    System.out.println("The string could not be recognized: " + line);
                }
            }
        }
    }

    private static void findStudent(Map<String, GradeBook> grade_book_map) {
        // Используем Scanner для консольного ввода
        try (Scanner scanner = new Scanner(System.in)) {
            System.out.print("Enter student's full name (Surname Name Middle_name) to start searching: ");
            if (scanner.hasNextLine()) {
                String lookupKey = scanner.nextLine().trim();
                GradeBook book = grade_book_map.get(lookupKey);

                if (book != null) {
                    System.out.println("Student found: " + lookupKey);
                    System.out.printf("Average mark for all sessions: %.2f%n", book.getAverageAll());
                } else {
                    System.out.println("Student " + lookupKey + " is not found");
                }
            }
        } catch (NoSuchElementException e) {
            System.err.println("Input error: No line found!");
        }
    }

    private static void writeOutput(List<GradeBook> grade_books) throws IOException {
        try (PrintWriter output = new PrintWriter(new FileWriter(OUTPUT_FILE))) {
            boolean excellent_exist = false;
            System.out.println("\nСписок отличников:\n");

            for (GradeBook gb : grade_books) {
                if (gb.isExcellent()) {
                    excellent_exist = true;

                    String studentInfo = String.format("%s %d курс %d группа", gb.getFullName(), gb.getCourse(), gb.getGroup());
                    System.out.println(studentInfo);
                    output.println(studentInfo);

                    // Используем константы для цикла (исправляет ошибки доступа)
                    for (byte i = GradeBook.MIN_SESSION; i <= GradeBook.MAX_SESSION; i++) {
                        GradeBook.Session session = gb.getSession(i);

                        if (!session.isDisciplinesEmpty()) {
                            String sessionAverage = String.format("Номер сессии: %d%nСредний балл за эту сессию: %.2f", i, session.getAverage());
                            System.out.println(sessionAverage);
                            output.println(sessionAverage);

                            for (GradeBook.Session.Discipline disc : session.getDisciplines()) {
                                String type = disc.isExam() ? "Экзамен" : "Зачёт";
                                String result;

                                if (disc.isExam()) {
                                    result = String.valueOf(disc.getMark());
                                } else {
                                    // Используем константу (исправляет ошибки доступа)
                                    result = (disc.getMark() == GradeBook.CREDIT_PASSED_MARK) ? "Зачтено" : "Незачтено";
                                }

                                String disciplineInfo = String.format("%s по дисциплине \"%s\". Результат: %s", type, disc.getNameOfDiscipline(), result);
                                System.out.println(disciplineInfo);
                                output.println(disciplineInfo);
                            }
                        }
                    }
                    String allAverage = String.format("Средний балл за все сессии: %.2f", gb.getAverageAll());
                    System.out.println(allAverage);
                    output.println(allAverage);
                    System.out.println();
                    output.println();
                }
            }
            if (!excellent_exist) {
                System.out.println("Отличников нет :(");
            }
        }
    }

    // Вспомогательные методы
    private static byte parseByte(String s) throws NumberFormatException {
        return Byte.parseByte(s);
    }

    private static boolean parseBoolean(String s) throws IllegalArgumentException {
        String input = s.trim();
        if (input.equalsIgnoreCase("true")) return true;
        if (input.equalsIgnoreCase("false")) return false;
        throw new IllegalArgumentException("Invalid boolean string: " + s);
    }

    public static boolean isString(String s) {
        return !(isByte(s) || isBoolean(s));
    }

    private static boolean isByte(String s) {
        if (s == null) return false;
        try {
            Byte.parseByte(s);
            return true;
        }
        catch (NumberFormatException e) {
            return false;
        }
    }

    private static boolean isBoolean(String s) {
        if (s == null) return false;
        String input = s.trim();
        return input.equalsIgnoreCase("true") || input.equalsIgnoreCase("false");
    }
}