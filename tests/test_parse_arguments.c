#include <assert.h>
#include <unistd.h>
#include "util.h"

int
main(void)
{
    Config config = {0};
    char *argv[] = {"hwmonitor", "--cpu", "--json"};

    optind = 1;
    parse_arguments(3, argv, &config);

    assert(config.show_cpu);
    assert(!config.show_gpu);
    assert(config.use_json);
    assert(!config.watch_mode);

    return 0;
}
