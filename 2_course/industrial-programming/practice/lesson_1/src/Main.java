import java.text.NumberFormat;
import java.util.HashSet;
import java.util.Scanner;
import java.util.Set;

public class Main {
    public static void main(String[] args) {
        Main tasks = new Main();
        tasks.secondTask();
    }

    private void secondTask() {
        Scanner input = new Scanner(System.in);


        System.out.println("Введите размерность матрицы: ");
        int n = input.nextInt();

        int[][] matrix = new int[n][n];
        System.out.println("Введите элементы матрицы по строкам.");
        for(int i = 0; i < n; i++) {
            System.out.printf("Строка %d: \n", i + 1);
            for(int j = 0; j < n; j++) {
                matrix[i][j] = input.nextInt();
            }
        }
        System.out.println("Введённая матрица: ");
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < n; j++) {
                System.out.print(matrix[i][j] + " ");
            }
            System.out.print("\n");
        }

//        System.out.println("Номера столбцов без повторений элементов: ");
//        for(int i = 0; i < n; i++) {
//            Set<Integer> set = new HashSet<>();
//            set.add(matrix[0][i]);
//            boolean isCurrentLine = true;
//            for(int j = 1; j < n; j++) {
//                if(!set.contains(matrix[j][i])) {
//                    set.add(matrix[j][i]);
//                } else {
//                    isCurrentLine = false;
//                }
//            }
//            if(isCurrentLine) System.out.print((i+1) + " ");
//        }

        Set<Set<Integer>> set = new HashSet<>();

        for(int i = 0; i < n; i++) {
            Set<Integer> currentSet = new HashSet<>();
            for(int j = 0; j < n; j++) {
                currentSet.add(matrix[i][j]);
            }
            set.add(currentSet);
        }
        System.out.println("Размер максимального множества различающихся строк: " + set.size());


        //        int minSequence = 0;
//        for(int i = 0; i < n; i++) {
//            int currSequenceLength = 1;
//            int minSequenceLength = 0;
//            int prevValue = matrix[i][0];
//            for(int j = 1; j < n; j++) {
//                int currValue = matrix[i][j];
//                if(prevValue == currValue) {
//                    currSequenceLength++;
//                } else {
//                    prevValue = currValue;
//                    minSequenceLength = Math.min(minSequenceLength, currSequenceLength);
//                }
//            }
//            minSequenceLength = Math.min(minSequenceLength, currSequenceLength);
//            if(lineWithMinSequence )
//        }
//        System.out.print("Минимальная последовательность чисел в строке под номером: ");

    }

    private void firstTask() {
        Scanner input = new Scanner(System.in);


        System.out.println("Введите X в радианах: ");
        float x = input.nextFloat();
        x = x % 6.28f;

        System.out.println("Введите K (точность): ");
        int k = input.nextInt();

        double result = calculateSin(x,k);

        NumberFormat formatter =  NumberFormat.getInstance();
        formatter.setMaximumFractionDigits(5);

        System.out.println("Результат программы: ");
        System.out.printf(formatter.format(result));

        System.out.println("\nРезультат встроенной функции: ");
        System.out.printf(formatter.format(Math.sin(x)));
    }

     private double calculateSin(float x, int k) {
         double epsilon = Math.pow(10, -k);
        double previous = 2 ;
        double sin = 0;
        for(int i = 0; Math.abs(previous) > epsilon; i++) {
            double current = (Math.pow(-1, i) * Math.pow(x, 2*i + 1))/ factorial(2*i + 1);
            sin += current;
            previous = current;
        }
        return sin;
    }

    private long factorial(int n) {
        if(n == 0) {
            return 1;
        } else {
            return n * factorial(n - 1);
        }
    }
}