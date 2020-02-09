#include <stdio.h>
#include <string.h>
#include <unistd.h>

char a_path[4000] = {};		//path given in argument
char n_path[5000] = {}; 	//new path, ready to save
char c_path[5000] = {};		//current argument, red from /etc/environment file
char cwd[3000];				//current working directory

FILE *env_file;

void getCWD(){
	getcwd(cwd, sizeof(cwd));
}

void readFile(){
	char c;

	env_file = fopen("/etc/environment", "r");

	if (env_file == NULL) {
		printf("I can not open a file, please run this script with super user privileges\n");
		return;
	}
	c = fgetc(env_file);
	while (c != EOF) {
		strncat(c_path, &c, 1);
		c = fgetc(env_file);
	}
	fclose(env_file);
}

void changeFile() {
	 char *contain = strstr(c_path, a_path);

	    if (contain != NULL) {

	    	strncpy(n_path, c_path, strlen(c_path) - strlen(contain)-1);
		strcat(n_path, "\" \0");
	        printf("\n '%s' directory has been cut from $PATH\n", a_path);

	    } else {

	        strncpy(n_path, c_path, strlen(c_path)-2);
	        strcat(n_path, ":");
	        strcat(n_path, a_path);
	        strcat(n_path, "\" \0");

	        printf("'%s' directory has been added to $PATH\n", a_path);

	    }
}

void writeFile(){
	env_file = fopen("/etc/environment", "w");

	    if (env_file == NULL) {
	        printf("I can not open a file, please check you writing privileges\n");
	        return;
	    }

	    fprintf(env_file, "%s", n_path);
	    fclose(env_file);
}


int main(int argc, char *argv[]) {

	getCWD();

    if (argc >= 2) {
        char arg[1000];
        strcpy(arg, argv[1]);

        if (argv[1][0] == '/') {
            strcpy(a_path, arg);
        } else {
            strcat(a_path, cwd);
            strcat(a_path, "/");
            strcat(a_path, arg);
        }
    } else {
        strcat(a_path, cwd);
    }
    readFile();
    changeFile();
    writeFile();

    printf("Please relog to see the changes.\n");

    return 1;
}
