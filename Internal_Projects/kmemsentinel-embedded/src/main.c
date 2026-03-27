#include "kmemsentinel.h"
#include "kmemleak_live.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void print_summary(const char *stage) {
    kmems_summary_t summary;
    kmems_get_summary(&summary);
    printf("[%s] allocs=%u frees=%u active=%u active_bytes=%zu leaks=%u leaked_bytes=%zu\n",
           stage, summary.total_allocs, summary.total_frees, summary.active_allocs,
           summary.active_bytes, summary.suspected_leaks, summary.leaked_bytes);
}

static int run_simulation_mode(void) {
    void *sensor_buf;
    void *comm_buf;
    void *temp_buf;
    uint32_t leaks;

    kmems_init();

    sensor_buf = kmems_alloc(64U, "sensor");
    kmems_tick();
    comm_buf = kmems_alloc(128U, "comm");
    kmems_tick();
    temp_buf = kmems_alloc(32U, "temp");

    kmems_free(temp_buf);
    kmems_tick();
    kmems_tick();

    leaks = kmems_scan(2U);
    print_summary("post-scan");

    printf("suspected leak count: %u\n", leaks);
    if (leaks > 0U) {
        printf("warning: suspected memory leak(s) detected.\n");
    } else {
        printf("no leaks detected.\n");
    }

    kmems_free(sensor_buf);
    kmems_free(comm_buf);
    kmems_scan(1U);
    print_summary("after-cleanup");
    return 0;
}

static void print_live_report(const char *source_name,
                              const kmemleak_report_t *report) {
    printf("kmemleak source: %s\n", source_name);
    printf("suspected_objects: %u\n", report->suspected_objects);
    printf("suspected_bytes: %zu\n", report->suspected_bytes);
    printf("parsed_size_entries: %u\n", report->parsed_size_entries);
}

static void print_usage(const char *argv0) {
    printf("Usage:\n");
    printf("  %s --simulate\n", argv0);
    printf("  %s --live [--trigger-scan]\n", argv0);
    printf("  %s --input <kmemleak_output_file>\n", argv0);
}

int main(int argc, char **argv) {
    int run_live = 0;
    int run_sim = 0;
    int trigger_scan = 0;
    const char *input_path = NULL;
    int i;

    if (argc == 1) {
        return run_simulation_mode();
    }

    for (i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--simulate") == 0) {
            run_sim = 1;
        } else if (strcmp(argv[i], "--live") == 0) {
            run_live = 1;
        } else if (strcmp(argv[i], "--trigger-scan") == 0) {
            trigger_scan = 1;
        } else if (strcmp(argv[i], "--input") == 0) {
            if ((i + 1) >= argc) {
                fprintf(stderr, "--input requires a file path argument\n");
                return 1;
            }
            input_path = argv[i + 1];
            i++;
        } else {
            fprintf(stderr, "Unknown argument: %s\n", argv[i]);
            print_usage(argv[0]);
            return 1;
        }
    }

    if (run_sim && (run_live || input_path != NULL)) {
        fprintf(stderr, "--simulate cannot be combined with --live/--input\n");
        return 1;
    }
    if (run_live && input_path != NULL) {
        fprintf(stderr, "Use either --live or --input, not both\n");
        return 1;
    }

    if (run_sim) {
        return run_simulation_mode();
    }

    if (run_live) {
        kmemleak_report_t report;
        int rc;
        if (trigger_scan) {
            rc = kmemleak_trigger_scan(KMEMLEAK_DEFAULT_PATH);
            if (rc != 0) {
                fprintf(stderr, "failed to trigger kmemleak scan at %s: %s\n",
                        KMEMLEAK_DEFAULT_PATH, strerror(errno));
                return 1;
            }
        }
        rc = kmemleak_analyze_file(KMEMLEAK_DEFAULT_PATH, &report);
        if (rc != 0) {
            fprintf(stderr, "failed to read/analyze %s: %s\n", KMEMLEAK_DEFAULT_PATH,
                    strerror(errno));
            return 1;
        }
        print_live_report(KMEMLEAK_DEFAULT_PATH, &report);
        return 0;
    }

    if (input_path != NULL) {
        kmemleak_report_t report;
        if (kmemleak_analyze_file(input_path, &report) != 0) {
            fprintf(stderr, "failed to read/analyze %s: %s\n", input_path,
                    strerror(errno));
            return 1;
        }
        print_live_report(input_path, &report);
        return 0;
    }

    print_usage(argv[0]);
    return 1;
}
