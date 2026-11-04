-- 1. Многотабличный запрос с сортировкой и отбором
-- Список всех объектов с их группами
SELECT A.InventoryNumber, A.AssetName, G.GroupName
FROM FixedAssets A
INNER JOIN AssetGroups G ON A.GroupCode = G.GroupCode
WHERE G.GroupCode = 2
ORDER BY A.AssetName;

-- 2. Запрос с вычисляемым полем
-- Добавляем к стоимости условный налог 18% для оценки
SELECT InventoryNumber, Cost, Cost * 0.18 AS TaxAmount, Cost * 1.18 AS TotalWithTax
FROM AssetOperations;

-- 3. Запрос с внешним объединением
-- Показать всех сотрудников и их операции
SELECT P.FullName, O.OpDate, O.OpType, O.Cost
FROM ResponsiblePersons P
LEFT JOIN AssetOperations O ON P.PersonCode = O.PersonCode;

-- 4. Запрос с группировкой и HAVING
-- Группировка операций по типу, где суммарная стоимость более 100 000
SELECT OpType, SUM(Cost) AS TotalSum
FROM AssetOperations
GROUP BY OpType
HAVING SUM(Cost) > 100000;

-- 5. Запрос на добавление
INSERT INTO FixedAssets (InventoryNumber, AssetName, GroupCode, Characteristic)
VALUES (5002, 'Ноутбук HP', 2, 'Core i5, 16GB');

-- 6. Запрос на удаление
DELETE FROM AssetOperations WHERE OpID = 999;

-- 7. Запрос на обновление
UPDATE AssetOperations SET Cost = Cost * 1.05 WHERE OpType = 'Приход';

-- 8. Создание новой таблицы на основе существующей (Архив операций)
SELECT * INTO AssetOpsArchive FROM AssetOperations WHERE OpDate < '2023-01-01';

-- 9. Запрос на объединение (UNION)
-- Список всех кодов из двух справочников
SELECT PersonCode AS Code, 'Персонал' AS Category FROM ResponsiblePersons
UNION
SELECT GroupCode, 'Группа ОС' FROM AssetGroups;

-- 10. Вложенный запрос
-- Найти операции по объектам, принадлежащим группе Вычислительная техника
SELECT * FROM AssetOperations
WHERE InventoryNumber IN (SELECT InventoryNumber FROM FixedAssets WHERE GroupCode = 2);

-- 11. Создание индекса
CREATE INDEX IX_AssetName ON FixedAssets(AssetName);

-- 12. Создание представления
GO
CREATE VIEW View_AnalyticReport AS
SELECT 
    A.InventoryNumber, 
    A.AssetName, 
    G.GroupName, 
    O.OpDate AS EntryDate, 
    O.Cost,
    P.FullName AS ResponsiblePerson
FROM FixedAssets A
JOIN AssetGroups G ON A.GroupCode = G.GroupCode
JOIN AssetOperations O ON A.InventoryNumber = O.InventoryNumber
JOIN ResponsiblePersons P ON O.PersonCode = P.PersonCode
WHERE O.OpType = 'Приход';
GO