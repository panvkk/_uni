package org.example.controller;

import org.example.model.User;
import org.example.service.BookService;
import org.example.service.OrderService;
import org.example.service.UserService;
import org.springframework.security.core.annotation.AuthenticationPrincipal;
import org.springframework.security.core.userdetails.UserDetails;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.*;

@Controller
@RequestMapping("/reader")
public class ReaderController {
    private final BookService bookService;
    private final UserService userService;
    private final OrderService orderService;

    public ReaderController(
            BookService bookService,
            UserService userService,
            OrderService orderService
    ) {
        this.bookService = bookService;
        this.userService = userService;
        this.orderService = orderService;
    }

    @GetMapping("/books")
    public String listBooks(Model model, @RequestParam(required = false) String query) {
        model.addAttribute("books", bookService.search(query));
        return "reader_books";
    }

    @GetMapping("/account")
    public String myAccount(@AuthenticationPrincipal UserDetails userDetails, Model model) {
        User user = userService.findByUsername(userDetails.getUsername());
        model.addAttribute("user", user);
        return "reader_account";
    }

    @GetMapping("/order")
    public String orderPage(Model model, @RequestParam(required = false) String genre) {
        if (genre != null && !genre.isEmpty()) {
            model.addAttribute("books", bookService.search(genre));
            model.addAttribute("selectedGenre", genre);
        }
        return "order_book";
    }

    @PostMapping("/order")
    public String placeOrder(@RequestParam int bookId, @AuthenticationPrincipal UserDetails userDetails) {
        User user = userService.findByUsername(userDetails.getUsername());

        // Запускаем процесс в фоновом потоке.
        // Метод вернет управление мгновенно, а поток продолжит работать.
        orderService.processOrder(bookId, user.getId());

        // Перенаправляем с сообщением
        return "redirect:/reader/order?success=true";
    }
}