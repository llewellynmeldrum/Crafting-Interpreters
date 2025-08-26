#include <stdio.h>
#include <stdlib.h>


#define PROG "clox"
#define PRINTF_FATAL_ERR(fmt, ...)do{\
	fprintf(stderr,fmt, ##__VA_ARGS__);\
	fprintf(stderr,"\nexiting...\n");\
	exit(EXIT_FAILURE);\
	}while(0);

void runFile(const char* filename);
void runPrompt();

int main(int argc, char** argv){
	if (argc>2){ 
		fprintf(stderr, "Error! Usage: %s [script] | %s\n",PROG,PROG);
		exit(EXIT_FAILURE);
	} else if (argc == 2){
		runFile(argv[1]);
	} else if (argc == 1){
		runPrompt();
	}
}
void run(const char* input){
	printf("%s\n", input);
}

/* Files.c/h */
#include <sys/stat.h>
size_t SYSCALL_FILESIZE(const char* filename){
	struct stat st;
	stat(filename, &st);
	return (size_t)(st.st_size);
}

char* readFile(const char* filename){
	if (!filename || filename[0]=='\0') PRINTF_FATAL_ERR("Filename passed incorrectly to readFile().");
	size_t filesize = SYSCALL_FILESIZE(filename);

	FILE* inputFile = fopen(filename, "r");

	char* buf = malloc(filesize * sizeof(char));
	if (!buf) PRINTF_FATAL_ERR("Failed to allocate buffer for file %s.",filename);
	size_t bytesRead = fread(buf, sizeof(char), filesize, inputFile);
	if (bytesRead!=filesize) PRINTF_FATAL_ERR("Failed to read %s, read (%zu/%zu)B.",filename,bytesRead,filesize);
	return buf;
}


void runFile(const char* filename){
	printf("ENTERED FILE MODE:\n");
	char* input_bytes = readFile(filename);	
	run(input_bytes);
}

//idk 
#define MAX_LINE_LEN	1024

void runPrompt(){
	printf("ENTERED PROMPT MODE:\n");
	char line[MAX_LINE_LEN];
	while(1){
		printf("> ");
		if (fgets(line, sizeof(line), stdin)==NULL) break;
		run(line);
	}
}
