package org.example.controller;

import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpSession;
import org.example.model.User;
import org.example.service.UserService;
import org.springframework.security.authentication.UsernamePasswordAuthenticationToken;
import org.springframework.security.core.Authentication;
import org.springframework.security.core.context.SecurityContextHolder;
import org.springframework.security.core.userdetails.UserDetails;
import org.springframework.security.core.userdetails.UserDetailsService;
import org.springframework.security.web.context.HttpSessionSecurityContextRepository;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.*;

@Controller
public class AuthController {
    private final UserService userService;
    // Добавляем UserDetailsService, чтобы загрузить правильный объект пользователя
    private final UserDetailsService userDetailsService;

    public AuthController(UserService userService, UserDetailsService userDetailsService) {
        this.userService = userService;
        this.userDetailsService = userDetailsService;
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
        // 1. Устанавливаем роль
        if (user.getUsername().contains("admin")) {
            user.setRole("ROLE_LIBRARIAN");
        } else {
            user.setRole("ROLE_READER");
        }

        // 2. Сохраняем в БД
        userService.register(user);

        // --- ИСПРАВЛЕННЫЙ АВТО-ЛОГИН ---
        try {
            // Загружаем UserDetails через стандартный сервис Spring Security
            // Это гарантирует, что объект будет именно того типа, который ожидают контроллеры
            UserDetails userDetails = userDetailsService.loadUserByUsername(user.getUsername());

            Authentication auth = new UsernamePasswordAuthenticationToken(
                    userDetails, // <-- Теперь здесь объект, а не просто строка
                    userDetails.getPassword(),
                    userDetails.getAuthorities()
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