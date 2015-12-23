#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/resource.h>

#define SERVER_FIFO_FILE "ServerFifo"
#define TIMEOUT 3

int PriorityRandom()
{
    return rand() % 11;;
}

char CharRandom()
{
    return 33 + (rand() % 94);
}

int BoolRandom()
{
    return rand() % 100 > 80 ? 1 : 0;
}

void CreateFifoFile(char* fileName)
{
    unlink(fileName);
    umask(0);
    mkfifo(fileName, 0777);
}

void RunClient()
{
    printf ("Process %d started.\n", getpid());

    umask(0);
    srand(time(0));

    int fileDescriptor = open(SERVER_FIFO_FILE, O_RDWR);

    if(fileDescriptor == -1)
    {
        perror("open() error");
        exit(-1);
    }

    char fifoFileName [1024];
    sprintf (fifoFileName, "%d", getpid());

    CreateFifoFile (fifoFileName);

    printf ("Process %d created named pipe '%s'.\n", getpid(), fifoFileName);

    write(fileDescriptor, fifoFileName, strlen(fifoFileName) + 1);
    close(fileDescriptor);

    fileDescriptor = open(fifoFileName, O_NONBLOCK | O_RDWR);

    time_t startTime = time(NULL);
    int receivedCounter = 0;

    while(time(NULL) - startTime < TIMEOUT)
    {
        char receivedChar;

        if(read(fileDescriptor, &receivedChar, sizeof(char)))
            receivedCounter++;
    }

    printf ("Process %d received %d symbols.\n", getpid(), receivedCounter);

    close(fileDescriptor);
    unlink(fifoFileName);

    exit(0);
}

int main()
{
	srand(time(0));

	int fileDescriptor;
    int clientFileNameMaxLength = 80;
	char clientFileName[clientFileNameMaxLength];

	CreateFifoFile(SERVER_FIFO_FILE);

    switch (fork())
    {
        case 0:
        {
            RunClient();
        }
        case -1:
        {
            printf ("fork() error");
            return -1;
        }
    }

    switch (fork())
    {
        case 0:
        {
            RunClient();
        }
        case -1:
        {
            printf ("fork() error");
            return -1;
        }
    }

	fileDescriptor = open(SERVER_FIFO_FILE, O_RDONLY);

    if (fileDescriptor == -1)
    {
        printf ("open() error");
        return -1;
    }

	while (1)
	{
		if (read(fileDescriptor, clientFileName, clientFileNameMaxLength))
        {
            switch (fork())
            {
                case 0:
                {
                    close(fileDescriptor);

                    int clientFifo = open(clientFileName, O_RDWR);
                    char randomChar = CharRandom();

                    while ((write(clientFifo, &randomChar, sizeof(char)) > 0))
                    {
                        //usleep(rand() % 150);
                        
                        randomChar = CharRandom();

                        if (BoolRandom())
                        {
                            setpriority(PRIO_PROCESS, 0, PriorityRandom());
                        }
                    }

                    close(clientFifo);
                    exit (0);
                }
                case -1:
                {
                    printf ("fork() error");
                    return -1;
                }
            }
		}

	}

	close(fileDescriptor);
	unlink(SERVER_FIFO_FILE);
}
