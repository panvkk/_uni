package org.example;

import com.google.gson.Gson;
import com.google.gson.reflect.TypeToken;
import javafx.application.Application;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.input.Clipboard;
import javafx.scene.input.ClipboardContent;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import javafx.stage.Stage;

import java.io.InputStream;
import java.io.InputStreamReader;
import java.lang.reflect.Type;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class OlympiadRewardApp extends Application {

    private static final double CONCERT_COST = 5000.00;
    private static final double DISCOUNT_RATE = 0.10;

    private final Map<String, ObservableList<Gift>> giftData = new HashMap<>();

    private final Map<String, CheckBox> congratulatorCheckBoxes = new HashMap<>();
    private final Map<String, VBox> giftSelectorVBoxes = new HashMap<>();
    private final Map<String, ComboBox<Gift>> giftComboBoxes = new HashMap<>();

    private RadioButton concertYesRadio;
    private RadioButton concertNoRadio;
    private CheckBox regularCustomerCheck;

    private Label totalCostLabel;
    private TextArea orderDetailsArea;
    private Button copyButton;

    public static void main(String[] args) {
        launch(args);
    }

    @Override
    public void start(Stage primaryStage) {
        primaryStage.setTitle("Расчет наград олимпиады");

        loadGiftData();

        VBox root = new VBox(10);
        root.setPadding(new Insets(15));

        root.getChildren().add(new Label("a. Выберите Поздравителя(-ей):"));
        VBox congratulatorsBox = new VBox(5);
        congratulatorsBox.setPadding(new Insets(0, 0, 0, 10));

        for (String name : giftData.keySet()) {
            CheckBox cb = new CheckBox(name);
            congratulatorCheckBoxes.put(name, cb);
            congratulatorsBox.getChildren().add(cb);
        }
        root.getChildren().add(congratulatorsBox);

        root.getChildren().add(new Label("b. Выберите Подарки:"));
        VBox giftSelectionPane = new VBox(8);
        giftSelectionPane.setPadding(new Insets(0, 0, 0, 10));

        for (String name : giftData.keySet()) {
            Label giftLabel = new Label("Подарок от: " + name);
            ComboBox<Gift> giftBox = new ComboBox<>(giftData.get(name));
            giftBox.setPromptText("Выберите подарок");
            giftBox.valueProperty().addListener(e -> updateCostAndDetails());

            VBox selectorBox = new VBox(3, giftLabel, giftBox);
            selectorBox.setVisible(false);
            selectorBox.setManaged(false);

            giftSelectorVBoxes.put(name, selectorBox);
            giftComboBoxes.put(name, giftBox);
            giftSelectionPane.getChildren().add(selectorBox);
        }
        root.getChildren().add(giftSelectionPane);

        root.getChildren().add(new Label(String.format("c. Нужен ли Концерт? (Стоимость: %.2f руб.)", CONCERT_COST)));
        ToggleGroup concertGroup = new ToggleGroup();
        concertYesRadio = new RadioButton("Да");
        concertYesRadio.setToggleGroup(concertGroup);
        concertYesRadio.setOnAction(e -> updateCostAndDetails());

        concertNoRadio = new RadioButton("Нет");
        concertNoRadio.setToggleGroup(concertGroup);
        concertNoRadio.setSelected(true);
        concertNoRadio.setOnAction(e -> updateCostAndDetails());

        VBox concertBox = new VBox(5, concertYesRadio, concertNoRadio);
        concertBox.setPadding(new Insets(0, 0, 0, 10));
        root.getChildren().add(concertBox);

        root.getChildren().add(new Label("d. Постоянный клиент?"));
        regularCustomerCheck = new CheckBox("Я постоянный клиент (скидка 10%)");
        regularCustomerCheck.setOnAction(e -> updateCostAndDetails());
        VBox customerBox = new VBox(5, regularCustomerCheck);
        customerBox.setPadding(new Insets(0, 0, 0, 10));
        root.getChildren().add(customerBox);

        root.getChildren().add(new Separator());
        root.getChildren().add(new Label("e. Итог:"));

        totalCostLabel = new Label("Общая стоимость: 0.00 руб.");
        totalCostLabel.setStyle("-fx-font-weight: bold; -fx-font-size: 14px;");

        copyButton = new Button("Копировать состав заказа");
        copyButton.setOnAction(e -> copyOrderDetailsToClipboard());

        HBox buttonBox = new HBox(10, copyButton);
        buttonBox.setAlignment(Pos.CENTER_RIGHT);

        orderDetailsArea = new TextArea();
        orderDetailsArea.setEditable(false);
        orderDetailsArea.setPrefHeight(150);
        orderDetailsArea.setWrapText(true);

        root.getChildren().addAll(totalCostLabel, copyButton , orderDetailsArea);

        for (String name : congratulatorCheckBoxes.keySet()) {
            CheckBox cb = congratulatorCheckBoxes.get(name);
            VBox selector = giftSelectorVBoxes.get(name);
            ComboBox<Gift> giftBox = giftComboBoxes.get(name);

            cb.selectedProperty().addListener((obs, oldVal, newVal) -> {
                selector.setVisible(newVal);
                selector.setManaged(newVal);
                if (!newVal) {
                    giftBox.setValue(null);
                }
                updateCostAndDetails();
            });
        }

        updateCostAndDetails();
        Scene scene = new Scene(root, 450, 700);
        primaryStage.setScene(scene);
        primaryStage.show();
    }

    private void loadGiftData() {
        Gson gson = new Gson();

        InputStream inputStream = getClass().getClassLoader().getResourceAsStream("gifts.json");

        if (inputStream == null) {
            System.err.println("FATAL ERROR: Resource file gifts.json not found in resources folder.");
            return;
        }

        Type type = new TypeToken<Map<String, List<Gift>>>(){}.getType();

        try (InputStreamReader reader = new InputStreamReader(inputStream)) {
            Map<String, List<Gift>> parsedData = gson.fromJson(reader, type);

            for (Map.Entry<String, List<Gift>> entry : parsedData.entrySet()) {
                ObservableList<Gift> observableList = FXCollections.observableArrayList(entry.getValue());
                giftData.put(entry.getKey(), observableList);
            }

        } catch (Exception e) {
            System.err.println("Error reading or parsing gifts.json: " + e.getMessage());
            e.printStackTrace();
        }
    }

    private void updateCostAndDetails() {
        double total = 0;
        StringBuilder details = new StringBuilder("--- Состав заказа ---\n");

        boolean someoneSelected = false;
        for (String name : congratulatorCheckBoxes.keySet()) {
            if (congratulatorCheckBoxes.get(name).isSelected()) {
                someoneSelected = true;
                details.append("\nПоздравитель: ").append(name).append("\n");

                Gift selectedGift = giftComboBoxes.get(name).getValue();
                if (selectedGift != null) {
                    total += selectedGift.getPrice();
                    details.append(String.format("  - Подарок: %s (%.2f руб.)\n",
                            selectedGift.getName(), selectedGift.getPrice()));
                } else {
                    details.append("  - Подарок: не выбран\n");
                }
            }
        }
        if (!someoneSelected) {
            details.append("\n(Поздравители не выбраны)\n");
        }

        if (concertYesRadio.isSelected()) {
            total += CONCERT_COST;
            details.append(String.format("\nУслуга: Концерт (%.2f руб.)\n", CONCERT_COST));
        }

        if (regularCustomerCheck.isSelected()) {
            double discountAmount = total * DISCOUNT_RATE;
            total -= discountAmount;
            details.append(String.format("\nСкидка (10%%): -%.2f руб.\n", discountAmount));
        }

        totalCostLabel.setText(String.format("Общая стоимость: %.2f руб.", total));
        orderDetailsArea.setText(details.toString());
    }

    private void copyOrderDetailsToClipboard() {
        final Clipboard clipboard = Clipboard.getSystemClipboard();
        final ClipboardContent content = new ClipboardContent();
        content.putString(orderDetailsArea.getText());
        clipboard.setContent(content);

        String originalText = copyButton.getText();
        copyButton.setText("Скопировано!");

        javafx.animation.PauseTransition pause = new javafx.animation.PauseTransition(
                javafx.util.Duration.seconds(1)
        );
        pause.setOnFinished(e -> copyButton.setText(originalText));
        pause.play();
    }
}