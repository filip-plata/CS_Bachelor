#include <unistd.h>
#include <stdlib.h>

pid_t getpid() {
    return (pid_t) atoi(getenv("MYPID"));
}
