package org.example;

import jakarta.xml.bind.JAXBContext;
import jakarta.xml.bind.Marshaller;
import jakarta.xml.bind.Unmarshaller;
import javafx.application.Application;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.collections.transformation.FilteredList;
import javafx.geometry.Insets;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.control.cell.PropertyValueFactory;
import javafx.scene.layout.*;
import javafx.stage.Stage;
import org.xml.sax.SAXException;

import javax.xml.XMLConstants;
import javax.xml.validation.Schema;
import javax.xml.validation.SchemaFactory;
import java.io.File;
import java.net.URL;

public class Main extends Application {

    private TableView<Book> table = new TableView<>();
    private ObservableList<Book> bookData = FXCollections.observableArrayList();
    private File xmlFile;
    private File xsdFile;

    public static void main(String[] args) {
        launch(args);
    }

    @Override
    public void start(Stage primaryStage) {
        primaryStage.setTitle("Library XML Manager");

        // Подготовка файлов
        loadFiles();

        // 4.2 Загрузка и валидация при старте
        loadAndValidateXML();

        // Настройка таблицы (3.2.1)
        setupTable();

        // Панель управления (Кнопки и Поиск)
        VBox controlPanel = createControlPanel();

        BorderPane root = new BorderPane();
        root.setCenter(table);
        root.setRight(controlPanel);

        Scene scene = new Scene(root, 1000, 600);
        primaryStage.setScene(scene);
        primaryStage.show();
    }

    private void loadFiles() {
        URL xsdUrl = getClass().getClassLoader().getResource("library.xsd");

        if (xsdUrl != null) {
            try {
                xmlFile = new File("data/library.xml");
                xsdFile = new File(xsdUrl.toURI());
            } catch (Exception e) {
                showAlert("Error", "Could not load file paths.");
            }
        }
    }

    // 4.2 Проверка на соответствие схеме при загрузке
    private void loadAndValidateXML() {
        try {
            JAXBContext context = JAXBContext.newInstance(Library.class);
            Unmarshaller unmarshaller = context.createUnmarshaller();

            // Установка XSD схемы для валидации
            SchemaFactory sf = SchemaFactory.newInstance(XMLConstants.W3C_XML_SCHEMA_NS_URI);
            Schema schema = sf.newSchema(xsdFile);
            unmarshaller.setSchema(schema);

            // Чтение
            Library library = (Library) unmarshaller.unmarshal(xmlFile);
            bookData.setAll(library.getBooks());

            System.out.println("XML loaded and validated successfully.");

        } catch (SAXException e) {
            showAlert("Validation Error", "XML is not valid against XSD: " + e.getMessage());
        } catch (Exception e) {
            showAlert("Error", "Error loading XML: " + e.getMessage());
            e.printStackTrace();
        }
    }

    private void saveXML() {
        try {
            Library lib = new Library();
            lib.setBooks(bookData);

            JAXBContext context = JAXBContext.newInstance(Library.class);
            Marshaller marshaller = context.createMarshaller();
            marshaller.setProperty(Marshaller.JAXB_FORMATTED_OUTPUT, true);
            marshaller.marshal(lib, xmlFile);

            showAlert("Success", "Data saved to XML.");
        } catch (Exception e) {
            showAlert("Error", "Could not save XML: " + e.getMessage());
        }
    }

    private void setupTable() {
        TableColumn<Book, Integer> idCol = new TableColumn<>("ID");
        idCol.setCellValueFactory(new PropertyValueFactory<>("id"));

        TableColumn<Book, String> titleCol = new TableColumn<>("Title");
        titleCol.setCellValueFactory(new PropertyValueFactory<>("title"));

        TableColumn<Book, String> authorCol = new TableColumn<>("Author");
        authorCol.setCellValueFactory(new PropertyValueFactory<>("author"));

        TableColumn<Book, Integer> yearCol = new TableColumn<>("Year");
        yearCol.setCellValueFactory(new PropertyValueFactory<>("year"));

        TableColumn<Book, String> genreCol = new TableColumn<>("Genre");
        genreCol.setCellValueFactory(new PropertyValueFactory<>("genre"));

        TableColumn<Book, Double> priceCol = new TableColumn<>("Price");
        priceCol.setCellValueFactory(new PropertyValueFactory<>("price"));

        TableColumn<Book, Integer> totalCol = new TableColumn<>("Total");
        totalCol.setCellValueFactory(new PropertyValueFactory<>("totalCopies"));

        TableColumn<Book, Integer> availCol = new TableColumn<>("Available");
        availCol.setCellValueFactory(new PropertyValueFactory<>("availableCopies"));

        table.getColumns().addAll(idCol, titleCol, authorCol, yearCol, genreCol, priceCol, totalCol, availCol);
        table.setItems(bookData);
    }

