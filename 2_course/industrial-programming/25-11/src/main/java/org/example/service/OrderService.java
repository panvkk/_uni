package org.example.service;

import org.example.model.Book;
import org.example.model.User;
import org.example.repository.UserRepository;
import org.springframework.scheduling.annotation.Async;
import org.springframework.stereotype.Service;

@Service
public class OrderService {

    private final BookService bookService;
    private final UserService userService;
    private final UserRepository userRepository;

    public OrderService(BookService bookService, UserService userService, UserRepository userRepository) {
        this.bookService = bookService;
        this.userService = userService;
        this.userRepository = userRepository;
    }

    @Async("libraryTaskExecutor")
    public void processOrder(int bookId, Long userId) {
        String threadName = Thread.currentThread().getName();
        System.out.println(">>> [Thread: " + threadName + "] Reader ordered a book. Librarian started working...");

        try {
            Thread.sleep(3000);

            Book book = bookService.getBookById(bookId);
            User user = userRepository.findById(userId).orElse(null);

            if (book != null && user != null) {
                synchronized (book) { // Блокируем книгу от других потоков
                    if (book.getAvailableCopies() > 0) {
                        // 1. Списываем копию
                        book.setAvailableCopies(book.getAvailableCopies() - 1);
                        bookService.saveToXml();

                        // 2. Выдаем читателю
                        user.getIssuedBooks().add(book.getTitle() + " (" + book.getAuthor() + ")");
                        userService.save(user);

                        System.out.println(">>> [Thread: " + threadName + "] SUCCESS: Book issued to " + user.getUsername());
                    } else {
                        System.out.println(">>> [Thread: " + threadName + "] FAIL: Book is out of stock.");
                    }
                }
            }
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
        System.out.println(">>> [Thread: " + threadName + "] Librarian finished work.");
    }
}