package org.example.model;

import jakarta.xml.bind.annotation.*;
import lombok.Data;
import java.util.ArrayList;
import java.util.List;

@Data
@XmlRootElement(name = "library")
@XmlAccessorType(XmlAccessType.FIELD)
public class LibraryWrapper {
    @XmlElement(name = "book")
    private List<Book> books = new ArrayList<>();
}