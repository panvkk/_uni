import java.text.NumberFormat;
import java.util.Scanner;

public class TaskLaba1 {
    public static void main(String[] args) {
        Scanner input = new Scanner(System.in);
        TaskLaba1 taskLaba1 = new TaskLaba1();


        System.out.println("Введите X в радианах: ");
        float x = input.nextFloat();
        x = x % 6.28f;

        System.out.println("Введите K (точность): ");
        int k = input.nextInt();

        double result = taskLaba1.calculateSin(x, k);

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
