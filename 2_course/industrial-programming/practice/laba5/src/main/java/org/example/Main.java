package org.example;

import java.io.*;
import java.util.*;

class FileProcessor {
    private final String inputFile;
    private final String outputFile;

    public FileProcessor(String inputFile, String outputFile) {
        this.inputFile = inputFile;
        this.outputFile = outputFile;
    }

    public List<String> readLines() throws IOException {
        List<String> lines = new ArrayList<>();
        try (BufferedReader br = new BufferedReader(new FileReader(inputFile))) {
            String line;
            while ((line = br.readLine()) != null) {
                lines.add(line);
            }
        }
        return lines;
    }

    public void writeLines(List<String> lines) throws IOException {
        try (BufferedWriter bw = new BufferedWriter(new FileWriter(outputFile))) {
            for (String line : lines) {
                bw.write(line);
                bw.newLine();
            }
        }
    }
}

public class Main {
    public static List<String> justifyText(List<String> words, int width) {
        List<String> result = new ArrayList<>();
        List<String> currentLine = new ArrayList<>();
        int currentLength = 0;

        for (String word : words) {
            if (currentLength + word.length() + currentLine.size() > width) {
                result.add(justifyLine(currentLine, width));
                currentLine.clear();
                currentLength = 0;
            }
            currentLine.add(word);
            currentLength += word.length();
        }

        if (!currentLine.isEmpty()) {
            result.add(String.join(" ", currentLine));
        }

        return result;
    }

    // Вспомогательный метод для распределения пробелов
    private static String justifyLine(List<String> words, int width) {
        if (words.size() == 1) {
            return words.get(0); // если одно слово — просто вернуть
        }

        int totalChars = words.stream().mapToInt(String::length).sum();
        int spaces = width - totalChars;
        int gaps = words.size() - 1;

        int spacePerGap = spaces / gaps;
        int extraSpaces = spaces % gaps;

        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < words.size(); i++) {
            sb.append(words.get(i));
            if (i < gaps) {
                sb.append(" ".repeat(spacePerGap + (i < extraSpaces ? 1 : 0)));
            }
        }
        return sb.toString();
    }

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        System.out.print("Введите ширину строки: ");
        int width = scanner.nextInt();

        FileProcessor processor = new FileProcessor("input.txt", "output.txt");

        try {
            // Читаем весь текст и разбиваем на слова
            List<String> lines = processor.readLines();
            List<String> words = new ArrayList<>();
            for (String line : lines) {
                String[] parts = line.trim().split("\\s+");
                words.addAll(Arrays.asList(parts));
            }

            // Форматируем
            List<String> justified = justifyText(words, width);

            // Записываем в файл
            processor.writeLines(justified);

            System.out.println("Файл успешно отформатирован и сохранён в output.txt");
        } catch (IOException e) {
            System.err.println("Ошибка работы с файлами: " + e.getMessage());
        }
    }
}
