package org.example;

import javafx.application.Application;
import javafx.application.Platform;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.ScrollPane;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.stage.Stage;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.atomic.AtomicInteger;

public class SortingApplication extends Application {
    private static final int INITIAL_ARRAY_SIZE = 50;
    private Integer[] initialArray;
    private static final String FILE_NAME = "array.txt";

    private final ExecutorService executor = Executors.newFixedThreadPool(3);
    private final AtomicInteger completedTasks = new AtomicInteger(0);
    private Button startButton;

    private VBox column1, column2, column3;

    @Override
    public void start(Stage stage) {
        initialArray = FileUtil.readArrayFromFile(FILE_NAME, INITIAL_ARRAY_SIZE);

        HBox columnsLayout = new HBox(10);
        columnsLayout.setPadding(new Insets(10));
        columnsLayout.setAlignment(Pos.TOP_CENTER);

        column1 = createSortingField("Поток 1: Пузырьком", Color.web("#4CAF50"));
        column2 = createSortingField("Поток 2: Вставками", Color.web("#2196F3"));
        column3 = createSortingField("Поток 3: Быстрая", Color.web("#FF9800"));

        columnsLayout.getChildren().addAll(column1, column2, column3);

        // Оборачиваем HBox в ScrollPane
        ScrollPane scrollPane = new ScrollPane();
        scrollPane.setContent(columnsLayout);
        scrollPane.setFitToWidth(true);
        // НЕ используем setFitToHeight(true), так как это мешает вертикальной прокрутке VBox
        scrollPane.setFitToHeight(false);

        startButton = new Button("Сортировать");
        startButton.setStyle("-fx-font-size: 18px; -fx-padding: 10 20;");
        startButton.setOnAction(e -> startSorting());

        BorderPane root = new BorderPane();
        root.setCenter(scrollPane);
        root.setBottom(startButton);
        BorderPane.setAlignment(startButton, Pos.CENTER);
        BorderPane.setMargin(startButton, new Insets(10));

        Scene scene = new Scene(root, 1100, 650);
        stage.setTitle("Многопоточная Визуализация Сортировки");
        stage.setScene(scene);

        stage.show();

        // Инициализация данных после отображения сцены
        Platform.runLater(() -> {
            double containerWidth = column1.getBoundsInLocal().getWidth() - 20;
            initializeData(column1, column2, column3, containerWidth);
        });
    }

    /**
     * Создает визуальный контейнер для одного потока, возвращаем PrefHeight.
     */
    private VBox createSortingField(String title, Color color) {
        VBox vBox = new VBox(5);
        vBox.setPadding(new Insets(10));
        vBox.setStyle("-fx-border-color: #ccc; -fx-border-width: 1;");
        vBox.setPrefWidth(350);

        Label titleLabel = new Label(title);
        titleLabel.setStyle("-fx-font-weight: bold; -fx-font-size: 14px;");

        Label statusLabel = new Label("Готов к запуску...");
        statusLabel.setMinHeight(40);
        String baseId = title.replaceAll("[^a-zA-ZА-Яа-я0-9]", "");
        statusLabel.setId(baseId);

        VBox dataContainer = new VBox();
        dataContainer.setPrefHeight(400);
        dataContainer.setStyle("-fx-background-color: #f5f5f5;");
        dataContainer.setId(baseId + "Container");

        vBox.getChildren().addAll(titleLabel, statusLabel, dataContainer);
        return vBox;
    }

    private void initializeData(VBox col1, VBox col2, VBox col3, double containerWidth) {
        Integer[] currentInitialArray = FileUtil.readArrayFromFile(FILE_NAME, INITIAL_ARRAY_SIZE);

        VBox container1 = (VBox) col1.lookup("#Поток1ПузырькомContainer");
        Label status1 = (Label) col1.lookup("#Поток1Пузырьком");
        VBox container2 = (VBox) col2.lookup("#Поток2ВставкамиContainer");
        Label status2 = (Label) col2.lookup("#Поток2Вставками");
        VBox container3 = (VBox) col3.lookup("#Поток3БыстраяContainer");
        Label status3 = (Label) col3.lookup("#Поток3Быстрая");

        status1.setText("Готов к запуску...");
        status2.setText("Готов к запуску...");
        status3.setText("Готов к запуску...");

        SortingData data1 = new SortingData(currentInitialArray, container1, containerWidth, Color.web("#4CAF50"));
        SortingData data2 = new SortingData(currentInitialArray, container2, containerWidth, Color.web("#2196F3"));
        SortingData data3 = new SortingData(currentInitialArray, container3, containerWidth, Color.web("#FF9800"));

        data1.updateVisualization(null, null);
        data2.updateVisualization(null, null);
        data3.updateVisualization(null, null);

        startButton.setUserData(new Object[]{data1, data2, data3, status1, status2, status3});
    }

    private void startSorting() {
        Platform.runLater(() -> {
            double containerWidth = column1.getBoundsInLocal().getWidth() - 20;
            initializeData(column1, column2, column3, containerWidth);
            runTasks();
        });
    }

    private void runTasks() {
        if (startButton.getUserData() == null) return;

        startButton.setDisable(true);
        completedTasks.set(0);

        Object[] userData = (Object[]) startButton.getUserData();
        SortingData taskData1 = (SortingData) userData[0];
        SortingData taskData2 = (SortingData) userData[1];
        SortingData taskData3 = (SortingData) userData[2];
        Label status1 = (Label) userData[3];
        Label status2 = (Label) userData[4];
        Label status3 = (Label) userData[5];

        Runnable completionCallback = () -> {
            if (completedTasks.incrementAndGet() == 3) {
                startButton.setDisable(false);
            }
        };

        executor.submit(new SortingTask(taskData1, "Пузырьком", status1, completionCallback));
        executor.submit(new SortingTask(taskData2, "Вставками", status2, completionCallback));
        executor.submit(new SortingTask(taskData3, "Быстрая", status3, completionCallback));
    }

    @Override
    public void stop() throws Exception {
        executor.shutdownNow();
        super.stop();
    }

    public static void main(String[] args) {
        launch(args);
    }
}