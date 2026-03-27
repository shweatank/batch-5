#include "kmemsentinel.h"

#include <stdio.h>
#include <string.h>

static int g_failures = 0;

#define CHECK_TRUE(expr, msg)                                                   \
    do {                                                                        \
        if (!(expr)) {                                                          \
            g_failures++;                                                       \
            printf("FAIL: %s\n", msg);                                          \
        }                                                                       \
    } while (0)

static void test_basic_alloc_free(void) {
    kmems_summary_t s;
    void *p;

    kmems_init();
    p = kmems_alloc(48U, "basic");
    CHECK_TRUE(p != NULL, "allocation should succeed");

    kmems_get_summary(&s);
    CHECK_TRUE(s.total_allocs == 1U, "total_allocs should be 1");
    CHECK_TRUE(s.active_allocs == 1U, "active_allocs should be 1");
    CHECK_TRUE(s.active_bytes == 48U, "active_bytes should match allocation");

    kmems_free(p);
    kmems_get_summary(&s);
    CHECK_TRUE(s.total_frees == 1U, "total_frees should be 1");
    CHECK_TRUE(s.active_allocs == 0U, "active_allocs should be 0 after free");
    CHECK_TRUE(s.active_bytes == 0U, "active_bytes should be 0 after free");
}

static void test_leak_detection(void) {
    kmems_summary_t s;
    void *a;
    void *b;
    uint32_t leaks;

    kmems_init();
    a = kmems_alloc(10U, "a");
    kmems_tick();
    b = kmems_alloc(20U, "b");
    kmems_tick();
    kmems_tick();

    leaks = kmems_scan(2U);
    CHECK_TRUE(leaks == 2U, "both allocations should be suspected leaks");
    kmems_get_summary(&s);
    CHECK_TRUE(s.suspected_leaks == 2U, "summary leak count mismatch");
    CHECK_TRUE(s.leaked_bytes == 30U, "summary leaked_bytes mismatch");

    kmems_free(a);
    kmems_free(b);
}

static void test_tag_copy(void) {
    kmems_record_t records[KMEMS_MAX_TRACKED];
    void *p;
    uint32_t n;

    kmems_init();
    p = kmems_alloc(16U, "long_tag_name_for_sensor_channel");
    CHECK_TRUE(p != NULL, "allocation should succeed");

    n = kmems_dump_records(records, KMEMS_MAX_TRACKED);
    CHECK_TRUE(n == 1U, "one record expected");
    CHECK_TRUE(strncmp(records[0].tag, "long_tag_name_for_sensor_channel", 31U) == 0,
               "tag should be copied/truncated safely");

    kmems_free(p);
}

int main(void) {
    test_basic_alloc_free();
    test_leak_detection();
    test_tag_copy();

    if (g_failures == 0) {
        printf("All tests passed.\n");
        return 0;
    }

    printf("Tests failed: %d\n", g_failures);
    return 1;
}
