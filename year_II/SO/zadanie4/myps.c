#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <minix/config.h>
#include <minix/callnr.h>
#include <minix/com.h>

#include "pm.h"
#include "mproc.h"


int do_myps(void) {
    size_t it;
    int uid = m_in.m1_i1;
    struct mproc *rmp = mp;

    if (uid == 0) {
      uid = (int) rmp->mp_realuid;
    }

    printf("pid\tppid\tuid\n");

    for (it = 0; it < NR_PROCS; it++) {

        if ((int) mproc[it].mp_pid == 0)
            continue;

        if ((int) mproc[it].mp_realuid == uid) {
            int parent_index = (int) mproc[it].mp_parent;
            printf("%d\t%d\t%d\n", (int) mproc[it].mp_pid,
                                 (int) mproc[parent_index].mp_pid,
                                 uid);
        }
    }

    return 0;
}
