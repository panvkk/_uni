-- 1-3 
SELECT username, phone_number FROM Users;
SELECT DISTINCT file_type FROM Attachments;
SELECT TOP 5 message_id, [content], is_read FROM Messages 
ORDER BY message_id DESC;
-- 4
SELECT u.username, m.[content], m.is_read FROM Users u
INNER JOIN Messages m ON u.user_id = m.sender_id
WHERE m.is_read = 0 ORDER BY u.username ASC;
-- 5
SELECT message_id, [content], LEN([content]) AS message_length
FROM Messages;
-- 6
SELECT u.username, cm.chat_id FROM Users u
LEFT JOIN Chat_Members cm ON u.user_id = cm.user_id;
-- 7
SELECT chat_id, COUNT(message_id) AS total_messages FROM Messages
GROUP BY chat_id HAVING COUNT(message_id) > 50;
-- 8
SELECT username, phone_number FROM Users
WHERE user_id IN (
    SELECT sender_id 
    FROM Messages 
    WHERE [content] LIKE '%привет%'
);
-- 9 
SELECT c.chat_id, c.type FROM Chats c
WHERE EXISTS (
    SELECT 1 
    FROM Messages m
    INNER JOIN Attachments a ON m.message_id = a.message_id
    WHERE m.chat_id = c.chat_id
);
-- 10 
-- Добавление нового пользователя
INSERT INTO Users (username, phone_number, bio) 
VALUES ('vova_vodolaz', '+3752911221', 'Салам всей братве из 11 изолятора');
-- Удаление опасных вложений
DELETE FROM Attachments 
WHERE file_type = 'exe';

-- 11
-- Создание копии структуры таблицы Users для бэкапа
SELECT user_id, username, phone_number, bio INTO Users_Backup 
FROM Users 
WHERE 1 = 0;

-- Создание абсолютно новой таблицы для черных списков с нуля со связью
CREATE TABLE Blacklist (
    ban_id INT IDENTITY(1,1) PRIMARY KEY,
    user_id INT NOT NULL,
    reason NVARCHAR(200) NULL,
    FOREIGN KEY (user_id) REFERENCES Users(user_id)
);

-- 12
-- Индекс для ускорения поиска и фильтрации пользователей по их юзернейму
CREATE INDEX IX_Users_Username ON Users(username);