package org.example;

import java.util.*;
import java.util.regex.*;

/**
 * Единый класс, содержащий всю логику парсинга и вычислений.
 */
public class RpnLogic {

    // --- Модель данных (Tokens) на Records (JDK 17) ---
    public sealed interface Token permits NumberToken, OpToken, VarToken, ParenToken {}
    public record NumberToken(double value) implements Token { @Override public String toString() { return String.valueOf(value); } }
    public record VarToken(String name) implements Token { @Override public String toString() { return name; } }
    public record OpToken(String op) implements Token { @Override public String toString() { return op; } }
    public record ParenToken(char ch) implements Token { @Override public String toString() { return String.valueOf(ch); } }

    public static class CalcException extends Exception {
        public CalcException(String message) { super(message); }
    }

    // --- Lexer ---
    private static final Pattern TOKEN_PATTERN = Pattern.compile("\\s*(\\d+(?:\\.\\d+)?|[A-Za-zА-Яа-яёЁ][A-Za-z0-9_А-Яа-яёЁ]*|\\^|\\+|\\-|\\*|/|[()\\[\\]\\{\\}])");

    public static List<Token> tokenize(String input) throws CalcException {
        List<Token> tokens = new ArrayList<>();
        Matcher m = TOKEN_PATTERN.matcher(input);
        int idx = 0;
        while (idx < input.length()) {
            m.region(idx, input.length());
            if (!m.lookingAt()) throw new CalcException("Неизвестный символ в позиции " + idx);

            String s = m.group(1);
            if (s.matches("\\d+(?:\\.\\d+)?")) tokens.add(new NumberToken(Double.parseDouble(s)));
            else if (s.matches("[A-Za-zА-Яа-яёЁ].*")) tokens.add(new VarToken(s));
            else if ("+-*/^".contains(s)) tokens.add(new OpToken(s));
            else if ("()[]{}".contains(s)) tokens.add(new ParenToken(s.charAt(0)));

            idx = m.end();
        }
        return tokens;
    }

    // --- Parser (Shunting-yard) ---
    public static List<Token> infixToRPN(List<Token> tokens) throws CalcException {
        List<Token> out = new ArrayList<>();
        Deque<Token> stack = new ArrayDeque<>();
        Set<Character> opening = Set.of('(', '[', '{');
        Map<Character, Character> closeToOpen = Map.of(')', '(', ']', '[', '}', '{');

        Token prev = null;
        for (Token t : tokens) {
            if (t instanceof NumberToken || t instanceof VarToken) {
                out.add(t);
                prev = t;
            } else if (t instanceof OpToken opTok) {
                String op = opTok.op();
                // Логика унарного минуса
                boolean isUnary = "-".equals(op) && (prev == null || prev instanceof OpToken || (prev instanceof ParenToken p && opening.contains(p.ch())));
                OpToken curTok = isUnary ? new OpToken("u-") : opTok;

                while (!stack.isEmpty() && stack.peek() instanceof OpToken top) {
                    int pTop = precedence(top.op());
                    int pCur = precedence(curTok.op());
                    if (pTop > pCur || (pTop == pCur && !isRightAssociative(curTok.op()))) {
                        out.add(stack.pop());
                    } else break;
                }
                stack.push(curTok);
                prev = curTok;
            } else if (t instanceof ParenToken pTok) {
                if (opening.contains(pTok.ch())) {
                    stack.push(t);
                } else {
                    char expected = closeToOpen.get(pTok.ch());
                    boolean found = false;
                    while (!stack.isEmpty()) {
                        Token top = stack.pop();
                        if (top instanceof ParenToken pTop) {
                            if (pTop.ch() == expected) { found = true; break; }
                            else throw new CalcException("Несовпадающие скобки");
                        } else out.add(top);
                    }
                    if (!found) throw new CalcException("Несбалансированные скобки");
                }
                prev = t;
            }
        }
        while (!stack.isEmpty()) {
            Token t = stack.pop();
            if (t instanceof ParenToken) throw new CalcException("Несбалансированные скобки");
            out.add(t);
        }
        return out;
    }

    // --- Evaluator ---
    public static double evalRPN(List<Token> rpn, Map<String, Double> vars) throws CalcException {
        Deque<Double> stack = new ArrayDeque<>();
        for (Token t : rpn) {
            if (t instanceof NumberToken n) stack.push(n.value());
            else if (t instanceof VarToken v) {
                Double val = vars.get(v.name());
                if (val == null) throw new CalcException("Неизвестная переменная: " + v.name());
                stack.push(val);
            } else if (t instanceof OpToken o) {
                String op = o.op();
                if ("u-".equals(op)) {
                    if (stack.isEmpty()) throw new CalcException("Недостаточно операндов");
                    stack.push(-stack.pop());
                } else {
                    if (stack.size() < 2) throw new CalcException("Недостаточно операндов для " + op);
                    double b = stack.pop();
                    double a = stack.pop();
                    switch (op) {
                        case "+" -> stack.push(a + b);
                        case "-" -> stack.push(a - b);
                        case "*" -> stack.push(a * b);
                        case "/" -> { if (b == 0) throw new CalcException("Деление на ноль"); stack.push(a / b); }
                        case "^" -> stack.push(Math.pow(a, b));
                        default -> throw new CalcException("Неизвестный оператор: " + op);
                    }
                }
            }
        }
        if (stack.size() != 1) throw new CalcException("Ошибка вычисления");
        return stack.pop();
    }

    public static String rpnToString(List<Token> rpn) {
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < rpn.size(); i++) {
            sb.append(rpn.get(i).toString());
            if (i < rpn.size() - 1) sb.append(" ");
        }
        return sb.toString();
    }

    private static int precedence(String op) {
        return switch (op) {
            case "u-" -> 5;
            case "^" -> 4;
            case "*", "/" -> 3;
            case "+", "-" -> 2;
            default -> 0;
        };
    }

    private static boolean isRightAssociative(String op) {
        return "^".equals(op) || "u-".equals(op);
    }
}