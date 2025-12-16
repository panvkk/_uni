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

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        System.out.print("Введите ширину строки: ");
        int width = scanner.nextInt();

        if (width < 1) {
            System.out.println("Ширина должна быть >= 1");
            return;
        }

        FileProcessor processor = new FileProcessor("input.txt", "output.txt");

        try {
            List<String> inputLines = processor.readLines();

            // 1. Разбиваем на абзацы (пустая строка - разделитель)
            List<List<String>> paragraphs = splitToParagraphs(inputLines);
            List<String> finalResult = new ArrayList<>();

            for (int i = 0; i < paragraphs.size(); i++) {
                List<String> words = splitParagraphToWords(paragraphs.get(i));

                if (!words.isEmpty()) {
                    // 2. Форматируем абзац с отступом (например, 4 пробела)
                    List<String> justifiedPara = justifyTextWithIndent(words, width, 4);
                    finalResult.addAll(justifiedPara);
                }

                // Добавляем пустую строку между абзацами
                if (i < paragraphs.size() - 1) {
                    finalResult.add("");
                }
            }

            processor.writeLines(finalResult);
            System.out.println("Файл успешно отформатирован и сохранён в output.txt");

        } catch (IOException e) {
            System.err.println("Ошибка работы с файлами: " + e.getMessage());
        }
    }

    // Алгоритм распределения пробелов (Full Justify)
    private static String justifyLine(List<String> words, int maxWidth) {
        if (words.size() == 1) {
            return words.get(0) + " ".repeat(Math.max(0, maxWidth - words.get(0).length()));
        }

        int totalWordsLen = words.stream().mapToInt(String::length).sum();
        int totalSpaces = maxWidth - totalWordsLen;
        int gaps = words.size() - 1;
        int baseSpace = totalSpaces / gaps;
        int extra = totalSpaces % gaps;

        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < words.size(); i++) {
            sb.append(words.get(i));
            if (i < gaps) {
                int spacesToApply = baseSpace + (i < extra ? 1 : 0);
                sb.append(" ".repeat(spacesToApply));
            }
        }
        return sb.toString();
    }

    // Выравнивание по левому краю (для последней строки абзаца)
    private static String leftJustifyLine(List<String> words, int maxWidth) {
        String line = String.join(" ", words);
        if (line.length() < maxWidth) {
            line += " ".repeat(maxWidth - line.length());
        }
        return line;
    }

    // Логика форматирования текста с учетом красной строки (индента)
    private static List<String> justifyTextWithIndent(List<String> words, int maxWidth, int indent) {
        List<String> result = new ArrayList<>();
        int pos = 0;
        boolean isFirstLine = true;

        while (pos < words.size()) {
            int currentWidth = isFirstLine ? Math.max(0, maxWidth - indent) : maxWidth;
            List<String> lineWords = new ArrayList<>();
            int currentLen = 0;

            while (pos < words.size()) {
                String word = words.get(pos);
                // Обработка слов длиннее ширины строки
                if (word.length() > currentWidth && lineWords.isEmpty()) {
                    String prefix = isFirstLine ? " ".repeat(indent) : "";
                    result.add(prefix + word);
                    pos++;
                    isFirstLine = false;
                    break;
                }

                int spaceNeeded = lineWords.isEmpty() ? 0 : 1;
                if (currentLen + spaceNeeded + word.length() <= currentWidth) {
                    lineWords.add(word);
                    currentLen += spaceNeeded + word.length();
                    pos++;
                } else {
                    break;
                }
            }

            if (!lineWords.isEmpty()) {
                String formattedLine;
                boolean isLastLine = (pos >= words.size());

                if (isLastLine) {
                    formattedLine = leftJustifyLine(lineWords, currentWidth);
                } else {
                    formattedLine = justifyLine(lineWords, currentWidth);
                }

                if (isFirstLine) {
                    result.add(" ".repeat(indent) + formattedLine);
                } else {
                    result.add(formattedLine);
                }
                isFirstLine = false;
            }
        }
        return result;
    }

    private static List<List<String>> splitToParagraphs(List<String> lines) {
        List<List<String>> paragraphs = new ArrayList<>();
        List<String> current = new ArrayList<>();
        for (String line : lines) {
            if (line.trim().isEmpty()) {
                if (!current.isEmpty()) {
                    paragraphs.add(new ArrayList<>(current));
                    current.clear();
                }
            } else {
                current.add(line);
            }
        }
        if (!current.isEmpty()) paragraphs.add(current);
        return paragraphs;
    }

    private static List<String> splitParagraphToWords(List<String> paraLines) {
        List<String> words = new ArrayList<>();
        for (String line : paraLines) {
            String[] parts = line.trim().split("\\s+");
            for (String p : parts) if (!p.isEmpty()) words.add(p);
        }
        return words;
    }
}