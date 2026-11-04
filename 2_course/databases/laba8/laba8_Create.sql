-- 1. Таблица групп ОС
CREATE TABLE AssetGroups (
    GroupCode INT PRIMARY KEY,
    GroupName NVARCHAR(100) NOT NULL
);

-- 2. Таблица ответственных лиц
CREATE TABLE ResponsiblePersons (
    PersonCode INT PRIMARY KEY,
    FullName NVARCHAR(150) NOT NULL
);

-- 3. Таблица инвентарных объектов
CREATE TABLE FixedAssets (
    InventoryNumber INT PRIMARY KEY,
    AssetName NVARCHAR(200) NOT NULL,
    GroupCode INT NOT NULL,
    Characteristic NVARCHAR(MAX),
    CONSTRAINT FK_Asset_Group FOREIGN KEY (GroupCode) REFERENCES AssetGroups(GroupCode)
);

-- 4. Таблица операци
CREATE TABLE AssetOperations (
    OpID INT IDENTITY(1,1) PRIMARY KEY,
    InventoryNumber INT NOT NULL,
    PersonCode INT NOT NULL,
    OpDate DATE NOT NULL DEFAULT GETDATE(),
    OpType NVARCHAR(10) NOT NULL CHECK (OpType IN ('Приход', 'Расход')),
    Cost DECIMAL(18,2) NOT NULL,
    DocInfo NVARCHAR(100),
    Reason NVARCHAR(200),
    CONSTRAINT FK_Op_Asset FOREIGN KEY (InventoryNumber) REFERENCES FixedAssets(InventoryNumber),
    CONSTRAINT FK_Op_Person FOREIGN KEY (PersonCode) REFERENCES ResponsiblePersons(PersonCode),
    CONSTRAINT CK_Cost_Positive CHECK (Cost >= 0)
);

-- Создание индекса для ускорения поиска по дате
CREATE INDEX IX_OpDate ON AssetOperations(OpDate);

-- Наполнение данными для тестов
INSERT INTO AssetGroups VALUES (1, 'Машины и оборудование'), (2, 'Вычислительная техника');
INSERT INTO ResponsiblePersons VALUES (101, 'Иванов И.И.'), (102, 'Петров П.П.');
INSERT INTO FixedAssets VALUES (5001, 'Сервер Dell', 2, 'Intel Xeon, 64GB RAM');
INSERT INTO AssetOperations (InventoryNumber, PersonCode, OpDate, OpType, Cost, DocInfo)
VALUES (5001, 101, '2023-01-15', 'Приход', 150000.00, 'Акт №1');