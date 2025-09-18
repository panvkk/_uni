import core.Utils;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Scanner;
import java.util.Set;

public class Task18Laba2 {

    public int countDistinctColumns(List<List<Integer>> matrix) {
        if (matrix == null || matrix.isEmpty()) {
            return 0;
        }

        int numRows = matrix.size();
        int numCols = 0;
        for (List<Integer> row : matrix) {
            if (!row.isEmpty()) {
                numCols = row.size();
                break;
            }
        }
        if (numCols == 0) {
            return 0;
        }

        int distinctColumnCount = 0;

        for (int j = 0; j < numCols; j++) {
            Set<Integer> uniqueElements = new HashSet<>();
            boolean isDistinctColumn = true;

            for (int i = 0; i < numRows; i++) {
                if (j >= matrix.get(i).size()) {
                    isDistinctColumn = false;
                    break;
                }
                int element = matrix.get(i).get(j);
                if (!uniqueElements.add(element)) {
                    isDistinctColumn = false;
                    break;
                }
            }

            if (isDistinctColumn) {
                distinctColumnCount++;
            }
        }
        return distinctColumnCount;
    }

    public static void main(String[] args) {
        Task18Laba2 task = new Task18Laba2();
        Utils utils = new Utils();
        String filePath = "matrix.txt";

        try {
            List<List<Integer>> matrix = utils.readMatrixFromFile(filePath);
            if (matrix.isEmpty()) {
                System.out.println("Файл пустой или содержит некорректные данные.");
                return;
            }

            System.out.println("Прочитанная матрица: ");
            for (List<Integer> row : matrix) {
                System.out.println(row);
            }

            int count = task.countDistinctColumns(matrix);
            System.out.println("---");
            System.out.printf("Число столбцов с попарно различными элементами: %d\n", count);

        } catch (FileNotFoundException e) {
            System.err.println("Ошибка: Файл не найден в " + filePath);
        }
    }
}