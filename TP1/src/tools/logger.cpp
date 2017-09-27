#include "tools/logger.h"

extern int LOG_MIN_LEVEL;
extern std::ofstream LOG_FILE_POINTER;

std::string getDate() {
    std::time_t now = std::time(nullptr);
    struct tm tstruct;
    char buf[40];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%F %T", &tstruct);
    return buf;
}

std::string getPID() {
    return std::to_string(getpid());
}

// PRE: Global variables LOG_MIN_LEVEL and LOG_FILE_POINTER have to be initialized.
// Returns 0 if the message was logged, 1 otherwise.
int log(std::string message, int messageLevel) {
    if (messageLevel < LOG_MIN_LEVEL) {
        return 1;
    }
    std::string timeString = getDate();
    std::string toLog = "[" + getDate() + " - " + getPID() + "] : " + message + "\n";
    LOG_FILE_POINTER << toLog;
    return 0;
}

// Logs a separator indicating the session start.
int logSessionStarted() {
    std::string separator = "========================================================================================================== \n";
    std::string sessionStartHeader = "Program started at " + getDate() + " by " + getPID() + "\n";
    LOG_FILE_POINTER << separator;
    LOG_FILE_POINTER << sessionStartHeader;
    LOG_FILE_POINTER << separator;
    return 0;
}

// Logs a separator indicating the session finished.
int logSessionFinished() {
    std::string separator = "========================================================================================================== \n";
    std::string sessionFinishedHeader = "Program finished at " + getDate() + " by " + getPID() + "\n";
    LOG_FILE_POINTER << separator;
    LOG_FILE_POINTER << sessionFinishedHeader;
    return 0;
}

void flushLog() {
    LOG_FILE_POINTER.flush();
}

// Overloaded loggers with concatenators ---------------------------------------------------------------------

int log(std::string messageToConcatenate, int integerToConcatenate, int messageLevel) {
    std::string intToString = std::to_string(integerToConcatenate);
    std::string concatenatedMessage = messageToConcatenate + intToString;
    return log(concatenatedMessage, messageLevel);
}

int log(std::string messageToConcatenate, const char* charsToConcatenate, int messageLevel) {
    std::string concatenatedMessage = "";
    if (charsToConcatenate != NULL) {
        std::string charToString(charsToConcatenate);
        concatenatedMessage = messageToConcatenate + charToString;
    } else {
        concatenatedMessage = messageToConcatenate + "[ **error** char* is NULL]";
    }
    return log(concatenatedMessage, messageLevel);
}

int log(std::string messageToConcatenate, char* charsToConcatenate, int messageLevel) {
    std::string concatenatedMessage = "";
    if (charsToConcatenate != NULL) {
        std::string charToString(charsToConcatenate);
        concatenatedMessage = messageToConcatenate + charToString;
    } else {
        concatenatedMessage = messageToConcatenate + "[ **error** char* is NULL]";
    }
    return log(concatenatedMessage, messageLevel);
}

int log(Loggable* classToConcatenate, int messageLevel) {
	std::string toLog;
    if (classToConcatenate != NULL) {
        toLog = classToConcatenate->logMemberVariables();
    } else {
        toLog = "[ **error** classToConcatenate* is NULL]";
    }
    return log(toLog, messageLevel);
}

int log(std::string messageToConcatenate, Loggable* classToConcatenate, int messageLevel) {
    std::string concatenatedMessage = "";
    if (classToConcatenate != NULL) {
        concatenatedMessage = messageToConcatenate + classToConcatenate->logMemberVariables();
    } else {
        concatenatedMessage = messageToConcatenate + "[ **error** classToConcatenate* is NULL]";
    }
    return log(concatenatedMessage, messageLevel);
}
