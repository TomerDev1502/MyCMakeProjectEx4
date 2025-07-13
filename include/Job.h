#pragma once
#include <string>
#include <sys/types.h>

/// מצב של Job ברשימה
enum class JobStatus { Running, Done };

/// מבנה לשמירת פרטי תהליך ברקע
struct Job {
    int id;               // מספר זיהוי רץ
    pid_t pid;            // PID של התהליך
    std::string cmd;      // מחרוזת הפקודה
    JobStatus status;     // מצב (Running/Done)
};
