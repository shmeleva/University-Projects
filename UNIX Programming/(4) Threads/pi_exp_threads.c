#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>

// Compile command:
// cc -lpthread -lm pi_exp_threads.c -o pi_exp_threads

// Barriers are defined in the optional part of POSIX standard
// and are not implemented in Mac OS X so I found this implementation:

#ifdef __APPLE__

#ifndef PTHREAD_BARRIER_H_
#define PTHREAD_BARRIER_H_

#include <pthread.h>
#include <errno.h>

typedef int pthread_barrierattr_t;
typedef struct
{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int count;
    int tripCount;
} pthread_barrier_t;


int pthread_barrier_init(pthread_barrier_t *barrier, const pthread_barrierattr_t *attr, unsigned int count)
{
    if(count == 0)
    {
        errno = EINVAL;
        return -1;
    }
    if(pthread_mutex_init(&barrier->mutex, 0) < 0)
    {
        return -1;
    }
    if(pthread_cond_init(&barrier->cond, 0) < 0)
    {
        pthread_mutex_destroy(&barrier->mutex);
        return -1;
    }
    barrier->tripCount = count;
    barrier->count = 0;

    return 0;
}

int pthread_barrier_destroy(pthread_barrier_t *barrier)
{
    pthread_cond_destroy(&barrier->cond);
    pthread_mutex_destroy(&barrier->mutex);
    return 0;
}

int pthread_barrier_wait(pthread_barrier_t *barrier)
{
    pthread_mutex_lock(&barrier->mutex);
    ++(barrier->count);
    if(barrier->count >= barrier->tripCount)
    {
        barrier->count = 0;
        pthread_cond_broadcast(&barrier->cond);
        pthread_mutex_unlock(&barrier->mutex);
        return 1;
    }
    else
    {
        pthread_cond_wait(&barrier->cond, &(barrier->mutex));
        pthread_mutex_unlock(&barrier->mutex);
        return 0;
    }
}

#endif // PTHREAD_BARRIER_H_
#endif // __APPLE__

#define NUMBER_OF_OPERATIONS 100000

pthread_barrier_t barrier;
pthread_mutex_t consoleMutex;

struct CalculateExpArgs
{
    double value;
    unsigned int numberOfOperations;
    double result;
};

void* CalculateExp(void* args)
{
    struct CalculateExpArgs* expArgs = (struct CalculateExpArgs*)args;

    pthread_mutex_lock(&consoleMutex);
    printf("CalculateExp: arg = %f\n", expArgs->value);
    pthread_mutex_unlock(&consoleMutex);

    double factorial = 1, power;
    expArgs->result = 1;

    for (unsigned int i = 0; i < expArgs->numberOfOperations; i++)
    {
        power = i + 1;
        factorial *= power;
        expArgs->result += pow(expArgs->value, power) / factorial;
    }

    pthread_mutex_lock(&consoleMutex);
    printf("CalculateExp: exp(arg) = %f\n", expArgs->result);
    pthread_mutex_unlock(&consoleMutex);

    pthread_barrier_wait(&barrier);

    return NULL;
}

struct CalculatePiArgs
{
    unsigned int numberOfOperations;
    double result;
};

void* CalculatePi(void* args)
{
    struct CalculatePiArgs* piArgs = (struct CalculatePiArgs*)args;

    pthread_mutex_lock(&consoleMutex);
    printf("CalculatePi: ...\n");
    pthread_mutex_unlock(&consoleMutex);

    piArgs->result = 1;

    int numerator = 2, denominator = 1;
    int numeratorCounter = 0, denominatorCounter = 1;

    for (unsigned int i = 0; i < piArgs->numberOfOperations; i++)
    {
        if (numeratorCounter == 2)
        {
            numeratorCounter = 0;
            numerator += 2;
        }

        if (denominatorCounter == 2)
        {
            denominatorCounter = 0;
            denominator += 2;
        }

        numeratorCounter++;
        denominatorCounter++;

        piArgs->result *= ((double) numerator / (double) denominator);
    }

    piArgs->result *= 2; 

    pthread_mutex_lock(&consoleMutex);
    printf("CalculatePi: pi = %f\n", piArgs->result);
    pthread_mutex_unlock(&consoleMutex);

    pthread_barrier_wait(&barrier);

    return NULL;
}

int main(int argc, const char * argv[])
{
    pthread_t threadExp;
    pthread_t threadPi;

    pthread_barrier_init(&barrier, NULL, 2);
    pthread_mutex_init(&consoleMutex, NULL);

    double x = 1;

    struct CalculateExpArgs expArgs;
    expArgs.value = (-pow(x, 2.)) / 2;
    expArgs.numberOfOperations = NUMBER_OF_OPERATIONS;

    if (pthread_create(&threadExp, NULL, &CalculateExp, &expArgs) == -1)
    {
        pthread_mutex_lock(&consoleMutex);
        printf("CalculateExp() thread launching failed.");
        pthread_mutex_unlock(&consoleMutex);
        return -1;
    }

    struct CalculatePiArgs piArgs;
    piArgs.numberOfOperations = NUMBER_OF_OPERATIONS;

    if (pthread_create(&threadPi, NULL, &CalculatePi, &piArgs) == -1)
    {
        pthread_mutex_lock(&consoleMutex);
        printf("CalculatePi() thread launching failed.");
        pthread_mutex_unlock(&consoleMutex);
        return -1;
    }

    pthread_join(threadExp, NULL);
    pthread_join(threadPi, NULL);

    pthread_barrier_destroy(&barrier);

    printf("f(x) = %f\n", expArgs.result / sqrt(2 * piArgs.result));

    return 0;
}