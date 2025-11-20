package org.example;

import com.squareup.moshi.*;
import com.squareup.moshi.Types;
import okio.BufferedSink;
import okio.BufferedSource;
import okio.Okio;

import java.io.File;
import java.io.IOException;
import java.lang.reflect.Type;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

public class Main {
    public static void main(String[] args) throws IOException {
        Moshi moshi = new Moshi.Builder().build();
        Type type = Types.newParameterizedType(List.class, GradeBook.class);
        JsonAdapter<List<GradeBook>> adapter = moshi.adapter(type);

        // --- 1. Чтение из input.txt и создание GradeBook List ---
        List<GradeBook> allGradeBooks = new ArrayList<>();
        File inputFileTxt = new File("input.txt");

        System.out.println("Чтение данных из input.txt...");
        try (BufferedSource source = Okio.buffer(Okio.source(inputFileTxt))) {
            while (true) {
                String line = source.readUtf8Line();
                if (line == null) break;
                try {
                    allGradeBooks.add(GradeBook.fromTxtLine(line));
                } catch (IllegalArgumentException e) {
                    System.err.println("Ошибка парсинга строки: " + line + ". " + e.getMessage());
                }
            }
        } catch (IOException e) {
            System.err.println("Ошибка при чтении файла input.txt: " + e.getMessage());
            return;
        }

        // --- 2. Запись в input.json ---
        File outputFileJson = new File("input.json");
        System.out.println("Формирование файла input.json...");
        try (BufferedSink sink = Okio.buffer(Okio.sink(outputFileJson))) {
            adapter.indent("  ").toJson(sink, allGradeBooks);
        }
        System.out.println("Файл input.json сформирован.");

        // --- 3. Фильтрация отличников (согласно изначальному условию) ---
        List<GradeBook> excellent = allGradeBooks.stream()
                .filter(GradeBook::isExcellent)
                .collect(Collectors.toList());

        // --- 4. Запись в output.json (список отличников) ---
        File outputFileExcellentJson = new File("output.json");
        System.out.println("Формирование файла output.json (отличники)...");
        try (BufferedSink sink = Okio.buffer(Okio.sink(outputFileExcellentJson))) {
            adapter.indent("  ").toJson(sink, excellent);
        }
        System.out.println("Файл output.json сформирован.");

        // --- 5. Запись в output.txt (список отличников в текстовом виде) ---
        File outputFileTxt = new File("output.txt");
        System.out.println("Формирование файла output.txt (отличники)...");
        try (BufferedSink sink = Okio.buffer(Okio.sink(outputFileTxt))) {
            // Добавляем заголовок
            sink.writeUtf8("Фамилия Имя Отчество;Курс;Группа;Статус\n");
            for (GradeBook gb : excellent) {
                sink.writeUtf8(gb.toTxtString() + "\n");
            }
        }
        System.out.println("Файл output.txt сформирован.");
    }
}