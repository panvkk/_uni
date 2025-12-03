package org.example.controller;

import org.example.model.User;
import org.example.service.BookService;
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

    public ReaderController(BookService bookService, UserService userService) {
        this.bookService = bookService;
        this.userService = userService;
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
}