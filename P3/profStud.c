#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#define SHARED 1

void *Student (void* arg); // the two threads
void *Professor (void* arg);
void* AnswerStart();
void* AnswerDone();
void* QuestionStart(int n);
void* QuestionDone(int n);

sem_t questionAsked, answerGiven, spacePresent, profNap, stuReady;       //global semaphores
int idTotal = 0;
int data;                // shared buffer, size = 1
int numStudents;
pthread_mutex_t mutex;
int stuCond, profAnswer = 0;

// main() -- read command line and create threads
int main(int argc, char *argv[]) {

    pthread_t prof, stud;

    sem_init(&answerGiven, SHARED, 1);    //sem answer given = 1
    sem_init(&questionAsked, SHARED, 0); //sem question asked = 0
    sem_init(&spacePresent, SHARED, 0);
    sem_init(&profNap, SHARED, 0);
    sem_init(&stuReady, SHARED, 1);

    if (argc < 1) {
	    printf("Usage: profstud <Number of Students>\n");
	    printf(" e.g.: ./profstud 3 \n");
	    exit(0);
    }
    numStudents = atoi(argv[1]);

    printf("Running program with %d students\n", numStudents);

    pthread_mutex_init(&mutex, NULL);               // Init mutex




    pthread_create(&prof, NULL, Professor, NULL);

    for (int i = 0; i < numStudents; i++) {
        pthread_create(&stud, NULL, Student, NULL);
    }

    pthread_join(prof, NULL);
    pthread_exit(0);
}


void *Student(void* arg) {

    int id = idTotal;
    idTotal++;
    printf("Student % d : The student is ready to ask a question\n", id);

    if (id != 0) {

        sem_wait(&spacePresent);

    }

    QuestionStart(id);
    printf("Student % d : The student is asking a question\n", id);
    QuestionDone(id);
    printf("Student % d : The student receives the answer and leaves\n", id);
    sem_post(&answerGiven);
    sem_post(&spacePresent);
    sched_yield();
}


void *Professor(void* arg) {
    for (int i = 0; i < numStudents; i++) {

        printf("The professor wants to be asked a question.\n");
        AnswerStart();
        printf("The professor is answering the question\n");
        AnswerDone();
        sched_yield();

    }

    sem_post(&profNap);

}

void* AnswerStart() {

    sem_wait(&questionAsked);
    sched_yield();

}
void* AnswerDone() {

    printf("The professor is finished with an answer\n");
    sem_post(&answerGiven);
    sched_yield();

}
void* QuestionStart(int n) {

    sem_wait(&answerGiven);
    sched_yield();

}
void* QuestionDone(int n) {

    printf("Student % d : The student is done asking a question\n", n);
    sem_post(&questionAsked);
    sem_wait(&answerGiven);

}