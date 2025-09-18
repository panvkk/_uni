package core;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class Utils {
    public List<List<Integer>> readMatrixFromFile(String filePath) throws FileNotFoundException {
        List<List<Integer>> matrix = new ArrayList<>();
        File file = new File(filePath);
        Scanner scanner = new Scanner(file);

        while (scanner.hasNextLine()) {
            String line = scanner.nextLine();
            if (line.trim().isEmpty()) {
                continue;
            }
            List<Integer> row = new ArrayList<>();
            Scanner lineScanner = new Scanner(line);
            while (lineScanner.hasNextInt()) {
                row.add(lineScanner.nextInt());
            }
            lineScanner.close();
            if (!row.isEmpty()) {
                matrix.add(row);
            }
        }
        scanner.close();
        return matrix;
    }
}
