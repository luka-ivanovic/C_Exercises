#define _POSIX_C_SOURCE 200809L
#include "circ_buf.h"

#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

int init_shm(circ_buf_t **buf, int create) {
    int flags = O_RDWR;
    if (create) flags |= O_CREAT;

    int fd = shm_open(SHM_NAME, flags, 0666);
    if (fd == -1) { perror("shm_open"); return -1; }

    if (create) {
        if (ftruncate(fd, (off_t)sizeof(circ_buf_t)) == -1) {
            perror("ftruncate"); close(fd); return -1;
        }
    }

    void *addr = mmap(NULL, sizeof(circ_buf_t),
                      PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);
    if (addr == MAP_FAILED) { perror("mmap"); return -1; }

    *buf = (circ_buf_t *)addr;
    if (create) {
        memset(*buf, 0, sizeof(circ_buf_t));
        (*buf)->stop = 0;
        (*buf)->readHead = 0;
        (*buf)->writeHead = 0;
    }
    return 0;
}