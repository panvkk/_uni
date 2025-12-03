package org.example.controller;

import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpSession;
import org.example.model.User;
import org.example.service.UserService;
import org.springframework.security.authentication.UsernamePasswordAuthenticationToken;
import org.springframework.security.core.Authentication;
import org.springframework.security.core.context.SecurityContextHolder;
import org.springframework.security.web.context.HttpSessionSecurityContextRepository;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.*;

@Controller
public class AuthController {
    private final UserService userService;

    public AuthController(UserService userService) {
        this.userService = userService;
    }

    @GetMapping("/login")
    public String login() { return "login"; }

    @GetMapping("/register")
    public String registerForm(Model model) {
        model.addAttribute("user", new User());
        return "register";
    }

    @PostMapping("/register")
    public String register(@ModelAttribute User user, HttpServletRequest request) {
        String rawPassword = user.getPassword();

        // 1. Присваиваем роль С ПРЕФИКСОМ для базы данных
        if (user.getUsername().contains("admin")) {
            user.setRole("ROLE_LIBRARIAN");
        } else {
            user.setRole("ROLE_READER");
        }

        userService.register(user);

        // --- АВТО-ЛОГИН ---
        try {
            // 2. Для авто-логина нам нужно создать Authority.
            // AuthorityUtils.createAuthorityList ожидает полный формат (с ROLE_)
            Authentication auth = new UsernamePasswordAuthenticationToken(
                    user.getUsername(),
                    null,
                    org.springframework.security.core.authority.AuthorityUtils.createAuthorityList(user.getRole()) // <-- Тут передаем "ROLE_READER" целиком
            );

            SecurityContextHolder.getContext().setAuthentication(auth);
            HttpSession session = request.getSession(true);
            session.setAttribute(HttpSessionSecurityContextRepository.SPRING_SECURITY_CONTEXT_KEY, SecurityContextHolder.getContext());

        } catch (Exception e) {
            e.printStackTrace();
            return "redirect:/login?error";
        }

        return "redirect:/default";
    }

    @GetMapping("/default")
    public String defaultAfterLogin(HttpServletRequest request) {
        if (request.isUserInRole("LIBRARIAN")) return "redirect:/librarian/books";
        return "redirect:/reader/books";
    }
}