#ifndef COMMON_H
#define COMMON_H

#include <stddef.h>
#include <stdint.h>

#define SHM_NAME            "/shm_3col_buf"
#define SEM_NAME_FREE       "/sem_3col_free"
#define SEM_NAME_USED       "/sem_3col_used"
#define SEM_NAME_WRITER     "/sem_3col_writer"

#define MAX_SOLUTION_EDGES  8

#define CBUF_CAPACITY       48

#endif /* COMMON_H */