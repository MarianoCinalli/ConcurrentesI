#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <string>
#include "interfaces/Loggable.h"

// File write
#include <fstream>

// Time
#include <ctime>
#include <unistd.h>

std::string getDate();

void writeToLog(std::string toLog);

int log(std::string message, int messageLevel);

int logSessionStarted();

int logSessionFinished();

int log(std::string messageToConcatenate, int integerToConcatenate, int messageLevel);
int log(std::string messageToConcatenate, const char* charsToConcatenate, int messageLevel);
int log(std::string messageToConcatenate, char* charsToConcatenate, int messageLevel);
int log(std::string messageToConcatenate, Loggable* classToConcatenate, int messageLevel);
int log(Loggable* classToConcatenate, int messageLevel);

void flushLog();

#endif
