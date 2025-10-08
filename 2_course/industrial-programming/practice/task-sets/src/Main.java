import java.io.*;
import java.util.HashSet;
import java.util.Objects;
import java.util.Scanner;
import java.util.Set;

public class SetOperations {

    private static final String FILE_A = "file_a.txt";
    private static final String FILE_B = "file_b.txt";
    private static final String FILE_RESULT = "file_result.txt";

    static class Student {
        private long longnum;
        private String name;
        private int group;
        private double grade;

        public Student(long longnum, String name, int group, double grade) {
            this.longnum = longnum;
            this.name = name;
            this.group = group;
            this.grade = grade;
        }

        @Override
        public String toString() {
            return longnum + "," + name + "," + group + "," + grade;
        }

        @Override
        public boolean equals(Object o) {
            if (this == o) return true;
            if (o == null || getClass() != o.getClass()) return false;
            Student student = (Student) o;
            return longnum == student.longnum &&
                    group == student.group &&
                    Double.compare(student.grade, grade) == 0 &&
                    Objects.equals(name, student.name);
        }

        @Override
        public int hashCode() {
            return Objects.hash(longnum, name, group, grade);
        }
    }

    static class FileUtils {

        public static Set<Student> readFile(String filename) {
            Set<Student> students = new HashSet<>();
            try (BufferedReader br = new BufferedReader(new FileReader(filename))) {
                String line;
                while ((line = br.readLine()) != null) {
                    try {
                        String[] parts = line.split(",");
                        if (parts.length == 4) {
                            long longnum = Long.parseLong(parts[0].trim());
                            String name = parts[1].trim();
                            int group = Integer.parseInt(parts[2].trim());
                            double grade = Double.parseDouble(parts[3].trim());

                            students.add(new Student(longnum, name, group, grade));
                        }
                    } catch (NumberFormatException | ArrayIndexOutOfBoundsException e) {
                        System.err.println("Ошибка при парсинге строки: " + line + " в файле " + filename);
                    }
                }
            } catch (FileNotFoundException e) {
                System.err.println("Файл не найден: " + filename);
            } catch (IOException e) {
                System.err.println("Ошибка чтения из файла: " + filename + ". " + e.getMessage());
            }
            return students;
        }

        public static void writeFile(String filename, Set<Student> students) {
            try (BufferedWriter bw = new BufferedWriter(new FileWriter(filename))) {
                for (Student student : students) {
                    bw.write(student.toString());
                    bw.newLine();
                }
                System.out.println("Результат записан в файл: " + filename);
            } catch (IOException e) {
                System.err.println("Ошибка записи в файл: " + filename + ". " + e.getMessage());
            }
        }
    }

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        int choice;

        do {
            displayMenu();
            System.out.print("Введите номер операции: ");
            if (scanner.hasNextInt()) {
                choice = scanner.nextInt();
                scanner.nextLine();
                performOperation(choice);
            } else {
                System.out.println("Некорректный ввод. Попробуйте снова.");
                scanner.nextLine();
                choice = 0;
            }
        } while (choice != 4);

        scanner.close();
        System.out.println("Программа завершена.");
    }

    private static void displayMenu() {
        System.out.println("\n===== Операции над множествами Student =====");
        System.out.println("1. Объединение (A ∪ B)");
        System.out.println("2. Пересечение (A ∩ B)");
        System.out.println("3. Разность (A \\ B)");
        System.out.println("4. Выход");
        System.out.println("------------------------------------------");
        System.out.println("Исходные файлы: " + FILE_A + ", " + FILE_B);
        System.out.println("Результирующий файл: " + FILE_RESULT);
    }

    private static void performOperation(int choice) {
        if (choice >= 1 && choice <= 3) {
            System.out.println("Чтение данных из файлов...");

            Set<Student> setA = FileUtils.readFile(FILE_A);
            Set<Student> setB = FileUtils.readFile(FILE_B);

            Set<Student> resultSet = new HashSet<>();

            switch (choice) {
                case 1:
                    resultSet.addAll(setA);
                    resultSet.addAll(setB);
                    System.out.println("Операция: Объединение (A ∪ B). Размер результата: " + resultSet.size());
                    break;
                case 2:
                    resultSet.addAll(setA);
                    resultSet.retainAll(setB);
                    System.out.println("Операция: Пересечение (A ∩ B). Размер результата: " + resultSet.size());
                    break;
                case 3:
                    resultSet.addAll(setA);
                    resultSet.removeAll(setB);
                    System.out.println("Операция: Разность (A \\ B). Размер результата: " + resultSet.size());
                    break;
            }

            FileUtils.writeFile(FILE_RESULT, resultSet);

        } else if (choice != 4) {
            System.out.println("Неизвестный пункт меню.");
        }
    }
}