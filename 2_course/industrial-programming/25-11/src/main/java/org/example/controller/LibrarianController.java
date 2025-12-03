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

    public LibrarianController(BookService bookService, UserRepository userRepository, UserService userService) {
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

    // --- ВОЗВРАТ КНИГИ (Страница) ---
    @GetMapping("/return")
    public String returnPage(Model model) {
        // Нам нужны и книги, и пользователи, чтобы выбрать, кто и что возвращает
        model.addAttribute("books", bookService.getAllBooks());
        model.addAttribute("users", userRepository.findAll());
        return "return_book";
    }

    // --- ВОЗВРАТ КНИГИ (Логика) ---
    @PostMapping("/return")
    public String returnBook(@RequestParam int bookId, @RequestParam Long userId) {
        Book book = bookService.getBookById(bookId);
        User user = userRepository.findById(userId).orElse(null);

        if (book != null && user != null) {
            // 1. Формируем строку, как она записана у пользователя
            String bookRecord = book.getTitle() + " (" + book.getAuthor() + ")";

            // 2. Проверяем, есть ли у пользователя эта книга
            if (user.getIssuedBooks().contains(bookRecord)) {

                // 3. Удаляем книгу у пользователя
                user.getIssuedBooks().remove(bookRecord);
                userService.save(user);

                // 4. Увеличиваем количество доступных копий в XML
                // (но не больше общего количества)
                if (book.getAvailableCopies() < book.getTotalCopies()) {
                    book.setAvailableCopies(book.getAvailableCopies() + 1);
                    bookService.saveToXml();
                }
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