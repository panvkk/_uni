package org.example;

import javafx.application.Application;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.stage.Stage;
import java.util.*;

public class CalculatorApp extends Application {

    // --- Controller Fields ---
    @FXML private TextField inputField;
    @FXML private TextField rpnField;
    @FXML private TextField resultField;
    @FXML private Button computeBtn; // Ссылки нужны для FXML injection, даже если не используются явно
    @FXML private Button clearBtn;

    // --- Main Entry Point ---
    public static void main(String[] args) {
        launch(args);
    }

    // --- Application Start ---
    @Override
    public void start(Stage stage) throws Exception {
        FXMLLoader loader = new FXMLLoader(getClass().getResource("/org/example/view.fxml"));

        // Устанавливаем текущий класс как контроллер
        loader.setController(this);

        Scene scene = new Scene(loader.load(), 700, 220);
        stage.setTitle("RPN Calculator");
        stage.setScene(scene);
        stage.show();
    }

    // --- Controller Logic ---
    @FXML
    private void initialize() {
        // Кнопки привязаны через onAction в FXML или можно здесь:
        // computeBtn.setOnAction(e -> onCompute());
    }

    @FXML
    private void onCompute() {
        String expr = (inputField.getText() == null) ? "" : inputField.getText().trim();
        if (expr.isEmpty()) {
            resultField.setText("Введите выражение");
            return;
        }

        try {
            // 1. Lexing
            List<RpnLogic.Token> tokens = RpnLogic.tokenize(expr);

            // 2. Parsing
            List<RpnLogic.Token> rpn = RpnLogic.infixToRPN(tokens);
            rpnField.setText(RpnLogic.rpnToString(rpn));

            // 3. Evaluation loop
            Map<String, Double> vars = new HashMap<>();
            while (true) {
                try {
                    double res = RpnLogic.evalRPN(rpn, vars);
                    resultField.setText(String.valueOf(res));
                    break;
                } catch (RpnLogic.CalcException ex) {
                    if (ex.getMessage() != null && ex.getMessage().startsWith("Неизвестная переменная: ")) {
                        String varName = ex.getMessage().substring("Неизвестная переменная: ".length());
                        Optional<Double> val = askVariable(varName);
                        if (val.isPresent()) {
                            vars.put(varName, val.get());
                        } else {
                            resultField.setText("Отмена ввода");
                            break;
                        }
                    } else {
                        throw ex; // Пробрасываем другие ошибки
                    }
                }
            }
        } catch (RpnLogic.CalcException e) {
            resultField.setText("Ошибка: " + e.getMessage());
        } catch (Exception e) {
            resultField.setText("Сбой: " + e.getMessage());
            e.printStackTrace();
        }
    }

    @FXML
    private void onClear() {
        inputField.clear();
        rpnField.clear();
        resultField.clear();
    }

    private Optional<Double> askVariable(String name) {
        TextInputDialog dlg = new TextInputDialog();
        dlg.setTitle("Ввод переменной");
        dlg.setHeaderText("Значение для " + name);
        return dlg.showAndWait().flatMap(s -> {
            try { return Optional.of(Double.parseDouble(s.trim())); }
            catch (NumberFormatException e) { return Optional.empty(); }
        });
    }
}