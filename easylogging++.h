/***********************************************************************\
* easylogging++.h - Core of EasyLogging++                              *
*   EasyLogging++ v2.5                                                 *
*   Cross platform logging made easy for C++ applications              *
*   Author Majid Khan <mkhan3189@gmail.com>                            *
*   http://www.icplusplus.com                                          *
*   https://github.com/mkhan3189/EasyLoggingPP                         *
*                                                                      *
* This program is free software: you can redistribute it and/or modify *
* it under the terms of the GNU General Public License as published by *
* the Free Software Foundation, version 3 of the License.              *
\***********************************************************************/
#ifndef EASYLOGGINGPP_H
#define EASYLOGGINGPP_H

#define _LOGGING_ENABLED 1

#define _ENABLE_DEBUG_LOGS 1
#define _DEBUG_LOGS_TO_STANDARD_OUTPUT 0
#define _DEBUG_LOGS_TO_FILE 1

#define _ENABLE_INFO_LOGS 1
#define _INFO_LOGS_TO_STANDARD_OUTPUT 1
#define _INFO_LOGS_TO_FILE 1

#define _ENABLE_WARNING_LOGS 1
#define _WARNING_LOGS_TO_STANDARD_OUTPUT 1
#define _WARNING_LOGS_TO_FILE 1

#define _ENABLE_ERROR_LOGS 1
#define _ERROR_LOGS_TO_STANDARD_OUTPUT 1
#define _ERROR_LOGS_TO_FILE 1

#define _ENABLE_FATAL_LOGS 1
#define _FATAL_LOGS_TO_STANDARD_OUTPUT 1
#define _FATAL_LOGS_TO_FILE 1

#define _ENABLE_PERFORMANCE_LOGS 1
#define _PERFORMANCE_LOGS_TO_STANDARD_OUTPUT 0
#define _PERFORMANCE_LOGS_TO_FILE 1

#define _ENABLE_HINTS 1
#define _HINTS_TO_STANDARD_OUTPUT 1
#define _HINTS_TO_FILE 0

#define _ENABLE_STATUS 1
#define _STATUS_TO_STANDARD_OUTPUT 1
#define _STATUS_TO_FILE 0

