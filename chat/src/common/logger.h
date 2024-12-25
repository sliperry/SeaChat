/**
 * @file logger.h
 * @brief Logging system for the project.
 */

#pragma once

#include "defines.h"

#define LOG_WARN_ENABLED 1
#define LOG_INFO_ENABLED 1
#define LOG_DEBUG_ENABLED 1
#define LOG_TRACE_ENABLED 1

// Disable debug and trace logging for release builds.
#if KRELEASE == 1
#define LOG_DEBUG_ENABLED 0
#define LOG_TRACE_ENABLED 0
#endif

// Logging levels
typedef enum log_level {
    LOG_LEVEL_FATAL = 0,
    LOG_LEVEL_ERROR = 1,
    LOG_LEVEL_WARN  = 2,
    LOG_LEVEL_INFO  = 3,
    LOG_LEVEL_DEBUG = 4,
    LOG_LEVEL_TRACE = 5
} log_level;

/**
 * @brief Sets a custom console write hook.
 * @param hook The custom hook function.
 */
b8 initialize_logging();
void shutdown_logging();

/**
 * @brief Logs a formatted message at the given log level.
 * @param level The log level.
 * @param message The format string.
 * @param ... Additional arguments.
 */
KAPI void _log_output(log_level level, const char* message, ...);

// Logging macros
#define KFATAL(message, ...) _log_output(LOG_LEVEL_FATAL, message, ##__VA_ARGS__);

#ifndef KERROR
#define KERROR(message, ...) _log_output(LOG_LEVEL_ERROR, message, ##__VA_ARGS__);
#endif

#if LOG_WARN_ENABLED == TRUE
#define KWARN(message, ...)  _log_output(LOG_LEVEL_WARN, message, ##__VA_ARGS__);
#else
#define KWARN(message, ..)
#endif

#if LOG_INFO_ENABLED == TRUE
#define KINFO(message, ...)  _log_output(LOG_LEVEL_INFO, message, ##__VA_ARGS__);
#else
#define KINFO(message, ..)
#endif

#if LOG_DEBUG_ENABLED == TRUE
#define KDEBUG(message, ...) _log_output(LOG_LEVEL_DEBUG, message, ##__VA_ARGS__);
#else
#define KDEBUG(message, ..)
#endif

#if LOG_TRACE_ENABLED ==TRUE
#define KTRACE(message, ...) _log_output(LOG_LEVEL_TRACE, message, ##__VA_ARGS__);
#else
#define KTRACE(message, ...)
#endif