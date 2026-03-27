#include "kmemleak_live.h"

#include <stdio.h>

static int g_failures = 0;

#define CHECK_TRUE(expr, msg)                                                   \
    do {                                                                        \
        if (!(expr)) {                                                          \
            g_failures++;                                                       \
            printf("FAIL: %s\n", msg);                                          \
        }                                                                       \
    } while (0)

static void test_analyze_text(void) {
    const char *sample =
        "unreferenced object 0xaaa (size 32):\n"
        "unreferenced object 0xbbb (size 16):\n";
    kmemleak_report_t report;
    int rc = kmemleak_analyze_text(sample, &report);
    CHECK_TRUE(rc == 0, "analyze_text should return success");
    CHECK_TRUE(report.suspected_objects == 2U, "expected two objects");
    CHECK_TRUE(report.suspected_bytes == 48U, "expected total size of 48");
    CHECK_TRUE(report.parsed_size_entries == 2U, "expected two parsed size entries");
}

static void test_analyze_file_fixture(void) {
    kmemleak_report_t report;
    int rc = kmemleak_analyze_file("tests/fixtures/kmemleak_sample.txt", &report);
    CHECK_TRUE(rc == 0, "analyze_file fixture should return success");
    CHECK_TRUE(report.suspected_objects == 2U, "fixture should have two objects");
    CHECK_TRUE(report.suspected_bytes == 192U, "fixture bytes should sum to 192");
    CHECK_TRUE(report.parsed_size_entries == 2U, "fixture should parse two size values");
}

int main(void) {
    test_analyze_text();
    test_analyze_file_fixture();

    if (g_failures == 0) {
        printf("All kmemleak parser tests passed.\n");
        return 0;
    }
    printf("Tests failed: %d\n", g_failures);
    return 1;
}
