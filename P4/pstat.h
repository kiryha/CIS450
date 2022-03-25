#ifndef _PSTAT_H_
#define _PSTAT_H_

#include "param.h"

struct pstat {
    int num_processes;
    int pid[NPROC];       // the PID of each process
    int tickets[NPROC];   // the number of tickets this process has
    int ticks[NPROC];     // the number of ticks each process has accumulated
};

// Extend functions visibility: pstat visible to getpinfo, tickets visible to settickets
extern int getpinfo(struct pstat* p);
extern int settickets(int tickets);

#endif // _PSTAT_H_
