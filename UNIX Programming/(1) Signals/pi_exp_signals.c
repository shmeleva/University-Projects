#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <signal.h>
#include <stdlib.h>

#define SUCCESS 0
#define ERROR -1

void Dummy () { }

int main(int argc, const char * argv[])
{
    int x = 1;

    int PiFileDescriptor[2];
    int ExpFileDescriptor[2];

    // The pipe() function creates a pipe (an object that allows unidirectional data flow)
    // and allocates a pair of file descriptors.
    pipe(PiFileDescriptor);
    pipe(ExpFileDescriptor);

    switch (fork())
    {
        case SUCCESS:
        {
            printf ("Pi child process...\n");

            // Перенаправление потока ввода.
            dup2 (PiFileDescriptor[0], 0);
            // Перенаправление потока ошибок в поток вывода.
            dup2 (1, 2);
            // Перенаправление потока вывода.
            dup2 (PiFileDescriptor[1], 1);

	    close (PiFileDescriptor[0]);
            close (PiFileDescriptor[1]);

            if (execl("/Users/emmer/Dropbox/Educational materials/Программирование в среде UNIX/Семестр (2)/Лабораторные работы/ЛР1/CalculatePi", 0) == -1)
            {
                printf ("Exp execl() error.\n");
                return -1;
            }

            exit (0);
        }
        case ERROR:
        {
            printf ("Pi fork() error.\n");
            return -1;
        }
    }

    switch (fork())
    {
        case SUCCESS:
        {
            printf ("Exp child process...\n");

            // Перенаправление потока ввода.
            dup2 (ExpFileDescriptor[0], 0);
            // Перевод потока ошибок в поток вывода.
            dup2 (1, 2);
            // Перенаправление потока вывода.
            dup2 (ExpFileDescriptor[1], 1);

	    close (ExpFileDescriptor[0]);
            close (ExpFileDescriptor[1]);

            write(ExpFileDescriptor[1], &x, sizeof(int));

            if (execl("/Users/emmer/Dropbox/Educational materials/Программирование в среде UNIX/Семестр (2)/Лабораторные работы/ЛР1/CalculateExp", 0) == -1)
            {
                printf ("Exp execl() error.\n");
                return -1;
            }

            exit (0);
        }
        case ERROR:
        {
            printf ("Exp fork() error.\n");
            return -1;
        }
    }

    sleep (3);

    // The process group of the current process is returned by getpgrp().
    int grp = -1 * getpgrp();

    printf ("Calculating...\n");

    // The sig argument specifies which signal was received.
    // The func procedure allows a user to choose the action upon receipt of a signal.
    // 30    SIGUSR1      terminate process    User defined signal 1
    // 31    SIGUSR2      terminate process    User defined signal 2
    signal(SIGUSR1, Dummy);
    signal(SIGUSR2, Dummy);

    // The kill() function sends the signal specified by sig to pid, a process or a group of processes.
    kill(grp, SIGUSR1);

    printf ("Sleep...\n");
    sleep(3);

    kill(grp, SIGUSR2);

    double exp = 0, pi = 0;

    read(PiFileDescriptor[0], &pi, sizeof(double));
    read(ExpFileDescriptor[0], &exp, sizeof(double));

    printf ("exp = %f\n", exp);
    printf ("pi = %f\n", pi);

    double result = exp / sqrt(2.0 * pi);
    printf ("f(x) = %lf\n", result);

    return 0;
}
