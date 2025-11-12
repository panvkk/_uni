package org.example;

import com.squareup.moshi.*;
import com.squareup.moshi.Types;
import okio.BufferedSink;
import okio.Okio;

import java.io.File;
import java.io.IOException;
import java.lang.reflect.Type;
import java.util.List;
import java.util.stream.Collectors;

public class Main {
    public static void main(String[] args) throws IOException {
        Moshi moshi = new Moshi.Builder().build();

        Type type = Types.newParameterizedType(List.class, GradeBook.class);
        JsonAdapter<List<GradeBook>> adapter = moshi.adapter(type);

        File inputFile = new File("input.json");
        List<GradeBook> gradeBooks = adapter.fromJson(Okio.buffer(Okio.source(inputFile)));

        if(gradeBooks == null) {
            System.out.println("No grade books found");
        }
        assert gradeBooks != null;
        List<GradeBook> excellent = gradeBooks.stream()
                .filter(GradeBook::isExcellent)
                .collect(Collectors.toList());

        File outputFile = new File("output.json");
        try (BufferedSink sink = Okio.buffer(Okio.sink(outputFile))) {
            adapter.indent("  ").toJson(sink, excellent);
        }

        System.out.println("Файл output.json сформирован.");
    }
}
