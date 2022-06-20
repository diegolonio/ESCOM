#ifndef _UNIDAD2_H_
#define _UNIDAD2_H_

void touch(char []);
void *shm(int, char [], char);
void shmfree(void *, char [], char);
int semnew(int, char [], char);
int sem(char [], char);
int semval(int);
void freesem(int, char [], char);
bool down(int, short);
bool up(int, short);

#include "../src/unidad2.c"

#endif /* _UNIDAD2_H_ */