#if _LOGGING_ENABLED
#include <sys/time.h>
#include <ctime>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
namespace easyloggingpp {
//////////////////////////////////////////////
//   Configuration for logging             ///
//////////////////////////////////////////////
/**
* Flag for showing log in standard output using std::cout
*/
const bool SHOW_STD_OUTPUT = true;

/**
* Flag to set whether to save log to file
*/
const bool SAVE_TO_FILE = true;

/**
* Flag to set whether to show date
*/
const bool SHOW_DATE = false;

/**
 * Flag to set whether to show time
*/
const bool SHOW_TIME = true;

/**
* Flag to set whether to show which file logged the output and what line
*/
const bool SHOW_LOG_LOCATION = false;

/**
* Flag to set whether to show which function logged the output
*/
const bool SHOW_LOG_FUNCTION = false;

/**
* Flag to set whether to show username or not
*/
const bool SHOW_USERNAME = false;

/**
* Flag to set whether to show hostname or not
*/
const bool SHOW_HOSTNAME = false;

/**
* Flag to set whether output value of NOT_SUPPORTED_STRING if extra info is not available on machine
*/
const bool SHOW_NOT_SUPPORTED_ON_NO_EXTRA_INFO = false;

/**
* outputs if extra info is not available on machine and SHOW_NOT_SUPPORTED_ON_NO_EXTRA_INFO is true
*/
const std::string NOT_SUPPORTED_STRING = "-not supported-";

/**
* If saving to file, this defines the filename
*/
const std::string LOG_FILENAME = "myeasylog.log";

/**
* Flag to set whether to save log file in custom location
*/
const bool USE_CUSTOM_LOCATION = true;

/**
* If using custom location, this is where custom location is picked up from.
* Note: This should end with last slash and should be a fully qualified path. 
* Relative paths are not allowed
*/
const std::string CUSTOM_LOG_FILE_LOCATION = "logs/";

/**
 * Determines whether to show log when starting any time tracked function
 */
const bool SHOW_START_FUNCTION_LOG = false;

////////////////////////////////////////////////////////////////////
///         END OF CONFIGURATION FOR LOGGING         ///
////////////////////////////////////////////////////////////////////
const bool EXTRA_INFO_ENABLED = ::easyloggingpp::SHOW_DATE || ::easyloggingpp::SHOW_TIME || ::easyloggingpp::SHOW_LOG_LOCATION || ::easyloggingpp::SHOW_LOG_FUNCTION || ::easyloggingpp::SHOW_USERNAME || ::easyloggingpp::SHOW_HOSTNAME;
static std::stringstream *logStream;
static bool toStandardOutput;
static bool toFile;
static bool streamInitialized = false;
static char dateBuffer[25];
static inline std::string getDateTime(void) {
  if (!(SHOW_DATE || SHOW_TIME)) return "";
#if _WIN32
  char* envDate = getenv("DATE");
  char* envTime = getenv("TIME");
  if ((envDate == NULL) || (envTime == NULL) || ((strcmp(envDate, "")) || (strcmp(envTime, "")))) {
#endif //_WIN32
    //    time_t rawtime;
    //    struct tm * timeinfo;	
    struct timeval t;
    gettimeofday(&t, 0);
    /* time (&rawtime); */
    /* timeinfo = localtime (&rawtime); */
    std::string format = "";
    if (::easyloggingpp::SHOW_DATE) format += "%d/%m/%Y";
    //    if (::easyloggingpp::SHOW_TIME) format += (std::string((::easyloggingpp::SHOW_DATE ? " " : "")) + std::string("%H:%M:%S"));
    std::stringstream  temp;
    temp <<t.tv_sec<<t.tv_usec;
    /* temp.assign((std::string)(t.tv_sec * 1000) + (std::string)t.tv_usec); */
    format += temp.str();
    strncpy(::easyloggingpp::dateBuffer,format.c_str(),format.size());
    temp.str(std::string());
    temp.clear();
    format.clear();

#if _WIN32
  } else {
    if (::easyloggingpp::SHOW_DATE) {
      strcpy(::easyloggingpp::dateBuffer, envDate);
    }
    if (::easyloggingpp::SHOW_TIME) {
      if (::easyloggingpp::SHOW_DATE) {
        strcat(::easyloggingpp::dateBuffer, " ");
        strcat(::easyloggingpp::dateBuffer, envTime);
      } else {
        strcpy(::easyloggingpp::dateBuffer, envTime);
      }
    }
  }
#endif //_WIN32
  std::string buffStr(::easyloggingpp::dateBuffer);
  return buffStr;
}
#ifndef __FILE__
 #define __FILE__ (SHOW_NOT_SUPPORTED_ON_NO_EXTRA_INFO) ? NOT_SUPPORTED_STRING : ""
#endif //__FILE
#ifndef __LINE__
 #define __LINE__ (SHOW_NOT_SUPPORTED_ON_NO_EXTRA_INFO) ? NOT_SUPPORTED_STRING : ""
#endif //_LINE__
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
 #define __func__ __FUNCTION__
#elif defined(__GNUC__) && (__GNUC__ >= 2)
 #define __func__ __PRETTY_FUNCTION__
#else
 #define __func__ (SHOW_NOT_SUPPORTED_ON_NO_EXTRA_INFO) ? NOT_SUPPORTED_STRING : ""
#endif //defined(_MSC_VER) && (_MSC_VER >= 1020)
static inline std::string getUsername(void) {
#if _WIN32
  char* username = getenv("USERNAME");
#else
  char* username = getenv("USER");
#endif //_WIN32
  if ((username == NULL) || ((strcmp(username, "") == 0))) {
    return std::string("");
  } else {
    return std::string(username);
  }
}
static inline std::string getHostname(void) {
#if _WIN32
  char* hostname = getenv("COMPUTERNAME");
#else
  char* hostname = getenv("HOSTNAME");
#endif //_WIN32
  if ((hostname == NULL) || ((strcmp(hostname, "") == 0))) {
    return std::string("unknown-host");
  } else {
    return std::string(hostname);
  }
}
static inline void cleanStream(void) {
  ::easyloggingpp::logStream->str("");
}
static void writeLog() {
  if (::easyloggingpp::SHOW_STD_OUTPUT && ::easyloggingpp::toStandardOutput) {
    std::cout << ::easyloggingpp::logStream->str();
  }
  if (::easyloggingpp::SAVE_TO_FILE && ::easyloggingpp::toFile) {
    std::string finalFilename = (USE_CUSTOM_LOCATION ? CUSTOM_LOG_FILE_LOCATION : "") + LOG_FILENAME;
    std::ofstream logFile(finalFilename.c_str(),
      std::ofstream::out | std::ofstream::app);
    if (logFile.is_open()) {
      logFile << ::easyloggingpp::logStream->str();
      logFile.close();
    } else {
      /* error opening log file */
    }
  }
  ::easyloggingpp::cleanStream();
}
#define WRITE_LOG(type,log) \
  if (!::easyloggingpp::streamInitialized) {\
    ::easyloggingpp::streamInitialized = true;\
    ::easyloggingpp::logStream = new std::stringstream();\
  }\
  (*::easyloggingpp::logStream) << "[" << type << "]";\
  if (::easyloggingpp::SHOW_DATE || ::easyloggingpp::SHOW_TIME) {\
    (*::easyloggingpp::logStream) << " [" << ::easyloggingpp::getDateTime() << "]";\
  }\
  if (::easyloggingpp::SHOW_USERNAME) {\
    (*::easyloggingpp::logStream) << " [" << ::easyloggingpp::getUsername() << (::easyloggingpp::SHOW_HOSTNAME ? "" : "]");\
  }\
  if (::easyloggingpp::SHOW_HOSTNAME) {\
    (*::easyloggingpp::logStream) << (::easyloggingpp::SHOW_USERNAME ? "@" : " [") << easyloggingpp::getHostname() << "]";\
  }\
  if (::easyloggingpp::SHOW_LOG_FUNCTION) {\
    (*::easyloggingpp::logStream) << " [" << __func__ << "]";\
  }\
  if (::easyloggingpp::SHOW_LOG_LOCATION) {\
    (*easyloggingpp::logStream) << " [" << __FILE__ << ":" << __LINE__ <<"]";\
  }\
    (*::easyloggingpp::logStream) << " " << log;\
    (*::easyloggingpp::logStream) << std::endl;\
    if (type == "DEBUG") { ::easyloggingpp::toStandardOutput = _DEBUG_LOGS_TO_STANDARD_OUTPUT; ::easyloggingpp::toFile = _DEBUG_LOGS_TO_FILE; }\
    if (type == "INFO") { ::easyloggingpp::toStandardOutput = _INFO_LOGS_TO_STANDARD_OUTPUT; ::easyloggingpp::toFile = _INFO_LOGS_TO_FILE; }\
    if (type == "WARNING") { ::easyloggingpp::toStandardOutput = _WARNING_LOGS_TO_STANDARD_OUTPUT; ::easyloggingpp::toFile = _WARNING_LOGS_TO_FILE; }\
    if (type == "ERROR") { ::easyloggingpp::toStandardOutput = _ERROR_LOGS_TO_STANDARD_OUTPUT; ::easyloggingpp::toFile = _ERROR_LOGS_TO_FILE; }\
    if (type == "FATAL") { ::easyloggingpp::toStandardOutput = _FATAL_LOGS_TO_STANDARD_OUTPUT; ::easyloggingpp::toFile = _FATAL_LOGS_TO_FILE; }\
    if (type == "PERFORMANCE") { ::easyloggingpp::toStandardOutput = _PERFORMANCE_LOGS_TO_STANDARD_OUTPUT; ::easyloggingpp::toFile = _PERFORMANCE_LOGS_TO_FILE; }\
    if (type == "HINT") { ::easyloggingpp::toStandardOutput = _HINTS_TO_STANDARD_OUTPUT; easyloggingpp::toFile = _HINTS_TO_FILE; }\
    if (type == "STATUS") { ::easyloggingpp::toStandardOutput = _STATUS_TO_STANDARD_OUTPUT; ::easyloggingpp::toFile = _STATUS_TO_FILE; }\
  ::easyloggingpp::writeLog();
 
