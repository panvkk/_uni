package org.example;

import java.util.*;

public class GradeBook {
    // КОНСТАНТЫ: Сделаны public static final для доступа из Main.java (исправляет ошибки доступа)
    public static final byte EXAM_MIN_MARK = 0;
    public static final byte EXAM_MAX_MARK = 10;
    public static final byte CREDIT_PASSED_MARK = 1;
    public static final byte CREDIT_FAILED_MARK = 0;
    public static final double MIN_AVERAGE = 0.0;
    public static final double MAX_AVERAGE = 10.0;
    public static final byte MIN_SESSION = 1;
    public static final byte MAX_SESSION = 9;

    private String surname;
    private String name;
    private String middle_name;
    private byte course;
    private byte group;
    private double averageAll;
    private final List<Session> sessions = new ArrayList<>(MAX_SESSION);

    public GradeBook (String surname,
                      String name,
                      String middle_name,
                      byte course,
                      byte group) {
        this.surname = Objects.requireNonNull(surname);
        this.name = Objects.requireNonNull(name);
        this.middle_name = Objects.requireNonNull(middle_name);
        this.course = course;
        this.group = group;

        // Инициализируем все 9 сессий сразу
        for (byte num = MIN_SESSION; num <= MAX_SESSION; num++) {
            sessions.add(new Session(num));
        }
    }

    public void calculateAllAverages() {
        double totalExamMarkSum = 0;
        int totalExamCount = 0;

        for (byte i = MIN_SESSION; i <= MAX_SESSION; i++) {
            Session currentSession = this.getSession(i);

            if (!currentSession.isDisciplinesEmpty()) {
                double sessionExamMarkSum = 0;
                int sessionExamCount = 0;

                for (GradeBook.Session.Discipline disc : currentSession.getDisciplines()) {
                    if (disc.isExam()) {
                        sessionExamMarkSum += disc.getMark();
                        sessionExamCount++;
                    }
                }

                // Обновление общего среднего
                totalExamMarkSum += sessionExamMarkSum;
                totalExamCount += sessionExamCount;

                // Обновление среднего балла сессии
                if (sessionExamCount > 0) {
                    currentSession.setAverage(sessionExamMarkSum / sessionExamCount);
                } else {
                    currentSession.setAverage(0.0);
                }
            } else {
                currentSession.setAverage(0.0);
            }
        }

        // Обновление общего среднего балла
        if (totalExamCount > 0) {
            this.setAverageAll(totalExamMarkSum / totalExamCount);
        } else {
            // Устанавливаем 0.0, если оценок нет
            this.averageAll = 0.0;
        }
    }

    // Оставлен для совместимости с оригинальным кодом
    public void calculateAverageMarkAllSessions() {
        calculateAllAverages();
    }

    public void setAverageAll(double a) {
        // Убрал проверку a <= 0, чтобы позволить среднему баллу быть 0.0
        if (a < MIN_AVERAGE || a > MAX_AVERAGE) {
            throw new IllegalArgumentException("Average mark should be >= " + MIN_AVERAGE + " and <= " + MAX_AVERAGE);
        }
        this.averageAll = a;
    }

    public double getAverageAll() {
        return averageAll;
    }

    public boolean isExcellent() {
        for (Session session : this.sessions) {
            if (session.isDisciplinesEmpty()) {
                continue;
            }

            for (GradeBook.Session.Discipline disc : session.getDisciplines()) {
                byte mark = disc.getMark();

                if (disc.isExam()) {
                    // Экзамен: должен быть >= 9
                    if (mark < 9) {
                        return false;
                    }
                } else {
                    // Зачёт: должен быть Зачтено (1)
                    if (mark != CREDIT_PASSED_MARK) {
                        return false;
                    }
                }
            }
        }
        return true;
    }

    // Переименован для соответствия именованию Java-методов
    public void calculateAverageMarkInSession(byte n) {
        if (n < MIN_SESSION || n > MAX_SESSION) {
            throw new IllegalArgumentException("Number of session should be >= " + MIN_SESSION + " and <= " + MAX_SESSION);
        }

        Session currentSession = this.getSession(n);

        double average = 0;
        int i = 0;

        if (!currentSession.isDisciplinesEmpty()) {
            for (GradeBook.Session.Discipline disc : currentSession.getDisciplines()) {
                if (disc.isExam()) {
                    average += disc.getMark();
                    i++;
                }
            }
        }

        if (i > 0) {
            currentSession.setAverage(average / i);
        } else {
            currentSession.setAverage(0.0);
        }
    }

    public List<Session> getSessions() {
        return Collections.unmodifiableList(sessions);
    }

    public Session getSession(byte session_number) {
        if (session_number < MIN_SESSION || session_number > MAX_SESSION) {
            throw new IllegalArgumentException("Number of session should be >=" + MIN_SESSION + " and <=" + MAX_SESSION);
        }

        return sessions.get(session_number - 1);
    }

    // Геттеры
    public String getName() { return name; }
    public String getSurname() { return surname; }
    public String getMiddleName() { return middle_name; }
    public byte getCourse() { return course; }
    public byte getGroup() { return group; }
    public String getFullName(){
        return String.format("%s %s %s", surname, name, middle_name);
    }

    // Внутренний класс Session
    public class Session {
        private final byte session_number;
        private double average;
        private final List<Discipline> disciplines = new ArrayList<>();

        private Session(byte session_number) {
            this.session_number = session_number;
        }

        public byte getSessionNumber() { return session_number; }

        public void setAverage(double a) {
            if (a < MIN_AVERAGE || a > MAX_AVERAGE) {
                throw new IllegalArgumentException("Average mark should be >= " + MIN_AVERAGE + " and <= " + MAX_AVERAGE);
            }
            this.average = a;
        }

        public double getAverage() { return average; }

        public List<Discipline> getDisciplines() { return disciplines; }

        public Discipline addDiscipline(String discipline_name, boolean is_exam, byte mark) {
            // Добавлена валидация оценок на уровне добавления
            if (is_exam) {
                if (mark < EXAM_MIN_MARK || mark > EXAM_MAX_MARK) {
                    throw new IllegalArgumentException("Mark of exam should be >= " + EXAM_MIN_MARK + " and <= " + EXAM_MAX_MARK);
                }
            } else {
                if (mark != CREDIT_PASSED_MARK && mark != CREDIT_FAILED_MARK) {
                    throw new IllegalArgumentException("Mark of credit should be " + CREDIT_FAILED_MARK + " or " + CREDIT_PASSED_MARK);
                }
            }

            Discipline d = new Discipline(discipline_name, is_exam, mark);
            disciplines.add(d);
            return d;
        }

        public boolean isDisciplinesEmpty() { return disciplines.isEmpty(); }

        // Внутренний класс Discipline
        public class Discipline {
            private final String discipline_name;
            private final boolean is_exam;
            private final byte mark;

            private Discipline(String discipline_name, boolean is_exam, byte mark) {
                this.discipline_name = discipline_name;
                this.is_exam = is_exam;
                this.mark = mark;
            }

            public String getNameOfDiscipline() { return discipline_name; }
            public boolean isExam() { return is_exam; }
            public byte getMark() {
                // Возвращает сохраненное значение, так как оно уже проверено в addDiscipline
                return mark;
            }
        }
    }
}