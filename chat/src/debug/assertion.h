/**
 * @file assertion.h
 * @brief Assertions for runtime error checking and debugging
 * @version 1.0
 * @date 2024-11-22
 */

#pragma once

#include "../common/logger.h"

/// Enable assertions by default for debug builds.
// To disable, comment out the line below.
#define KASSERTIONS_ENABLED

// Debug break macro to halt execution in debuggers.
#ifdef KASSERTIONS_ENABLED
#if _MSC_VER
#include <intrin.h>
#define debugBreak() __debugbreak()
#else
#define debugBreak() __builtin_trap()
#endif

/**
 * @brief Reports an assertion failure.
 * @param expression The failed expression as a string.
 * @param message An optional message providing context.
 * @param file The file in which the assertion failed.
 * @param line The line number at which the assertion failed.
 */
KAPI void report_assertion_failure(const char* expression, const char* message, const char* file, i32 line);

/**
 * @brief Basic assertion macro.
 * @param expr The expression to evaluate.
 */
#define KASSERT(expr)                                                \
    {                                                                \
        if (expr) {                                                  \
        } else {                                                     \
            report_assertion_failure(#expr, "", __FILE__, __LINE__); \
            debugBreak();                                            \
        }                                                            \
    }

/**
 * @brief Assertion with a custom message.
 * @param expr The expression to evaluate.
 * @param message Custom failure message.
 */
#define KASSERT_MSG(expr, message)                                        \
    {                                                                     \
        if (expr) {                                                       \
        } else {                                                          \
            report_assertion_failure(#expr, message, __FILE__, __LINE__); \
            debugBreak();                                                 \
        }                                                                 \
    }

#ifdef _DEBUG
#define KASSERT_DEBUG(expr)                                          \
    {                                                                \
        if (expr) {                                                  \
        } else {                                                     \
            report_assertion_failure(#expr, "", __FILE__, __LINE__); \
            debugBreak();                                            \
        }                                                            \
    }
#else
#define KASSERT_DEBUG(expr)  // Does nothing at all
#endif

#else
#define KASSERT(expr)               // Does nothing at all
#define KASSERT_MSG(expr, message)  // Does nothing at all
#define KASSERT_DEBUG(expr)         // Does nothing at all
#endif