#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <signal.h>
#include <stdlib.h>

int flag = 1;

void EndPiCalculation(int sig)
{
    flag = 0;
}

void StartPiCalculation(int sig)
{
	// Сигнал SIGUSR2 окончания вычислений.
    signal(SIGUSR2, EndPiCalculation);

    write(2, "StartPiCalculation...\n", strlen("StartPiCalculation...\n")+1);

    double pi = 1;

    int numerator = 2, denominator = 1;
    int numeratorCounter = 0, denominatorCounter = 1;

    while (flag)
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

        pi *= ((double)numerator / (double)denominator);
    }

    pi *= 2;

    // Запись данных в выходной поток, который перехватывается родительским процессом.
	write(1, &pi, sizeof(double));

	// Вывод на консоль информации с использованием потока ошибок, перенаправленного на поток вывода.
	write(2, "Pi child was killed.\n", strlen("Pi child was killed.\n")+1);

    exit (0);
}

int main(int argc, const char * argv[])
{
	// Сигнал SIGUSR1 начала вычислений.
	signal(SIGUSR1, StartPiCalculation);

    while (1);

    return 0;
}
