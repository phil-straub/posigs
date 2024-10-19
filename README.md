# posigs - Asynchronous Signal Handling on POSIX Systems

The `posigs` C-library replaces the unpredictable and platform-dependent signal handling mechanism
provided by the C and POSIX standards with a well-defined asynchronous interface using a dedicated
signal handling thread (following a suggestion on stackoverflow.com).

## Building the Library and Demo

The library provides a single header (`src/posigs.h`) and source file (`src/posigs.c`) as well as the
source code for a demo program (`src/main.c`).
Due to their small size, it is recommended to simply include the library files directly in projects
rather than building a static or dynamic library.
Compiling the source files requires a compiler with (partial) support for C23 and POSIX,
e.g. gcc 14.2.1 or clang 18.1.8. In either case the following compiler flags are recommended:

~~~
    -fPIC -std=gnu2x -pthread
~~~

For convenience, a Makefile is included. Ensure that `make` is installed and run `make` to
compile the demo executable or run `make lib` to compile only the single object file containing the
compiled library code. See the Makefile for additional information.

## Runing the Demo

After `make` run either `make run` or `build/posigs` to try out the demo.
Entering `ctrl + c` should generate a terminal message rather than terminating the program.
Press `enter` to quit the demo.

## Using the Library

The library provides only three function, namely

~~~C
    bool posigs_create_handler(PosigsHandler * handler);                                                                                     
    bool posigs_destroy_handler(PosigsHandler * handler);                                                 
    bool posigs_handler_is_running(PosigsHandler * handler);
~~~

for creating a signal handler, terminating a signal handler and obtaining the current state of a
signal handler (running/not running), respectively.

### Creating a Signal Handler

Fill in the fields `sig_set`, `sig_action`, `sig_action_arg` and `timeout` of a
`PosigsHandler` struct and pass a pointer to the struct to `posigs_create_handler`.
The pointer must remain valid while the signal handler is active
and should not be touched by any non-library function.
The meaning of these fields is as follows:

+ `sig_set` describes which signals should be handled by the signal handler (see references at the end).
+ `sig_action` is a function pointer `void (*)(int signum, void * arg)`
that is executed whenever a signal is received.
The first argument identifies the signal type, the second is an arbitrary parameter.
+ `sig_action_arg` is a `void`-pointer that is passed to `sig_action`.
+ `timeout` is a struct `timespec` (see references) determining the timeout periode during signal handling.
In practice, this sets the 'reaction time' when the signal handler is terminated.
A larger timeout period may also slightly increase performance. It must not be zero.

Other fields of the struct must never be set manually. The function call will return `true`/`false`
according to whether creating the signal handler succeeded/failed.

## Destroying a Signal Handler

To stop a signal handler, pass a pointer to the `PosigsHandler` struct to `posigs_destroy_handler`.
If the signal handler encountered an error earlier or if a call to the threading library failed, this
function will return `false` and otherwise `true`.

## Querying the State of the Signal Handler

Call `posigs_handler_is_running` on a signal handler to check whether it is still running.
If it is not, either an error occured or it was passed to `posigs_destroy_handler` earlier.

## Notes

For an example, see `src/main.c`.

It is recommended to only use a single signal handler per process.

For C++ projects the header should be included as

~~~Cpp
    extern "C" {#include "posigs.h"}
~~~

(not tested yet!).

## References

+ [Linux Man Page for signal.h](https://www.man7.org/linux/man-pages/man2/signal.2.html)
+ [POSIX Man Page for sigtimedwait](https://www.man7.org/linux/man-pages/man3/sigtimedwait.3p.html)
+ [POSIX Man Page for pthread_sigmask](https://www.man7.org/linux/man-pages/man3/pthread_sigmask.3p.html)
+ [LINUX Man Page for timespec](https://www.man7.org/linux/man-pages/man3/timespec.3type.html)
+ [GNU libc Manual for singal sets](https://www.gnu.org/software/libc/manual/html_node/Signal-Sets.html)
+ [GNU libs Manual for ISO threads](https://www.gnu.org/software/libc/manual/html_node/Threads.html)
