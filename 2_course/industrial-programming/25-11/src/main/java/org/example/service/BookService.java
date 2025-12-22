package org.example.service;


import jakarta.annotation.PostConstruct;
import jakarta.xml.bind.JAXBContext;
import jakarta.xml.bind.Marshaller;
import jakarta.xml.bind.Unmarshaller;
import org.example.model.Book;
import org.example.model.LibraryWrapper;
import org.springframework.stereotype.Service;

import java.io.File;
import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

@Service
public class BookService {
    private final String FILE_PATH = "library.xml"; // Файл должен лежать в корне проекта
    private List<Book> books = new ArrayList<>();

    @PostConstruct
    public void init() {
        loadFromXml();
    }

    private void loadFromXml() {
        try {
            File file = new File(FILE_PATH);
            if (file.exists()) {
                JAXBContext context = JAXBContext.newInstance(LibraryWrapper.class);
                Unmarshaller unmarshaller = context.createUnmarshaller();
                LibraryWrapper wrapper = (LibraryWrapper) unmarshaller.unmarshal(file);
                this.books = wrapper.getBooks();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public synchronized void saveToXml() {
        try {
            LibraryWrapper wrapper = new LibraryWrapper();
            wrapper.setBooks(this.books);
            JAXBContext context = JAXBContext.newInstance(LibraryWrapper.class);
            Marshaller marshaller = context.createMarshaller();
            marshaller.setProperty(Marshaller.JAXB_FORMATTED_OUTPUT, true);
            marshaller.marshal(wrapper, new File(FILE_PATH));
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public List<Book> getAllBooks() { return books; }

    public Book getBookById(int id) {
        return books.stream().filter(b -> b.getId() == id).findFirst().orElse(null);
    }

    public void addBook(Book book) {
        int maxId = books.stream().mapToInt(Book::getId).max().orElse(0);
        book.setId(maxId + 1);
        book.setAvailableCopies(book.getTotalCopies());
        books.add(book);
        saveToXml();
    }

    public void updateBook(Book updatedBook) {
        // Логика обновления (например, цены)
        saveToXml();
    }

    public List<Book> search(String query) {
        if (query == null || query.isEmpty()) return books;
        String q = query.toLowerCase();
        return books.stream()
                .filter(b -> b.getTitle().toLowerCase().contains(q) ||
                        b.getAuthor().toLowerCase().contains(q) ||
                        String.valueOf(b.getYear()).contains(q) ||
                        b.getGenre().toLowerCase().contains(q))
                .collect(Collectors.toList());
    }
}