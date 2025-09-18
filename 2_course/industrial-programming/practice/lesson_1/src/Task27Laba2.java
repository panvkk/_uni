import core.Utils;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Scanner;
import java.util.Set;

public class Task27Laba2 {

    public int countMaxDissimilarRows(List<List<Integer>> matrix) {
        if (matrix == null || matrix.isEmpty()) {
            return 0;
        }

        Set<Set<Integer>> uniqueSets = new HashSet<>();

        for (List<Integer> row : matrix) {
            Set<Integer> rowSet = new HashSet<>(row);
            if (!rowSet.isEmpty()) {
                uniqueSets.add(rowSet);
            }
        }

        return uniqueSets.size();
    }

    public static void main(String[] args) {
        Task27Laba2 task = new Task27Laba2();
        Utils utils = new Utils();
        String filePath = "matrix.txt";

        try {
            List<List<Integer>> matrix = utils.readMatrixFromFile(filePath);
            if (matrix.isEmpty()) {
                System.out.println("Файл пуст или не содержит корректных данных.");
                return;
            }

            System.out.println("Прочитанная матрица:");
            for (List<Integer> row : matrix) {
                System.out.println(row);
            }

            int count = task.countMaxDissimilarRows(matrix);
            System.out.println("---");
            System.out.printf("Количество строк в максимальном множестве попарно непохожих строк: %d\n", count);

        } catch (FileNotFoundException e) {
            System.err.println("Ошибка: Файл не найден по пути " + filePath);
        }
    }
}