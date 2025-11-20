package org.example;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.Arrays;
import java.util.stream.Collectors;

public class FileUtil {

    /**
     * Генерирует массив случайных чисел для использования по умолчанию.
     */
    public static Integer[] generateRandomArray(int size) {
        Integer[] arr = new Integer[size];
        for (int i = 0; i < size; i++) {
            // Значения от 1 до 200
            arr[i] = (int) (Math.random() * 200) + 1;
        }
        return arr;
    }

    /**
     * Считывает массив целых чисел из файла, расположенного в папке resources.
     */
    public static Integer[] readArrayFromFile(String filename, int defaultSize) {
        // Используем ClassLoader для доступа к ресурсу через путь /filename
        try (InputStream is = SortingApplication.class.getResourceAsStream("/" + filename)) {
            if (is == null) {
                throw new java.io.IOException("Файл не найден в resources: /" + filename);
            }

            String content;
            try (BufferedReader reader = new BufferedReader(new InputStreamReader(is))) {
                content = reader.lines().collect(Collectors.joining("\n")).trim();
            }

            if (content.isEmpty()) {
                System.err.println("Файл пуст. Использован случайный массив.");
                return generateRandomArray(defaultSize);
            }

            // Разделяем по запятым или пробелам
            String[] parts = content.split("[,\\s]+");

            return Arrays.stream(parts)
                    .filter(s -> !s.isEmpty())
                    .map(Integer::parseInt)
                    .toArray(Integer[]::new);

        } catch (java.io.IOException | NumberFormatException e) {
            System.err.println("Ошибка чтения или парсинга файла: " + e.getMessage() + ". Использован случайный массив.");
            return generateRandomArray(defaultSize);
        }
    }
}