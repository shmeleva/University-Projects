#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

int flag = 1;
int x = 0;

void EndExpCalculation(int sig)
{
    flag = 0;
}

void StartExpCalculation(int sig)
{
    // Сигнал SIGUSR2 окончания вычислений.
    signal(SIGUSR2, EndExpCalculation);

    write(2, "StartExpCalculation...\n", strlen("StartExpCalculation...\n")+1);

    double value = -(pow(x, 2.0))/2;

    double exp = 1;
    double factorial = 1;
    double power;

    unsigned int i = 0;
    while (flag)
    {
        power = i + 1;
        factorial *= power;
        exp += pow(value, power) / factorial;
        i++;
    }

    // Запись данных в выходной поток, который перехватывается родительским процессом.
    write(1, &exp, sizeof(double));

    // Вывод на консоль информации с использованием потока ошибок, перенаправленного на поток вывода.
    write(2, "Exp child was killed.\n", strlen("Exp child was killed.\n")+1);

    exit (0);
}

int main(int argc, const char * argv[])
{
    signal(SIGUSR1, StartExpCalculation);

    // Чтение параметра.
    read(0, &x, sizeof(int));

    while (1);

    return 0;
}