    private VBox createControlPanel() {
        VBox vbox = new VBox(10);
        vbox.setPadding(new Insets(10));
        vbox.setPrefWidth(300);

        Label lblMenu = new Label("User Menu");
        lblMenu.setStyle("-fx-font-weight: bold; -fx-font-size: 14px;");

        // 3.2.3 Поиск
        TextField searchField = new TextField();
        searchField.setPromptText("Search (Author, Year, Genre)...");

        FilteredList<Book> filteredData = new FilteredList<>(bookData, p -> true);
        searchField.textProperty().addListener((observable, oldValue, newValue) -> {
            filteredData.setPredicate(book -> {
                if (newValue == null || newValue.isEmpty()) return true;
                String lowerCaseFilter = newValue.toLowerCase();

                if (book.getTitle().toLowerCase().contains(lowerCaseFilter)) return true;
                if (book.getAuthor().toLowerCase().contains(lowerCaseFilter)) return true;
                if (String.valueOf(book.getYear()).contains(lowerCaseFilter)) return true;
                if (book.getGenre().toLowerCase().contains(lowerCaseFilter)) return true;
                return false;
            });
        });
        table.setItems(filteredData);

        // 3.2.2 Добавление новой книги
        Button btnAdd = new Button("Add New Book");
        btnAdd.setMaxWidth(Double.MAX_VALUE);
        btnAdd.setOnAction(e -> showAddBookDialog());

        // 3.2.4 Переоценка (изменение цены)
        Button btnChangePrice = new Button("Change Price");
        btnChangePrice.setMaxWidth(Double.MAX_VALUE);
        btnChangePrice.setOnAction(e -> changePriceAction());

        // 3.2.5 (или 3.2.4 в задании) Эмуляция выдачи
        Button btnCheckout = new Button("Issue Book (Checkout)");
        btnCheckout.setMaxWidth(Double.MAX_VALUE);
        btnCheckout.setOnAction(e -> checkoutBookAction());

        Button btnSave = new Button("Save Changes to XML");
        btnSave.setMaxWidth(Double.MAX_VALUE);
        btnSave.setOnAction(e -> saveXML());

        vbox.getChildren().addAll(lblMenu, new Label("Search:"), searchField,
                new Separator(), btnAdd, btnChangePrice, btnCheckout,
                new Separator(), btnSave);
        return vbox;
    }

    // Логика добавления книги
    private void showAddBookDialog() {
        Dialog<Book> dialog = new Dialog<>();
        dialog.setTitle("Add Book");
        dialog.setHeaderText("Enter book details");

        ButtonType loginButtonType = new ButtonType("Add", ButtonBar.ButtonData.OK_DONE);
        dialog.getDialogPane().getButtonTypes().addAll(loginButtonType, ButtonType.CANCEL);

        GridPane grid = new GridPane();
        grid.setHgap(10);
        grid.setVgap(10);
        grid.setPadding(new Insets(20, 150, 10, 10));

        TextField title = new TextField(); title.setPromptText("Title");
        TextField author = new TextField(); author.setPromptText("Author");
        TextField year = new TextField(); year.setPromptText("Year");
        TextField genre = new TextField(); genre.setPromptText("Genre");
        TextField price = new TextField(); price.setPromptText("Price");
        TextField total = new TextField(); total.setPromptText("Total Copies");

        grid.add(new Label("Title:"), 0, 0); grid.add(title, 1, 0);
        grid.add(new Label("Author:"), 0, 1); grid.add(author, 1, 1);
        grid.add(new Label("Year:"), 0, 2); grid.add(year, 1, 2);
        grid.add(new Label("Genre:"), 0, 3); grid.add(genre, 1, 3);
        grid.add(new Label("Price:"), 0, 4); grid.add(price, 1, 4);
        grid.add(new Label("Total:"), 0, 5); grid.add(total, 1, 5);

        dialog.getDialogPane().setContent(grid);

        dialog.setResultConverter(dialogButton -> {
            if (dialogButton == loginButtonType) {
                try {
                    int nextId = bookData.stream().mapToInt(Book::getId).max().orElse(0) + 1;
                    return new Book(
                            nextId,
                            title.getText(),
                            author.getText(),
                            Integer.parseInt(year.getText()),
                            genre.getText(),
                            Double.parseDouble(price.getText()),
                            Integer.parseInt(total.getText()),
                            Integer.parseInt(total.getText()) // Изначально все доступны
                    );
                } catch (NumberFormatException ex) {
                    showAlert("Error", "Invalid number format");
                    return null;
                }
            }
            return null;
        });

        dialog.showAndWait().ifPresent(book -> {
            bookData.add(book);
            table.refresh();
        });
    }

    // Логика изменения цены
    private void changePriceAction() {
        Book selected = table.getSelectionModel().getSelectedItem();
        if (selected == null) {
            showAlert("Warning", "Select a book first.");
            return;
        }

        TextInputDialog dialog = new TextInputDialog(String.valueOf(selected.getPrice()));
        dialog.setTitle("Change Price");
        dialog.setHeaderText("New price for: " + selected.getTitle());
        dialog.setContentText("Price:");

        dialog.showAndWait().ifPresent(result -> {
            try {
                double newPrice = Double.parseDouble(result);
                selected.setPrice(newPrice);
                table.refresh(); // Обновить таблицу
            } catch (NumberFormatException e) {
                showAlert("Error", "Invalid price format");
            }
        });
    }

    // Логика выдачи книги (уменьшение availableCopies)
    private void checkoutBookAction() {
        Book selected = table.getSelectionModel().getSelectedItem();
        if (selected == null) {
            showAlert("Warning", "Select a book first.");
            return;
        }

        if (selected.getAvailableCopies() > 0) {
            selected.setAvailableCopies(selected.getAvailableCopies() - 1);
            table.refresh();
            showAlert("Success", "Book issued to reader. Copies left: " + selected.getAvailableCopies());
        } else {
            showAlert("Error", "No copies available!");
        }
    }

    private void showAlert(String title, String content) {
        Alert alert = new Alert(Alert.AlertType.INFORMATION);
        alert.setTitle(title);
        alert.setContentText(content);
        alert.showAndWait();
    }
}