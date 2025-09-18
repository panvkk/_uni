import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;
import core.Utils;

public class Task9Laba2 {

    public int findRowWithMinMaxSeries(List<List<Integer>> matrix) {
        if (matrix == null || matrix.isEmpty()) {
            return -1;
        }

        int minMaxSeriesLength = Integer.MAX_VALUE;
        int resultRowIndex = -1;

        for (int i = 0; i < matrix.size(); i++) {
            List<Integer> row = matrix.get(i);
            if (row.isEmpty()) {
                continue;
            }

            int currentMaxSeriesLength = 1;
            int currentSeriesLength = 1;

            for (int j = 1; j < row.size(); j++) {
                if (row.get(j).equals(row.get(j - 1))) {
                    currentSeriesLength++;
                } else {
                    currentMaxSeriesLength = Math.max(currentMaxSeriesLength, currentSeriesLength);
                    currentSeriesLength = 1;
                }
            }
            currentMaxSeriesLength = Math.max(currentMaxSeriesLength, currentSeriesLength);

            if (currentMaxSeriesLength >= 2) {
                if (currentMaxSeriesLength < minMaxSeriesLength) {
                    minMaxSeriesLength = currentMaxSeriesLength;
                    resultRowIndex = i;
                }
            }
        }
        return resultRowIndex;
    }

    public static void main(String[] args) {
        Task9Laba2 task = new Task9Laba2();
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

            int resultRowIndex = task.findRowWithMinMaxSeries(matrix);
            if (resultRowIndex != -1) {
                System.out.println("---");
                System.out.printf("Строка, в которой длина максимальной серии минимальна, имеет индекс: %d\n", resultRowIndex);
                System.out.println("Содержимое строки: " + matrix.get(resultRowIndex));
            } else {
                System.out.println("Не удалось найти подходящую строку.");
            }
        } catch (FileNotFoundException e) {
            System.err.println("Ошибка: Файл не найден по пути " + filePath);
        }
    }
}

