#include <stdlib.h>
#include <stdio.h>

#include "posigs.h"

static void sig_action(int signum, void * arg);

int main(int argc, char * argv[])
{
    sigset_t sig_set;
    sigemptyset(&sig_set);
    if (sigaddset(&sig_set, SIGINT) != 0 || sigaddset(&sig_set, SIGQUIT) != 0) {
        fprintf(stderr, "Error: Unable to initialize signal set!\n");
        return EXIT_FAILURE;
    }

    atomic_bool is_running = true;

    PosigsHandler handler = {
        .sig_set = &sig_set,
        .sig_action = sig_action,
        .sig_action_arg = (void*) &is_running,
        .timeout = {.tv_nsec = 1000'000}
    };

    if (!posigs_create_handler(&handler)) {
        fprintf(stderr, "Error: Unable to create signal handler!\n");
    }

    fgetc(stdin);

    if (!posigs_handler_is_running(&handler)) {
        fprintf(stderr, "Error: Signal handler is not running!\n");
    }

    posigs_destroy_handler(&handler);

    return EXIT_SUCCESS;
}

static void sig_action(int signum, void * arg)
{
    fprintf(stderr, "\nsignal: %d\n", signum);
}
