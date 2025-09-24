import java.util.Scanner;

public class TaskLaba3 {
    public static void main(String[] args) {
        Scanner input = new Scanner(System.in);

        System.out.println("Введите текст: ");
        StringBuilder builder = new StringBuilder();

        String currentString = input.nextLine();

        while(!currentString.isEmpty()) {
            builder.append(currentString).append(System.lineSeparator());
            currentString = input.nextLine();
        }

        String inString = builder.toString();

        System.out.println(inString);

        System.out.println("Введите 2 символа, между которыми удалить текст: ");
        String symbols =  input.nextLine();
        char firstSymbol = symbols.charAt(0);
        char secondSymbol = symbols.charAt(1);

        int firstSymbolIndex = inString.indexOf(firstSymbol);
        int secondSymbolIndex = inString.lastIndexOf(secondSymbol);

        if(firstSymbolIndex == -1 || secondSymbolIndex == -1 || firstSymbolIndex > secondSymbolIndex) {
            System.out.println("Невозможно удалить текст между символами.");
        } else {
            StringBuilder newBuilder = new StringBuilder(inString);
            newBuilder.delete(firstSymbolIndex, secondSymbolIndex + 1);

            String resultString = newBuilder.toString();
            System.out.println(resultString);
        }
        input.close();
    }
}
