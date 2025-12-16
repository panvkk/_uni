package org.example;

import java.io.BufferedReader;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;

public class StudentData {

    /**
     * Современная модель данных (Record).
     * Валидация выполняется в конструкторе.
     */
    public record Student(long number, String name, int group, double grade) {
        public Student {
            if (number < 0) throw new IllegalArgumentException("Номер не может быть отрицательным");
            if (group <= 0) throw new IllegalArgumentException("Группа должна быть > 0");
            if (grade < 0) throw new IllegalArgumentException("Балл не может быть отрицательным");
        }

        // Для удобного поиска
        public String getNameString() {
            return name;
        }
    }

    /**
     * Логика загрузки из файла.
     * Поддерживает формат: [Номер] [Имя Фамилия...] [Группа] [Балл]
     */
    public static List<Student> loadFromFile(Path path) throws IOException {
        List<Student> list = new ArrayList<>();
        try (BufferedReader br = Files.newBufferedReader(path)) {
            String line;
            int lineNo = 0;
            while ((line = br.readLine()) != null) {
                lineNo++;
                line = line.trim();
                if (line.isEmpty()) continue;

                String[] tokens = line.split("\\s+");
                // Нужно минимум 4 токена: Номер, Имя(мин 1 слово), Группа, Балл
                if (tokens.length >= 4) {
                    try {
                        long number = Long.parseLong(tokens[0]);
                        // Балл - последний
                        double grade = Double.parseDouble(tokens[tokens.length - 1]);
                        // Группа - предпоследняя
                        int group = Integer.parseInt(tokens[tokens.length - 2]);

                        // Всё, что между номером и группой — это имя
                        StringBuilder nameSb = new StringBuilder();
                        for (int i = 1; i <= tokens.length - 3; i++) {
                            if (i > 1) nameSb.append(' ');
                            nameSb.append(tokens[i]);
                        }

                        list.add(new Student(number, nameSb.toString(), group, grade));
                    } catch (NumberFormatException ex) {
                        System.err.println("Ошибка чисел в строке " + lineNo + ": " + line);
                    } catch (IllegalArgumentException ex) {
                        System.err.println("Ошибка валидации в строке " + lineNo + ": " + ex.getMessage());
                    }
                } else {
                    System.err.println("Неверный формат строки " + lineNo);
                }
            }
        }
        return list;
    }
}