#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "../include/unidad2.h"

void touch(char archivo[])
{
	char comando[strlen(archivo)+7];

	strcpy(comando, "touch ");
	strcat(comando, archivo);
	system(comando);
}

void *shm(int tamano, char archivo[], char clave)
{
	int shmid;
	key_t llave;

	touch(archivo);
	llave = ftok(archivo, clave);
	shmid = shmget(llave, tamano, IPC_CREAT|0600);

	return shmat(shmid, 0, 0);
}

void shmfree(void *shm, char archivo[], char clave)
{
	int shmid;
	key_t llave;

	llave = ftok(archivo, clave);
	shmid = shmget(llave, (size_t)NULL, IPC_CREAT|0600);

	shmdt(shm);
	shmctl(shmid, IPC_RMID, NULL);
	unlink(archivo);
}

int semnew(int valor_inicial, char archivo[], char clave)
{
	int semid;
	key_t llave;

	touch(archivo);
	llave = ftok(archivo, clave);
	semid = semget(llave, 1, IPC_CREAT|0644);

	if (semid == -1) {
		return -1;
	}

	semctl(semid, 0, SETVAL, valor_inicial);

	return semid;
}

int sem(char archivo[], char clave)
{
	int semid;
	key_t llave;

	touch(archivo);
	llave = ftok(archivo, clave);
	semid = semget(llave, 1, IPC_CREAT|0644);

	if (semid == -1) {
		return -1;
	}

	return semid;
}

int semval(int semid)
{
	return semctl(semid, 0, GETVAL);
}

void freesem(int semid, char archivo[], char clave)
{
	semctl(semid, 0, IPC_RMID);
	unlink(archivo);
}

bool down(int semid, short bandera)
{
	struct sembuf op_p;

	op_p.sem_num = 0;
	op_p.sem_op = -1;
	op_p.sem_flg = bandera;

	if (semop(semid, &op_p, 1) == -1 && errno == EAGAIN) {
		return false;
	}

	return true;
}

bool up(int semid, short bandera)
{
	struct sembuf op_v;
	
	op_v.sem_num = 0;
	op_v.sem_op = +1;
	op_v.sem_flg = bandera;

	if (semop(semid, &op_v, 1) == -1 && errno == EAGAIN) {
		return false;
	}

	return true;
}
