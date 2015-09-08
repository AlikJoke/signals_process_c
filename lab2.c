#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <stdlib.h>

int num;

void handlerAlarm( int sig_name )
{
	signal( sig_name, &handlerAlarm );
	printf( "Сигнал SIGALRM %i отправлен для PID = %i с PPID = %i\n", sig_name, getpid(), getppid() );
	kill( getpid(), SIGINT );
}

void handlerInt( int sig_name )
{
	sleep(10);
	signal( sig_name, &handlerInt );
	printf( "Получен отклик SIGINT %i для процесса с PID = %i\n", sig_name,  getpid() );
	num++;
}

int main(int argc, char* argv[])
{
	if ( argc != 2 )
	{
		printf( "Invalid number of params\n" );
		exit(123);
	}
	int i;
	int maxReturnCode = 0;
        int currentReturnCode = 0;

	num = 1;
	int numb = atoi(argv[1]);
	pid_t pid[numb];

	pid_t main_pid = getpid();
        printf( "Main PID = %i\n", main_pid );

	signal( SIGINT, &handlerInt );
	signal( SIGALRM, &handlerAlarm );

	for ( i = 0; i < numb; i++ )
	{
			pid[i] = fork();
	}

	if ( main_pid == getpid() )
	{
		for ( i = 0; i < numb; i++ )
		{
			kill( pid[i], SIGALRM );
		}
		sleep(11);
		if ( numb != num )
		{
			_exit(0);
		}
		else
		{
			for ( i = 0; i < numb; i++ )
			{
				kill( pid[i], SIGKILL );
				printf( "Сигнал SIGKILL отправлен процессу с PID = %i и PPID = %i\n", getpid(), getppid() );
			}

		        for ( i = 0; i < numb; i++ )
            		{
                		waitpid( pid[i], &currentReturnCode, NULL );
                		if ( currentReturnCode > maxReturnCode )
                    			maxReturnCode = currentReturnCode;
            		}

			printf( "Максимальный код возврата процесса-потомка: %i\n", maxReturnCode );
            		_exit( maxReturnCode );
 		}
      	 }	else
		{
			 while( 1 )
   			 {
        			sleep(1);
    			 }
		}

	_getchar();

	_exit(0);
}
