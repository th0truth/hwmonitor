#include "base.h"
#include <signal.h>
#include <unistd.h>
#include <curl/curl.h>

#include "display.h"
#include "util.h"
#include "api/groq.h"

/* Global flag to control the watch loop */
static volatile bool keep_running = true;

/* Signal handler to ensure clean exit from watch mode */
static void
handle_sigint(int sig)
{
    (void)sig;
    keep_running = false;
}

int
main(int argc, char **argv)
{
    Config config = {0};

    /* Must initialize curl globally before using it */
    curl_global_init(CURL_GLOBAL_DEFAULT);

    /* Parsed from util.c */
    parse_arguments(argc, argv, &config);

    if (config.watch_mode) {
        signal(SIGINT, handle_sigint);
    }

    do {
        SystemHardware hw = {0};
        fetch_hardware(&config, &hw);

        if (config.watch_mode) {
            display_clear();
            printf("hwmonitor - Live View (Press Ctrl+C to exit)\n");
        }

        if (config.use_ai && config.ai_prompt != NULL) {
            /* Create context for AI analysis */
            cJSON *ctx = cJSON_CreateObject();

            /* Show hardware components */
            if (hw.battery != NULL) {
                cJSON_AddItemToObject(ctx, "battery", battery_to_json_obj(hw.battery));
            }

            if (hw.cpu != NULL) {
                cJSON_AddItemToObject(ctx, "cpu", cpu_to_json_obj(hw.cpu));
            }

            if (hw.mainboard != NULL) {
                cJSON_AddItemToObject(ctx, "mainboard", mainboard_to_json_obj(hw.mainboard));
            }

            if (hw.os != NULL) {
                cJSON_AddItemToObject(ctx, "os", os_to_json_obj(hw.os));
            }

            if (hw.ram != NULL) {
                cJSON_AddItemToObject(ctx, "ram", ram_to_json_obj(hw.ram));
            }

            if (hw.gpus != NULL && hw.gpu_count > 0) {
                cJSON *list = cJSON_CreateArray();
                for (int i = 0; i < hw.gpu_count; ++i) {
                    cJSON_AddItemToArray(list, gpu_to_json_obj(hw.gpus[i]));
                }
                cJSON_AddItemToObject(ctx, "gpus", list);
            }

            if (hw.networks != NULL && hw.network_count > 0) {
                cJSON *list = cJSON_CreateArray();
                for (int i = 0; i < hw.network_count; ++i) {
                    cJSON_AddItemToArray(list, network_to_json_obj(hw.networks[i]));
                }
                cJSON_AddItemToObject(ctx, "networks", list);
            }

            if (hw.storages != NULL && hw.storage_count > 0) {
                cJSON *list = cJSON_CreateArray();
                for (int i = 0; i < hw.storage_count; ++i) {
                    cJSON_AddItemToArray(list, storage_to_json_obj(hw.storages[i]));
                }
                cJSON_AddItemToObject(ctx, "storages", list);
            }

            char *ctx_str = cJSON_PrintUnformatted(ctx);
            groq_analyze_hardware(ctx_str, config.ai_prompt);
            free(ctx_str);
            cJSON_Delete(ctx);

            if (!config.watch_mode) {
                free(config.ai_prompt);
                config.ai_prompt = NULL;
            }
        } else if (config.use_json) {
            output_json(&config, &hw);
        } else {
            output_plaintext(&hw);
        }

        free_hardware(&hw);

        if (config.watch_mode && keep_running) {
            sleep(1);
        }

    } while (config.watch_mode && keep_running);

    if (config.output_file != NULL) {
        free(config.output_file);
    }

    if (config.ai_prompt != NULL) {
        free(config.ai_prompt);
    }

    curl_global_cleanup();
    return 0;
}
