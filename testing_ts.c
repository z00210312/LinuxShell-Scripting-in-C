// shm_ts2.c -- time server shared mem ver2 : use semaphores for locking
// program uses shared memory with key 99
// program uses semaphore set with key 9906

#include "stdio.h"
#include "stdlib.h"
#include "sys/shm.h"
#include "time.h"
#include "sys/types.h"
#include "sys/sem.h"
#include "signal.h"
#include "string.h"

#define TIME_MEM_KEY 99						  // like a filename
#define TIME_SEM_KEY 9906
#define SEG_SIZE ( (size_t)100 )			  // size of segment
#define oops( m, x ) { perror(m); exit(x); }

// semaphore union
union semun { int val; struct semid_ds* buf; short* array; };

int seg_id, semset_id;					      // global for cleanup()

void cleanup(int);

char* ctime();
void cleanup(int);
void set_sem_value(int, int, int);
void wait_and_lock_WD(int);
void release_lock_WD(int);
void wait_and_lock_RD(int);
void release_lock_RD(int);

int main()
{
	signal(SIGINT, cleanup);                  // ctrl-C calls cleanup()
	char a[100];
	char* mem_ptr;
	time_t now;
	int n;

	// create a shared memory segment
	seg_id = shmget(TIME_MEM_KEY, SEG_SIZE, IPC_CREAT | 0777);

	if (seg_id == -1)
		oops("shmget", 1);
	// attach to it and get a pointer to where it attaches
	mem_ptr = shmat(seg_id, NULL, 0);

	if (mem_ptr == (void*)-1)
		oops("shmat", 2);
	// Thing 1: create a semset: key 9900, 2 semaphores, and mode rw-rw-rw
	semset_id = semget(TIME_SEM_KEY, 2, (0666 | IPC_CREAT | IPC_EXCL));

	if (semset_id == -1)
		oops("semget", 3);

	// Thing 2: Set both semaphores to 0:
	set_sem_value(semset_id, 0, 0);		  // set counters
	set_sem_value(semset_id, 1, 0);		  // both to zero
	while(1)
	{
		time(&now);						  // get the time

		wait_and_lock_RD(semset_id);
		//printf("direct from memory: ..%s\n", mem_ptr);
		if (strcmp(mem_ptr, "HI") == 0)
			printf("Greeting!\n");
		if (strcmp(mem_ptr, "PID") == 0)
			printf("Server pid : %d!\n",getpid());
		if (strcmp(mem_ptr, "QUIT") == 0){
			printf("GoodBye!\n");
			release_lock_RD(semset_id);
			break;
		}
		release_lock_RD(semset_id);

		if (strcmp(mem_ptr, "") != 0) {
			wait_and_lock_WD(semset_id);
			strcpy(mem_ptr, "");
			release_lock_WD(semset_id);
		}
	}
	cleanup(0);
	return 0;
}

// Thing 5: Delete the semaphore
void cleanup(int n)
{
	shmctl(seg_id, IPC_RMID, NULL);		  // remove shared memory
	semctl(semset_id, 0, IPC_RMID, NULL);	  // remove semaphore set
}

// Thing 2:  initialize a semaphore
void set_sem_value(int semset_id, int semnum, int val)
{
	union semun initval;

	initval.val = val;

	if (semctl(semset_id, semnum, SETVAL, initval) == -1)
		oops("semctl", 4);
}

void wait_and_lock_RD(int semset_id)
{
	union semun sem_info;					  // some properties
	struct sembuf actions[2];				  // action set, an array

	actions[0].sem_num = 1;					  // sem[1] is n_writers
	actions[0].sem_flg = SEM_UNDO;			  // auto cleanup
	actions[0].sem_op = 0;					  // wait for 0
	actions[1].sem_num = 0;					  // sem[0] is n_readers
	actions[1].sem_flg = SEM_UNDO;			  // auto cleanup
	actions[1].sem_op = 1;					  // incr n_readers

	if (semop(semset_id, actions, 2) == -1)
		oops("semop: locking", 10);
}

void release_lock_RD(int semset_id)
{
	union semun sem_info;					  // some properties
	struct sembuf actions[1];				  // action set

	actions[0].sem_num = 0;					  // sem[0] is n_readers
	actions[0].sem_flg = SEM_UNDO;			  // auto cleanup
	actions[0].sem_op = -1;					  // decr reader country

	if (semop(semset_id, actions, 1) == -1)
		oops("semop: unlocking", 10);
}

// Thing 3:  build and execute a 2-element action set:
//     wait for 0 on n_readers AND increment n_writers
void wait_and_lock_WD(int semset_id)
{
	struct sembuf actions[2];				  // action set, an array

	actions[0].sem_num = 0;					  // sem[0] is n_readers
	actions[0].sem_flg = SEM_UNDO;			  // auto cleanup
	actions[0].sem_op = 0;				      // wait til no readers
	actions[1].sem_num = 1;                   // sem[1] is n_writers
	actions[1].sem_flg = SEM_UNDO;		      // auto cleanup
	actions[1].sem_op = 1;					  // increment number of writers

	if (semop(semset_id, actions, 2) == -1)
		oops("semop: locking", 10);
}

// Thing 4:  build and execute a 1-element action set: decrement num_writers
void release_lock_WD(int semset_id)
{
	struct sembuf actions[1];				  // action set, an array
	actions[0].sem_num = 1;					  // sem[0] is n_writers
	actions[0].sem_flg = SEM_UNDO;			  // auto cleanup
	actions[0].sem_op = -1;					  // decrement number of writer count

	if (semop(semset_id, actions, 1) == -1)
		oops("semop: unlocking", 10);
}
