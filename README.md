# UMICH CIS450
### P4 XV6 Lottery Scheduler report


1) Edit Makefile file, add to UPROGS section:
_lotteryTest\
_ps\

add to EXTRA section:
ps.c lotteryTest.c

2) Edit ($ pico usys.S) and compile ($ gcc -S usys.S) usys.S file, add:
SYSCALL(settickets)
SYSCALL(getpinfo)
SYSCALL(yield)

3) Edit user.h file, add:

struct pstat;

int settickets(int);
int getpinfo(struct pstat*);
int yield(void);

4) Edit syscall.h file, add:
SYS_settickets 23
SYS_getpinfo 24
SYS_yield 25

5) Edit syscall.c file, add:
extern int sys_settickets(void);
extern int sys_getpinfo(void);
extern int sys_yield(void);

[SYS_settickets]   sys_settickets,
[SYS_getpinfo]     sys_getpinfo,
[SYS_yield]        sys_yield,

6) Edit sysproc.c file, add:

extern int settickets(int);
extern int getpinfo(struct pstat*);

int
sys_settickets(void){
  int tickets;
  if (argint(0, &tickets) < 0) return -1;
  return settickets(tickets);
}

int
sys_getpinfo(void){
  struct pstat* p;
  if (argptr(0,(void*)&p,sizeof(struct pstat*)) < 0) return -1;
  return getpinfo(p);
}

int
sys_yield(void){
  yield();
  return 0;
}

7) Add to VDI Home/xv6/ such files:
lotteryTest.c
pstat.h 
ps.c
random.h
proc.c
proc.h