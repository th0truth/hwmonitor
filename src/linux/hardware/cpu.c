#include "base.h"
#include "file.h"
#include "io.h"
#include <string.h>
#include "cpu.h"

static int16_t
cpu_get_total_cores(void)
{
    char *online = file_read_stripped("/sys/devices/system/cpu/online", "\n", false);
    if (online == NULL) {
        return -1;
    }

    int start, end;
    int16_t count = 0;

    char *saveptr = NULL;
    char *token = strtok_r(online, ",", &saveptr); 
    
    while (token != NULL) {
         if (sscanf(token, "%d-%d", &start, &end) == 2) {
            if (end >= start) {
                count += (end - start) + 1;
            }
        } else if (sscanf(token, "%d", &start) == 1) {
            count += 1;
        }
        
        token = strtok_r(NULL, ",", &saveptr); 
    }

    free(online);
    return count;
}

static float
cpu_get_total_freq_mhz(uint16_t core_id, const char *filename)
{
    char buffer[BUFFER_SIZE];
    snprintf(buffer, sizeof(buffer), "/sys/devices/system/cpu/cpu%u/cpufreq/%s", core_id, filename);

    char *cpu_freq = file_read_stripped(buffer, "\n", true);
    if (cpu_freq == NULL) {
        return -1;
    }

    int32_t khz = atoi(cpu_freq);
   
    free(cpu_freq);

    if (khz <= 0) {
        return -1;
    }

    return khz / 1000.0f;
}

static char *
cpu_get_arch(const char *flags)
{
    if (flags == NULL) {
        return strdup("x86");
    }

    char *flag = str_find_value(flags, "lm", NULL);
    if (flag == NULL) {
        return strdup("x86"); 
    }

    free(flag);

    return strdup("x86_64"); 
}

CPU *
cpu_get_info(void)
{
    CPU *cpu = calloc(1, sizeof(CPU));
    if (cpu == NULL) {
        return NULL;
    }

    char *cpu_info = file_read_stripped("/proc/cpuinfo", ":\t", false);
    if (cpu_info == NULL) {
        free(cpu);
        return NULL;
    }

    cpu->vendor_id      = str_find_value(cpu_info, "vendor_id", "\n");
    cpu->model_name     = str_find_value(cpu_info, "model name", "\n");
    cpu->flags          = str_find_value(cpu_info, "flags", "\n");
    cpu->arch           = cpu_get_arch(cpu->flags);

    cpu->online_cores   = cpu_get_total_cores();
    if (cpu->online_cores > 0) {
        cpu->max_MHz    = cpu_get_total_freq_mhz((cpu->online_cores - 1), "scaling_max_freq");
        cpu->min_MHz    = cpu_get_total_freq_mhz((cpu->online_cores - 1), "scaling_min_freq");
    } else {
        cpu->max_MHz    = 0;
        cpu->min_MHz    = 0;
    }

    cpu->cpu_family     = str_parse_value(cpu_info, "cpu family", "\n");
    cpu->model          = str_parse_value(cpu_info, "model", "\n");
    cpu->stepping       = str_parse_value(cpu_info, "stepping", "\n");
    cpu->total_cores    = str_parse_value(cpu_info, "cpu cores", "\n");
    cpu->total_threads  = str_parse_value(cpu_info, "siblings", "\n");

    free(cpu_info);
    return cpu;
}

void
free_cpu(CPU *cpu)
{
    if (cpu == NULL) {
        return;
    }
    free(cpu->vendor_id);
    free(cpu->model_name);
    free(cpu->flags);
    free(cpu);
}

cJSON *
cpu_to_json_obj(const CPU *cpu)
{
    cJSON *obj = cJSON_CreateObject();
    if (cpu == NULL) {
        return obj;
    }

    cJSON_AddStringToObject(obj, "vendor", STR_OR_UNK(cpu->vendor_id));
    cJSON_AddStringToObject(obj, "model_name", STR_OR_UNK(cpu->model_name));
    cJSON_AddStringToObject(obj, "flags", STR_OR_UNK(cpu->flags));
    cJSON_AddStringToObject(obj, "arch", STR_OR_UNK(cpu->arch));

    cJSON_AddNumberToObject(obj, "max_freq_mhz", cpu->max_MHz);
    cJSON_AddNumberToObject(obj, "min_freq_mhz", cpu->min_MHz);
    cJSON_AddNumberToObject(obj, "online_cores", cpu->online_cores);
    cJSON_AddNumberToObject(obj, "cpu_family", cpu->cpu_family);
    cJSON_AddNumberToObject(obj, "model", cpu->model);
    cJSON_AddNumberToObject(obj, "stepping", cpu->stepping);
    cJSON_AddNumberToObject(obj, "total_cores", cpu->total_cores);
    cJSON_AddNumberToObject(obj, "total_threads", cpu->total_threads);

    return obj;
}
