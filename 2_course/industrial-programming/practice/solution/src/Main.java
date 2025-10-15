import java.util.*;
import java.io.*;

public class Main {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        String pStr = scanner.next();

        char[] pChars = pStr.toCharArray();
        int n = pChars.length;

        Set<String> validPasswords = new HashSet<>();

        for (int i = 0; i < n; i++) {
            char originalChar = pChars[i];
            int originalDigit = Character.getNumericValue(originalChar);

            for (int newDigit = 0; newDigit <= 9; newDigit++) {

                if (newDigit == originalDigit) {
                    continue;
                }

                if (i == 0 && newDigit == 0) {
                    continue;
                }

                pChars[i] = Character.forDigit(newDigit, 10);
                String newPStr = new String(pChars);

                if (isDivisibleByNine(newPStr)) {
                    validPasswords.add(newPStr);
                }
            }

            pChars[i] = originalChar;
        }

        for (String password : validPasswords) {
            System.out.println(password);
        }

        scanner.close();
    }

    private static boolean isDivisibleByNine(String numberStr) {
        int sumOfDigits = 0;
        for (char c : numberStr.toCharArray()) {
            sumOfDigits += Character.getNumericValue(c);
        }
        return sumOfDigits % 9 == 0;
    }
}