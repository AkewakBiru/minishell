#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

int main()
{
	char *line = readline("> ");
	printf("line: %s\n", line);
}