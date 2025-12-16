package org.example;

import com.squareup.moshi.*;
import com.squareup.moshi.Types;
import okio.BufferedSink;
import okio.BufferedSource;
import okio.Okio;

import java.io.File;
import java.io.IOException;
import java.lang.reflect.Type;
import java.util.ArrayList;
import java.util.List;

public class Main {
    public static void main(String[] args) throws IOException {
        Moshi moshi = new Moshi.Builder().build();
        Type type = Types.newParameterizedType(List.class, GradeBook.class);
        JsonAdapter<List<GradeBook>> adapter = moshi.adapter(type);

        // --- 1. Чтение из input.txt и создание GradeBook List ---
        List<GradeBook> studentsFromTxt = new ArrayList<>();
        File inputFileTxt = new File("input.txt");

        System.out.println("Чтение данных из input.txt...");
        try (BufferedSource source = Okio.buffer(Okio.source(inputFileTxt))) {
            while (true) {
                String line = source.readUtf8Line();
                if (line == null) break;
                try {
                    studentsFromTxt.add(GradeBook.fromTxtLine(line));
                } catch (IllegalArgumentException e) {
                    System.err.println("Ошибка парсинга строки: " + line + ". " + e.getMessage());
                }
            }
        } catch (IOException e) {
            System.err.println("Ошибка при чтении файла input.txt: " + e.getMessage());
            return;
        }

        // --- 2. Запись в temp.json (промежуточный файл) ---
        File outputFileJson = new File("temp.json");
        System.out.println("Формирование файла temp.json (из input.txt)...");
        try (BufferedSink sink = Okio.buffer(Okio.sink(outputFileJson))) {
            // Используем adapter для записи всего списка студентов
            adapter.indent("  ").toJson(sink, studentsFromTxt);
        }
        System.out.println("Файл temp.json сформирован.");

        // --- 3. Чтение из temp.json ---
        List<GradeBook> studentsFromJson;
        System.out.println("Чтение данных из temp.json...");
        try (BufferedSource source = Okio.buffer(Okio.source(outputFileJson))) {
            studentsFromJson = adapter.fromJson(source);
        } catch (IOException e) {
            System.err.println("Ошибка при чтении файла temp.json: " + e.getMessage());
            return;
        }

        if (studentsFromJson == null) {
            System.err.println("Ошибка: Список студентов, прочитанный из JSON, пуст.");
            return;
        }
        System.out.println("Данные успешно прочитаны из temp.json.");

        // --- 4. Запись в output.txt (все студенты в оригинальном текстовом формате) ---
        File outputFileTxt = new File("output.txt");
        System.out.println("Формирование файла output.txt (полный список)...");
        try (BufferedSink sink = Okio.buffer(Okio.sink(outputFileTxt))) {
            // Используем новый метод toOriginalTxtString() для восстановления формата, близкого к input.txt
            for (GradeBook gb : studentsFromJson) {
                sink.writeUtf8(gb.toOriginalTxtString() + "\n");
            }
        }
        System.out.println("Файл output.txt сформирован. Он должен быть идентичен input.txt (за исключением порядка, если он не гарантируется).");
    }
}