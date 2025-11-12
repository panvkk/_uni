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
    }

    public boolean isExcellent() {
        for (SessionRecord r : sessions) {
            if (r.isCredit) {
                if (r.grade < 1) return false;
            } else {
                if (r.grade < 9) return false;
            }
        }
        return true;
    }
}
