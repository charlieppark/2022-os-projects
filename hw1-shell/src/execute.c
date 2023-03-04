#include "execute.h"

static int *execute_checker;

int execute(char* command, char** arr, const int pipe_index) {
    execute_checker = mmap(NULL, sizeof *execute_checker,
        PROT_READ | PROT_WRITE, 
        MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    *execute_checker = 0;

    if (pipe_index == -1) {

        pid_t pid;
        pid = fork();

#ifdef DEBUG
        fprintf(stdout, "%d", pid);
#endif
        switch (pid) {
            case -1:
                perror("ERROR: [Execute] Failed to fork");
                return -1;
            case 0:
                execvp(command, arr);
                *execute_checker = -1;
                fprintf(stderr, "ERROR: [Execute] Failed to execute '%s'", command);
                perror(" ");
                exit(1);
                break;
            default:
                wait(0);
                int ret = *execute_checker;
                munmap(execute_checker, sizeof(*execute_checker));
                return ret;
        }
    }  else {
        char* left_command = command;
        char** left_arr = arr;

        char* right_command = arr[pipe_index + 1];
        char** right_arr = arr + pipe_index + 1;

        arr[pipe_index] = NULL;

        int length;

        for (int i = 0; ; i++) {
            if (right_arr[i] == NULL) {
                length = i + 1;
                break;
            }
        }

        int fd[2];

        if (pipe(fd) == -1) {
            perror("ERROR: [Execute-pipe] Failed to create pipe");
            exit(EXIT_FAILURE);
        }

        pid_t pid;
        pid = fork();

        switch (pid) {
            case -1:
                perror("ERROR: [Execute] Failed to fork");
                return -1;
            case 0:
                dup2(fd[1], 1);
                close(fd[0]);
                
                execvp(left_command, left_arr);

                *execute_checker = -1;

                fprintf(stderr, "ERROR: [Execute] Failed to execute '%s'", command);
                perror(" ");
                exit(1);
                break;
            default:
                wait(0);
                break;
        }

        dup2(fd[0], 0);
        close(fd[1]);

        char line[1024];
        
        char* file_name = ".pipe_temp";

        FILE *fp;
        fp = fopen(file_name, "w");
        if (fp == NULL) {
            perror("ERROR: [Execute-fp] Failed to open file");
        }

        while (fgets(line, sizeof(line), stdin) != 0) {
            fprintf(fp, "%s", line);
        }

        fclose(fp);
        
        char** new_arr = malloc_s(sizeof(*new_arr) * (length + 1), __func__);

        for (int i = 0; i < length - 1; i++) {
            new_arr[i] = right_arr[i];
        }

        new_arr[length - 1] = file_name;

        new_arr[length] = NULL;

        pid_t npid;
        npid = fork();

        switch (npid) {
            case -1:
                perror("ERROR: [Execute] Failed to fork");
                return -1;
            case 0:
                execvp(right_command, new_arr);
                *execute_checker = -1;
                fprintf(stderr, "ERROR: [Execute] Failed to execute '%s'", command);
                perror(" ");
                exit(1);
                break;
            default:
                wait(0);
                remove(file_name);
                free(new_arr);
                close(fd[0]);
                int ret = *execute_checker;
                munmap(execute_checker, sizeof(*execute_checker));
                return ret;
        }
    }

    return -1;
}



Validity cd(const char* const target_dir, Env* const env) {
    char* copied = malloc_s(sizeof(*copied) * env->path_max, __func__);

    if (!strcmp(target_dir, "")) {
        strcpy(copied, "~");
    } else {
        strcpy(copied, target_dir);
    }


    if (!strcmp(copied, "~")) {
        strcpy(copied, env->home_dir);
    } 
    
    if(chdir(copied) == -1) {
        perror("ERROR: [cd] Failed to change directory");
        fprintf(stdout, " : path [%s]\n", copied);
        return INVALID;
	}

    env->current_path_abs = get_current_path_abs(env->path_max);
    env->current_path = get_current_path(env->current_path_abs, env->home_dir);

    return VALID;
}

void history_save(const Env env, const char* const command) {
    FILE* fp;

    if (access(env.history, F_OK) != -1) {
        fp = fopen(env.history, "a");
        if (fp == NULL) {
            perror("ERROR: [History-save] Failed to open file");
        }
        
        fprintf(fp, "%s\n", command);
        
        fclose(fp);
    }

    return;
}

char* dollar(const char* const str) {
    if (str[0] == '$') {
        char* ret = getenv(str + 1);
        if (ret == NULL) {
            return "";
        } else {
            return ret;
        }
    }

    return NULL;
}