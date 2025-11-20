package org.example;

import java.util.ArrayList;
import java.util.List;

public class GradeBook {
    public String lastName;
    public String firstName;
    public String middleName;
    public int course;
    public String group;
    public List<SessionRecord> sessions = new ArrayList<>();

    public static class SessionRecord {
        public int sessionNumber;
        public String subject;
        public int grade;
        public boolean isCredit;

        @Override
        public String toString() {
            // Для зачета: 1 = "Зачтено", другое = "Не зачтено"
            String gradeDisplay = isCredit ? (grade == 1 ? "Зачтено" : "Не зачтено") : String.valueOf(grade);
            return String.format("Сессия %d - %s: %s", sessionNumber, subject, gradeDisplay);
        }
    }

    public boolean isExcellent() {
        for (SessionRecord r : sessions) {
            // Для зачета (isCredit = true) - Оценка 1 = "Зачтено"
            if (r.isCredit) {
                if (r.grade < 1) return false;
                // Для экзамена/диф. зачета (isCredit = false) - Оценка >= 9
            } else {
                if (r.grade < 9) return false;
            }
        }
        return true;
    }

    public static GradeBook fromTxtLine(String line) {
        GradeBook gb = new GradeBook();
        String[] parts = line.split(";");

        if (parts.length < 5) {
            throw new IllegalArgumentException("Недостаточно полей для GradeBook: " + line);
        }

        gb.lastName = parts[0];
        gb.firstName = parts[1];
        gb.middleName = parts[2];
        // Использование trim() для удаления лишних пробелов, которые могут быть перед числами
        gb.course = Integer.parseInt(parts[3].trim());
        gb.group = parts[4];

        for (int i = 5; i < parts.length; i++) {
            String sessionPart = parts[i];
            String[] sessionFields = sessionPart.split(":");
            if (sessionFields.length == 4) {
                SessionRecord sr = new SessionRecord();
                sr.sessionNumber = Integer.parseInt(sessionFields[0].trim());
                sr.subject = sessionFields[1];
                sr.grade = Integer.parseInt(sessionFields[2].trim());
                sr.isCredit = Boolean.parseBoolean(sessionFields[3].trim());
                gb.sessions.add(sr);
            }
        }

        return gb;
    }

    // Измененный метод для output.txt
    public String toTxtString() {
        String excellentStatus = isExcellent() ? "Отличник" : "Обычный студент";
        // Формат для output.txt: Фамилия Имя Отчество;Курс;Группа;Статус
        return String.format("%s %s %s;%d;%s;%s",
                lastName, firstName, middleName, course, group, excellentStatus);
    }
}