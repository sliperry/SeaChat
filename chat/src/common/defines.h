/**
 * @file defines.h
 * @brief Global definitions for types, macros, and platform detection.
 * @version 1.0
 * @date 2024-11-21
 */

#pragma once

// Unsigned int types.
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

// Signed int types.
typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef signed long long i64;

// Floating point types.
typedef float f32;
typedef double f64;

// Boolean types.
typedef int b32;
typedef _Bool b8;

// Memory ranges.
typedef struct range {
    u64 offset;
    u64 size;
} range;

// Static assertions for type sizes.
#if defined(__clang__) || defined(__GNUC__)
#define STATIC_ASSERT _Static_assert
#else
#define STATIC_ASSERT static_assert
#endif

STATIC_ASSERT(sizeof(u8) == 1, "Expected u8 to be 1 byte.");
STATIC_ASSERT(sizeof(u16) == 2, "Expected u16 to be 2 byte.");
STATIC_ASSERT(sizeof(u32) == 4, "Expected u32 to be 4 byte.");
STATIC_ASSERT(sizeof(u64) == 8, "Expected u64 to be 8 byte.");

STATIC_ASSERT(sizeof(i8) == 1, "Expected i8 to be 1 bytes.");
STATIC_ASSERT(sizeof(i16) == 2, "Expected i16 to be 2 bytes.");
STATIC_ASSERT(sizeof(i32) == 4, "Expected i32 to be 4 bytes.");
STATIC_ASSERT(sizeof(i64) == 8, "Expected i64 to be 8 bytes.");

STATIC_ASSERT(sizeof(f64) == 8, "Expected f64 to be 8 bytes.");
STATIC_ASSERT(sizeof(f32) == 4, "Expected f32 to be 4 bytes.");

#define TRUE 1
#define FALSE 0

// Platform detection.
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) 
    #define PLATFORM_WINDOWS 1
#elif defined(__linux__)
    #define PLATFORM_LINUX 1
#elif defined(__APPLE__)
    #define PLATFORM_APPLE 1
#else
    #error "Unsupported platform detected!"
#endif

// Dynamic library export/import macros.
#ifdef KEXPORT
    // Export symbols for dynamic libraries.
    #if _MSC_VER
        #define KAPI __declspec(dllexport)
    #else
        #define KAPI __attribute__((visibility("default")))
    #endif
#else
    // Import symbols for dynamic libraries.
    #if _MSC_VER
        #define KAPI __declspec(dllimport)
    #else
        #define KAPI
    #endif
#endif

// Memory macros.
#define KCLAMP(value, min, max) ((value < min) ? min : ((value > max) ? max : value))

// Utility macros.
#define INVALID_ID_U32 4294967295U
#define GIBIBYTES(amount) ((amount) * 1024ULL * 1024ULL * 1024ULL)

// Inline qualifiers.
#if defined(__clang__) || defined(__GNUC__)
    #define KINLINE __attribute__((always_inline)) inline
#else
    #define KINLINE __forceinline
#endif

/**
 * @brief Aligns a value to the next multiple of the alignment.
 * @param operand The value to align.
 * @param granularity The alignment boundary.
 * @return The aligned value.
 */
KINLINE u64 get_aligned(u64 operand, u64 granularity) {
    return ((operand + (granularity - 1)) & ~(granularity - 1));
}
