#include <signal.h>
#include <unistd.h>
#include <curl/curl.h>

#include "display.h"
#include "theme.h"
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

    /* Initialize theme colors based on TTY check */
    theme_init(false);

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
            cJSON *ctx = hardware_to_json(&hw);
            if (ctx != NULL) {
                char *ctx_str = cJSON_PrintUnformatted(ctx);
                groq_analyze_hardware(ctx_str, config.ai_prompt);
                free(ctx_str);
                cJSON_Delete(ctx);
            }

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
