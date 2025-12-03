package org.example.model;

import jakarta.xml.bind.annotation.*;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;
import java.util.List;

@Data
@AllArgsConstructor
@NoArgsConstructor
@XmlAccessorType(XmlAccessType.FIELD)
public class Book {
    @XmlAttribute(required = true)
    private int id;
    @XmlAttribute(required = true)
    private int totalCopies;
    @XmlAttribute(required = true)
    private int availableCopies;

    private String title;
    private String author;
    private int year;
    private String genre;
    private double price;
}