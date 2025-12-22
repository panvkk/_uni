package org.example.model;

import jakarta.persistence.*;
import lombok.Data;

@Entity
@Data
@Table(name = "order_requests")
public class OrderRequest {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    private Long userId;
    private String username;

    private int bookId;
    private String bookTitle;
    private String bookAuthor;

    private String status; // "PENDING", "APPROVED", "COMPLETED"
}