  #if _ENABLE_DEBUG_LOGS
    #define DEBUG(logStr) WRITE_LOG("DEBUG",logStr)
  #else
    #define DEBUG(x)
  #endif //_ENABLE_DEBUG_LOGS
  #if _ENABLE_INFO_LOGS
    #define INFO(logStr) WRITE_LOG("INFO",logStr)
  #else
    #define INFO(x)
  #endif //_ENABLE_INFO_LOGS
  #if _ENABLE_WARNING_LOGS
    #define WARN(logStr) WRITE_LOG("WARN",logStr)
  #else
    #define WARNING(x)
  #endif //_ENABLE_WARNING_LOGS
  #if _ENABLE_ERROR_LOGS
    #define ERROR(logStr) WRITE_LOG("ERROR",logStr)
  #else
    #define ERROR(x)
  #endif //_ENABLE_ERROR_LOGS
  #if _ENABLE_FATAL_LOGS
    #define FATAL(logStr) WRITE_LOG("FATAL",logStr)
  #else
    #define FATAL(x)
  #endif //_ENABLE_FATAL_LOGS
  #if _ENABLE_PERFORMANCE_LOGS
    static inline std::string formatSeconds(double secs) {
      double result = secs;
      std::string unit = "seconds";
      std::stringstream ss;
      if (result > 60.0f) {result /= 60; unit = "minutes";
        if (result > 60.0f) {result /= 60; unit = "hours";
          if (result > 24.0f) {result /= 24; unit = "days";}
        }
      }
      ss << result << " " << unit;
      return ss.str();
    }
    #define PERFORMANCE(logStr) WRITE_LOG("PERFORMANCE",logStr)
    #define START_FUNCTION_LOG "Executing [" << __func__ << "]"
    #define TIME_OUTPUT "Executed [" << __func__ << "] in [~ " << ::easyloggingpp::formatSeconds(difftime(functionEndTime,functionStartTime)) << "]"
    #define FUNC_SUB_COMMON_START { if (::easyloggingpp::SHOW_START_FUNCTION_LOG) { PERFORMANCE(START_FUNCTION_LOG) } time_t functionStartTime, functionEndTime; time(&functionStartTime);
    #define FUNC_SUB_COMMON_END time(&functionEndTime); PERFORMANCE(TIME_OUTPUT);
    #define SUB(FUNCTION_NAME,PARAMS) void FUNCTION_NAME PARAMS FUNC_SUB_COMMON_START 
    #define END_SUB FUNC_SUB_COMMON_END }
    #define FUNC(RETURNING_TYPE,FUNCTION_NAME,PARAMS) RETURNING_TYPE FUNCTION_NAME PARAMS FUNC_SUB_COMMON_START 
    #define END_FUNC FUNC_SUB_COMMON_END }
    #define RETURN(expr) FUNC_SUB_COMMON_END return expr;
  #else
    #define PERFORMANCE(x)
    #define SUB(FUNCTION_NAME,PARAMS) void FUNCTION_NAME PARAMS {
    #define END_SUB }
    #define FUNC(RETURNING_TYPE,FUNCTION_NAME,PARAMS) RETURNING_TYPE FUNCTION_NAME PARAMS {
    #define END_FUNC }
    #define RETURN(expr) return expr;
  #endif //_ENABLE_PERFORMANCE_LOGS
  #if _ENABLE_HINTS
    #define HINT(logStr) WRITE_LOG("HINT",logStr)
  #else
    #define HINT(x)
  #endif //_ENABLE_HINTS
  #if _ENABLE_STATUS
    #define STATUS(logStr) WRITE_LOG("STATUS",logStr)
  #else
    #define STATUS(x)
  #endif //_ENABLE_STATUS
} //namespace easyloggingpp
#else
  #define DEBUG(x)
  #define INFO(x)
  #define WARN(x)
  #define ERROR(x)
  #define FATAL(x)
  #define PERFORMANCE(x)
  #define HINT(x)
  #define STATUS(x)
  #define SUB(FUNCTION_NAME,PARAMS) void FUNCTION_NAME PARAMS {
  #define END_SUB }
  #define FUNC(RETURNING_TYPE,FUNCTION_NAME,PARAMS) RETURNING_TYPE FUNCTION_NAME PARAMS {
  #define END_FUNC }
  #define RETURN(expr) return expr;
#endif //_LOGGING_ENABLED
#endif //EASYLOGGINGPP_H
