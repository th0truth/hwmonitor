#pragma once

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdint.h>
#include <stdbool.h>
#include <cJSON.h>

typedef struct {
    char *vendor_id;
    char *model_name;
    char *flags;
    char *arch;
    float max_MHz;
    float min_MHz;
    float curr_temp;
    float curr_usage;
    uint16_t cpu_family;
    uint16_t model;
    uint16_t stepping;
    uint16_t total_cores;
    uint16_t total_threads;
    uint16_t online_cores;
} CPU;

typedef struct {
    uint64_t user;
    uint64_t nice;
    uint64_t system;
    uint64_t idle;
    uint64_t iowait;
    uint64_t irq;
    uint64_t softirq;
    uint64_t steal;
} CPUTimes;

cJSON *cpu_to_json_obj(const CPU *cpu);

CPU *cpu_get_info(void);

void free_cpu(CPU *cpu);

#ifdef __cplusplus
}
#endif /* __cplusplus */
