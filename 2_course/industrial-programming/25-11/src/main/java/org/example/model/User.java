package org.example.model;

import jakarta.persistence.*;
import lombok.Data;
import java.util.ArrayList;
import java.util.List;

@Entity
@Data
@Table(name = "users")
public class User {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    @Column(unique = true)
    private String username;
    private String password;
    private String role; // "ROLE_LIBRARIAN" или "ROLE_READER"

    @ElementCollection(fetch = FetchType.EAGER)
        private List<String> issuedBooks = new ArrayList<>();

        public List<String> getIssuedBooks() {
            if (issuedBooks == null) {
                issuedBooks = new ArrayList<>();
            }
            return issuedBooks;
        }
}