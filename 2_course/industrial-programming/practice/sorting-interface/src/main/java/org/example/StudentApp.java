package org.example;

import javafx.application.Application;
import javafx.beans.property.SimpleDoubleProperty;
import javafx.beans.property.SimpleIntegerProperty;
import javafx.beans.property.SimpleLongProperty;
import javafx.beans.property.SimpleStringProperty;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.collections.transformation.FilteredList;
import javafx.collections.transformation.SortedList;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.stage.FileChooser;
import javafx.stage.Stage;

import java.io.File;
import java.io.IOException;
import java.nio.file.Path;
import java.util.List;
import java.util.function.Predicate;

public class StudentApp extends Application {

    // --- FXML поля ---
    @FXML private TextField searchNumber;
    @FXML private TextField searchName;
    @FXML private TextField searchGroup;
    @FXML private TextField searchGrade;

    @FXML private ComboBox<String> sortBox;
    @FXML private TableView<StudentData.Student> table;
    @FXML private TableColumn<StudentData.Student, Number> colNumber;
    @FXML private TableColumn<StudentData.Student, String> colName;
    @FXML private TableColumn<StudentData.Student, Number> colGroup;
    @FXML private TableColumn<StudentData.Student, Number> colGrade;

    // --- Данные ---
    private final ObservableList<StudentData.Student> masterData = FXCollections.observableArrayList();
    private FilteredList<StudentData.Student> filteredData;

    public static void main(String[] args) {
        launch(args);
    }

    @Override
    public void start(Stage stage) throws Exception {
        FXMLLoader loader = new FXMLLoader(getClass().getResource("/org/example/view.fxml"));
        loader.setController(this); // Устанавливаем текущий класс контроллером

        Scene scene = new Scene(loader.load(), 800, 600);
        stage.setTitle("Справочник студентов");
        stage.setScene(scene);
        stage.show();
    }

    @FXML
    private void initialize() {
        // 1. Настройка колонок (используем Records)
        colNumber.setCellValueFactory(data -> new SimpleLongProperty(data.getValue().number()));
        colName.setCellValueFactory(data -> new SimpleStringProperty(data.getValue().name()));
        colGroup.setCellValueFactory(data -> new SimpleIntegerProperty(data.getValue().group()));
        colGrade.setCellValueFactory(data -> new SimpleDoubleProperty(data.getValue().grade()));

        // 2. Настройка списка с фильтрацией и сортировкой
        filteredData = new FilteredList<>(masterData, p -> true);
        SortedList<StudentData.Student> sortedData = new SortedList<>(filteredData);
        sortedData.comparatorProperty().bind(table.comparatorProperty());
        table.setItems(sortedData);

        // 3. Слушатели на поля поиска
        searchNumber.textProperty().addListener((obs, o, n) -> updateFilter());
        searchName.textProperty().addListener((obs, o, n) -> updateFilter());
        searchGroup.textProperty().addListener((obs, o, n) -> updateFilter());
        searchGrade.textProperty().addListener((obs, o, n) -> updateFilter());

        // 4. Инициализация сортировки
        sortBox.getItems().addAll("По номеру", "По имени", "По группе", "По баллу");
        sortBox.setValue("По номеру");
    }

    @FXML
    private void onLoad() {
        FileChooser fc = new FileChooser();
        fc.setTitle("Открыть список студентов");
        fc.getExtensionFilters().add(new FileChooser.ExtensionFilter("Текстовые файлы", "*.txt", "*.csv"));
        File file = fc.showOpenDialog(table.getScene().getWindow());

        if (file != null) {
            try {
                List<StudentData.Student> loaded = StudentData.loadFromFile(file.toPath());
                masterData.setAll(loaded);
            } catch (IOException e) {
                showAlert("Ошибка чтения", e.getMessage());
            }
        }
    }

    @FXML
    private void onApplySort() {
        String mode = sortBox.getValue();
        if (mode == null) return;

        table.getSortOrder().clear();
        switch (mode) {
            case "По номеру" -> table.getSortOrder().add(colNumber);
            case "По имени" -> table.getSortOrder().add(colName);
            case "По группе" -> table.getSortOrder().add(colGroup);
            case "По баллу" -> table.getSortOrder().add(colGrade);
        }
        // Принудительная сортировка по первой колонке в sortOrder
        if (!table.getSortOrder().isEmpty()) {
            table.sort();
        }
    }

    private void updateFilter() {
        String numQ = searchNumber.getText().toLowerCase().trim();
        String nameQ = searchName.getText().toLowerCase().trim();
        String groupQ = searchGroup.getText().toLowerCase().trim();
        String gradeQ = searchGrade.getText().toLowerCase().trim();

        filteredData.setPredicate(student -> {
            if (!numQ.isEmpty() && !String.valueOf(student.number()).contains(numQ)) return false;
            if (!nameQ.isEmpty() && !student.name().toLowerCase().contains(nameQ)) return false;
            if (!groupQ.isEmpty() && !String.valueOf(student.group()).contains(groupQ)) return false;
            if (!gradeQ.isEmpty() && !String.valueOf(student.grade()).contains(gradeQ)) return false;
            return true;
        });
    }

    private void showAlert(String title, String content) {
        Alert alert = new Alert(Alert.AlertType.ERROR);
        alert.setTitle("Ошибка");
        alert.setHeaderText(title);
        alert.setContentText(content);
        alert.showAndWait();
    }
}