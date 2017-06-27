#include <unistd.h>  
#include <stdlib.h>  
#include <fcntl.h>  
#include <limits.h>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <stdio.h>  
#include <string.h>  
int pipe_fd = -1;  
int data_fd = -1;  
int res = 0;  
const int open_mode = O_WRONLY;  
int bytes_sent = 0;  
char buffer[PIPE_BUF + 1];  
 
int fifo_init(const char * fifo_name)  
{

    if(access(fifo_name, F_OK) == -1)  
    {  

        res = mkfifo(fifo_name, 0777);  
        if(res != 0)  
        {  
            fprintf(stderr, "Could not create fifo %s\n", fifo_name);  
            return -1; 
        }  
    }  
  
    printf("Process %d opening FIFO O_WRONLY\n", getpid());  
    //以只写阻塞方式打开FIFO文件，以只读方式打开数据文件  
    pipe_fd = open(fifo_name, open_mode);  
    printf("Process %d result %d\n", getpid(), pipe_fd);  
    return 0
}
int fifo_send(char *msg)
{
    if(pipe_fd != -1)  
    {  
	char input[100];
	while (scanf("%s", input)) {
		if ( !strcmp(input, "quit") ) {
			
			break;
		}else
		{
		    res = write(pipe_fd, input, strlen(input));  
		    if(res == -1)  
		    {  
		        fprintf(stderr, "Write error on pipe\n");  
		        exit(EXIT_FAILURE);  
		    }  

		}
		sleep(1);
	}
}
}
int main()  
{  
    int pipe_fd = -1;  
    int res = 0;  
    pipe_fd = fifo_init("/tmp/qqmsg_fifo");  
  
    if(pipe_fd != -1)  
    {  
	char input[100];
	while (scanf("%s", input)) {
		if ( !strcmp(input, "quit") ) {
			
			break;
		}else
		{
		    res = write(pipe_fd, input, strlen(input));  
		    if(res == -1)  
		    {  
		        fprintf(stderr, "Write error on pipe\n");  
		        exit(EXIT_FAILURE);  
		    }  

		}
		sleep(1);
	}
	
        close(pipe_fd);  
	return 0;

     }  
    else  
        exit(EXIT_FAILURE);  
  
    printf("Process %d finished\n", getpid());  
    exit(EXIT_SUCCESS);  
} 
