package org.example.controller;

import org.example.model.Book;
import org.example.model.User;
import org.example.repository.UserRepository;
import org.example.service.BookService;
import org.example.service.UserService;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@Controller
@RequestMapping("/librarian")
public class LibrarianController {
    private final BookService bookService;
    private final UserRepository userRepository;
    private final UserService userService;

    public LibrarianController(
            BookService bookService,
            UserRepository userRepository,
            UserService userService
    ) {
        this.bookService = bookService;
        this.userRepository = userRepository;
        this.userService = userService;
    }

    @GetMapping("/books")
    public String listBooks(Model model) {
        model.addAttribute("books", bookService.getAllBooks());
        return "librarian_books";
    }

    @GetMapping("/books/add")
    public String addBookForm(Model model) {
        model.addAttribute("book", new Book());
        return "book_add";
    }

    @PostMapping("/books/add")
    public String addBook(@ModelAttribute Book book) {
        bookService.addBook(book);
        return "redirect:/librarian/books";
    }

    @PostMapping("/books/updatePrice")
    public String updatePrice(@RequestParam int id, @RequestParam double newPrice) {
        Book book = bookService.getBookById(id);
        if (book != null) {
            book.setPrice(newPrice);
            bookService.saveToXml();
        }
        return "redirect:/librarian/books";
    }

    // 3.1.4 Эмуляция выдачи
    @GetMapping("/issue")
    public String issuePage(Model model) {
        model.addAttribute("books", bookService.getAllBooks());
        model.addAttribute("users", userRepository.findAll()); // Список всех юзеров
        return "issue_book";
    }

    @PostMapping("/issue")
    public String issueBook(@RequestParam int bookId, @RequestParam Long userId) {
        Book book = bookService.getBookById(bookId);
        User user = userRepository.findById(userId).orElse(null);

        if (book != null && user != null && book.getAvailableCopies() > 0) {
            // 1. Уменьшаем копии в XML
            book.setAvailableCopies(book.getAvailableCopies() - 1);
            bookService.saveToXml();

            // 2. Записываем в аккаунт читателя
            user.getIssuedBooks().add(book.getTitle() + " (" + book.getAuthor() + ")");
            userService.save(user);
        }
        return "redirect:/librarian/issue";
    }

    // --- НОВАЯ ЛОГИКА ВОЗВРАТА ---

    @GetMapping("/return")
    public String returnPage(Model model) {
        // Мы передаем просто список всех пользователей.
        // Thymeleaf сам переберет их книги.
        model.addAttribute("users", userRepository.findAll());
        return "return_book";
    }

    @PostMapping("/return")
    public String returnBook(@RequestParam Long userId, @RequestParam String bookString) {
        User user = userRepository.findById(userId).orElse(null);

        if (user != null) {
            // 1. Удаляем строку с книгой у пользователя
            // (bookString приходит точно такой, как он записан в базе)
            if (user.getIssuedBooks().contains(bookString)) {
                user.getIssuedBooks().remove(bookString);
                userService.save(user);

                // 2. Ищем эту книгу в XML, чтобы увеличить счетчик
                // Так как у нас нет ID книги в юзере, ищем по совпадению названия
                List<Book> allBooks = bookService.getAllBooks();
                for (Book b : allBooks) {
                    String recordName = b.getTitle() + " (" + b.getAuthor() + ")";
                    if (recordName.equals(bookString)) {
                        if (b.getAvailableCopies() < b.getTotalCopies()) {
                            b.setAvailableCopies(b.getAvailableCopies() + 1);
                        }
                        break; // Книга найдена и обновлена
                    }
                }
                bookService.saveToXml();
            }
        }
        return "redirect:/librarian/return?success";
    }

    // 3.1.5 Просмотр читателей
    @GetMapping("/readers")
    public String listReaders(Model model) {
        model.addAttribute("readers", userRepository.findAll());
        return "readers_list";
    }
}