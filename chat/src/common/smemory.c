#include "smemory.h"

#include "logger.h"
#include "cstring.h"
#include "platform/platform.h"

#include <string.h>
#include <stdio.h>

struct memory_stats {
    u64 total_allocated;
    u64 tagged_allocations[MEMORY_TAG_MAX_TAGS];
};

static struct memory_stats stats;

static const char* memory_tag_strings[MEMORY_TAG_MAX_TAGS] = {
    "UNKNOWN    ",
    "ARRAY      ",
    "DARRAY     ",
    "DICT       ",
    "RING_QUEUE ",
    "BST        ",
    "STRING     ",
    "APPLICATION",
    "JOB        ",
    "TEXTURE    ",
    "MAT_INST   ",
    "RENDERER   ",
    "CHAT       ",
    "TRANSFORM  ",
    "ENTITY     ",
    "ENTITY_NODE",
    "SCENE      "
};

void initialize_memory() {
    platform_zero_memory(&stats, sizeof(stats));
}

void shutdown_memory() {

}

void* sallocate(u64 size, memory_tag tag) {
    if (tag == MEMORY_TAG_MAX_TAGS) {
        KWARN("sallocate called using the MEMORY_TAG_MAX_TAGS tag. Re-class this allocation.");
    }

    stats.total_allocated += size;
    stats.tagged_allocations[tag] += size;

    void* block = platform_allocate(size, FALSE);
    platform_zero_memory(block, size);

    return block;
}

void sfree(void* block, u64 size, memory_tag tag) {
    if (tag == MEMORY_TAG_MAX_TAGS) {
        KWARN("sallocate called using the MEMORY_TAG_MAX_TAGS tag. Re-class this allocation.");
    }

    stats.total_allocated -= size;
    stats.tagged_allocations[tag] -= size;

    platform_free(block, FALSE);
}

void* szero_memory(void* block, u64 size) {
    return platform_zero_memory(block, size);
}

void* scopy_memory(void* dest, const void* source, u64 size) {
    return platform_copy_memory(dest, source, size);
}

void* sset_memory(void* dest, i32 value, u64 size) {
    return platform_set_memory(dest, value, size);
}

char* get_memory_usage_str() {
    range buffer_range = { .offset = 0, .size = 8000 };
    char buffer[8000] =  "System memory use (tagged):\n";
    buffer_range.offset = strlen(buffer);

    for (u32 i = 0; i < MEMORY_TAG_MAX_TAGS; ++i) {
        char unit[4] = "XiB";
        f32 amount = 1.0f;

        if (stats.tagged_allocations[i] >= GIBIBYTES(1)) {
            unit[0] = 'G';
            amount = (f32)stats.tagged_allocations[i] / (f32)GIBIBYTES(1);
        } else if (stats.tagged_allocations[i] >= MIBIBYTES(1)) {
            unit[0] = 'M';
            amount = (f32)stats.tagged_allocations[i] / (f32)MIBIBYTES(1);
        } else if (stats.tagged_allocations[i] >= KILOBYTES(1)) {
            unit[0] = 'K';
            amount = (f32)stats.tagged_allocations[i] / (f32)KILOBYTES(1);
        } else {
            unit[0] = 'B';
            unit[1] = '\0';
            amount = (f32)stats.tagged_allocations[i];
        }

        i32 length = snprintf(
            buffer + buffer_range.offset,
            buffer_range.size - buffer_range.offset,
            "  %s: %.2f%s\n",
            memory_tag_strings[i],
            amount,
            unit
        );

        buffer_range.offset += length;
        buffer_range.offset = KCLAMP(buffer_range.offset, 0, buffer_range.size - 1);
    }
    
    char* out_string = string_duplicate(buffer);
    return out_string;
}