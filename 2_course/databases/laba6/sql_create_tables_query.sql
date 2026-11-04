-- 1. Книги
CREATE TABLE Books (
    book_id INT PRIMARY KEY IDENTITY(1,1),
    title NVARCHAR(255) NOT NULL,
    authors NVARCHAR(255),
    publisher NVARCHAR(100)
);

-- 2. Продавцы
CREATE TABLE Sellers (
    seller_id INT PRIMARY KEY IDENTITY(1,1),
    full_name NVARCHAR(150) NOT NULL,
    location_desc NVARCHAR(255), -- Описание или адрес
    commission DECIMAL(5, 2) DEFAULT 0
);

-- 3. Места хранения (Склады / Лотки)
CREATE TABLE Storage_Location (
    location_id INT PRIMARY KEY IDENTITY(1,1),
    seller_id INT NULL FOREIGN KEY REFERENCES Sellers(seller_id),
    name NVARCHAR(100) NOT NULL
);

-- 4. Накладные
CREATE TABLE Invoices (
    invoice_id INT PRIMARY KEY IDENTITY(1,1),
    invoice_number NVARCHAR(50) NOT NULL UNIQUE,
    invoice_date DATETIME DEFAULT GETDATE(),
    operation_type NVARCHAR(50), -- Приход, Продажа, Перемещение
    from_location_id INT NULL FOREIGN KEY REFERENCES Storage_Location(location_id),
    to_location_id INT NULL FOREIGN KEY REFERENCES Storage_Location(location_id)
);

-- 5. Позиции в инвойсе (Многие-ко-многим)
CREATE TABLE Invoice_Items (
    invoice_id INT FOREIGN KEY REFERENCES Invoices(invoice_id),
    book_id INT FOREIGN KEY REFERENCES Books(book_id),
    quantity INT CHECK (quantity > 0),
    PRIMARY KEY (invoice_id, book_id) -- Составной ключ
);

-- 6. Инвентаризация (Запасы в локациях)
CREATE TABLE Inventory (
    location_id INT FOREIGN KEY REFERENCES Storage_Location(location_id),
    book_id INT FOREIGN KEY REFERENCES Books(book_id),
    quantity INT DEFAULT 0 CHECK (quantity >= 0),
    PRIMARY KEY (location_id, book_id)
);

-- 7. Цены (История цен)
CREATE TABLE Prices (
    price_id INT PRIMARY KEY IDENTITY(1,1),
    book_id INT FOREIGN KEY REFERENCES Books(book_id),
    purchase_price DECIMAL(10, 2) CHECK (purchase_price > 0),
    selling_price DECIMAL(10, 2) CHECK (selling_price > 0),
    effective_from DATETIME DEFAULT GETDATE()
);