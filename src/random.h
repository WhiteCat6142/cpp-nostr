#include <sys/random.h>
static int fill_random(unsigned char* data, size_t size) {
    ssize_t res = getrandom(data, size, 0);
    if (res < 0 || (size_t)res != size ) {
        return 0;
    } else {
        return 1;
    }
}