#include "SiSH.h"

static char* get_login_name(const int login_name_max);
static char* get_host_name(const int host_name_max);
static void set_Env(Env* env);
static char* get_home_dir(const char* const login_name);
static void clear_buffer(char* buffer, const int len);
static int sish_loop(Env* env);
static int sish_file_mode(Env* env, char* file_name);

Env env;

void handler(int sig) {
    switch (sig) {
        case SIGINT:
            fprintf(stdout, "^C\n");
            break;
        case SIGTSTP:
            fprintf(stdout, "^Z\n");
            break;    
        case SIGQUIT:
            fprintf(stdout, "^\\\n");
            break;
        default:
            break;
    }
}


int main(int argc, char* argv[]) {

    set_Env(&env);

    int sys_fault_count =  -1;

    char* script_file = NULL;

    char nologo = 0;
    char nowelcome = 0;

    for (int i = 1; i < argc; i++) {

        if (strcmp(argv[i], "--nologo") == 0) {
            nologo = 1;
            continue;
        }

        if (strcmp(argv[i], "--nowelcome") == 0) {
            nowelcome = 1;
            continue;
        }     

        if (strcmp(argv[i], "--login_name_max") == 0) {
			
			if (argc <= i + 1) {
				fprintf(stderr, "ARGUMENTS ERROR");
				exit(EXIT_FAILURE);
			}

			i++;
			(&env)->login_name_max = atoi(argv[i]);
			continue;
		}

        if (strcmp(argv[i], "--host_name_max") == 0) {
			
			if (argc <= i + 1) {
				fprintf(stderr, "ARGUMENTS ERROR");
				exit(EXIT_FAILURE);
			}

			i++;
			(&env)->host_name_max = atoi(argv[i]);
			continue;
		}

        if (strcmp(argv[i], "--path_max") == 0) {
			
			if (argc <= i + 1) {
				fprintf(stderr, "ARGUMENTS ERROR");
				exit(EXIT_FAILURE);
			}

			i++;
			(&env)->path_max = atoi(argv[i]);
			continue;
		}

        if (strcmp(argv[i], "--arg_max") == 0) {
			
			if (argc <= i + 1) {
				fprintf(stderr, "ARGUMENTS ERROR");
				exit(EXIT_FAILURE);
			}

			i++;
			(&env)->arg_max = atoi(argv[i]);
			continue;
		}

        if (strcmp(argv[i], "--sys_fault_count") == 0) {
			
			if (argc <= i + 1) {
				fprintf(stderr, "ARGUMENTS ERROR");
				exit(EXIT_FAILURE);
			}

			i++;
			sys_fault_count = atoi(argv[i]);
			continue;
		}

		script_file = argv[i];
    }

    if (script_file == NULL) {
        if (!nologo) {
            empty_line();
            empty_line();
            print_logo();
            empty_line();
            empty_line();
        }

        if (!nowelcome) {
            empty_line();
            print_welcome();
            empty_line();
            empty_line();
        }

        int ret = sish_loop(&env);
        while (ret == EXIT_FAILURE) {
            ret = sish_loop(&env);
            sys_fault_count--;
            if (sys_fault_count < 0) {
                break;
            }
        }
        return ret;
    } else {
        return sish_file_mode(&env, script_file);
    }
    

}

static int sish_loop(Env* env) {
    signal(SIGINT, handler);
    signal(SIGTSTP, handler);
    signal(SIGQUIT, handler);

    FILE* fp = fopen(env->history, "r");

    if (fp == NULL) {
        print_error(__func__, "Failed to open file : History not exists");
        FILE* fp = fopen(env->history, "w");
        if (fp == NULL) {
            print_error(__func__, "SERIOUS--Failed to open file twice");
            exit(1);
        }
        fclose(fp);
        
        fprintf(stdout, "CREATED HISTORY FILE. RUN SHELL AGAIN\n");

        exit(0);
    }

    List* history_block = l_init();
    
    char line[1024];

    while (fgets(line, sizeof(line), fp)) {
        char* str = malloc_s(sizeof(*str) * (strlen(line) + 1), __func__);

        strcpy(str, line);

        str[strlen(str) - 1] = '\0';

        l_insert(history_block, str);

        if (history_block->size > 500) {
            l_remove(history_block, 0);
        }
    }

#ifdef DEBUG
    for (int i = 0; i < history_block->size; i++) {
        printf("%s\n", l_get(history_block, i));
    }
#endif

    fclose(fp);

    Validity valid = INIT;

    while(1) {        
        int stdin_copy = dup(0);
        int stdout_copy = dup(1);
        
        env->current_path_abs = get_current_path_abs(env->path_max);
        env->current_path = get_current_path(env->current_path_abs, env->home_dir);
        env->login_name = get_login_name(env->login_name_max);
        env->home_dir = get_home_dir(env->login_name);

        char buffer[env->arg_max];
        
        clear_buffer(buffer, sizeof(buffer));
        
        print_line_format(*env, valid);
        
        int idx = 0;

        char before;
        char c = -1;
        char blank_checker = -1;

        int pipe_index = -1;
        
        int history_cursor = -1;

        int history_len = 0;

        char* history = "";

        while (1) {
            before = c;
			c = getch();

            if (before == '\33' && c == '[') {
                char arrow = getch();
                int buf_len;

                if ((arrow != 'A') && (arrow != 'B')) continue;

                if (strcmp(buffer, history)) {
                    buf_len = strlen(buffer);

                    erase_console(buf_len);
                    clear_buffer(buffer, sizeof(buffer));
                }

                switch (arrow) {
                    case 'A':
                        if ((history_cursor < 500) && (history_cursor < history_block->size - 1)) history_cursor++;
                        else continue;
                        break;
                    case 'B':
                        if (history_cursor > 0) history_cursor--;
                        else {
                            history_len = strlen(history);
                            history = "";
                            clear_buffer(buffer, sizeof(buffer));
                            erase_console(history_len);
                            idx = 0;
                            continue;
                        }
                        break;
                    default:
                        break;
                }

                history_len = strlen(history);
                history = l_get(history_block, history_block->size - 1 - history_cursor);
                erase_console(history_len);
                fprintf(stdout, "%s", history);
                strcpy(buffer, history);
                idx = strlen(buffer);
                continue;
            }

            if ((before == 9) && (c == 9)) { // tab
                buffer[idx] = '\0';
                int arr_size;
                char** arr = parser("ls | grep blank", &arr_size, *env);
                pipe_index = 1;

                char* incomplete = malloc_s(sizeof(*incomplete) * 100, __func__);

                strcpy(incomplete, buffer + blank_checker + 1);
                arr[3] = incomplete;

                empty_line();
                
                char* command = arr[0];

                execute(command, arr, pipe_index);

                dup2(stdin_copy, 0);
                dup2(stdout_copy, 1);
                close(stdin_copy);
                close(stdout_copy);

                print_line_format(*env, INIT);
                fprintf(stdout, "%s", buffer);
                pipe_index = -1;
                continue;
            }

            if (c == 127) {
                if (idx > 0) {
                    idx--;
                    erase_console(1);
                }
                continue;
            }

            if ((c >= 32) && (c <= 126)) {
                printf("%c", c);

                if (c == 32) blank_checker = idx;

                buffer[idx++] = c;
            }

            if (c == 10) {
                empty_line();
                buffer[idx] = '\n';
                break;
            }
		}

        if (buffer[0] == '\n') {
            valid = -1;
            continue;
        }

        buffer[idx] = '\0';

#ifdef DEBUG
        fprintf(stdout, "%s", buffer);
#endif

        int arr_size;
        char** arr = parser(buffer, &arr_size, *env);

        for (int i = 0; i < arr_size - 1; i++) {
            if (!strcmp(arr[i], "|") && arr_size - 1 > i + 1) {
                pipe_index = i;
            }

            char* env = dollar(arr[i]);
            if (env != NULL) {
                arr[i] = env;
            }
        }

        char* command = arr[0];

        if (!strcmp(command, "quit") || !strcmp(command, "exit")) {
            //fprintf(stdout, "\n");
            exit(EXIT_SUCCESS);
        }

        if (!strcmp(command, "cd")) {
            if (arr_size > 3) {
                printf("%d", arr_size);
                print_error(__func__, "Wrong directory : Too many arguments");
                continue;
            }

            char* path;

            if (arr_size == 2) {
                path = "";
            } else {
                path = arr[1];
            }

            valid = cd(path, env);
            continue;
        }

        if (execute(command, arr, pipe_index) != -1) {
            valid = VALID;

            history_save(*env, buffer);

            char* buffer_save = malloc_s(sizeof(*buffer_save) * (strlen(buffer) + 1), __func__);

            strcpy(buffer_save, buffer);

            l_insert(history_block, buffer_save);

            if (history_block->size > 500) {
                l_remove(history_block, 0);
            }
        } else {
            valid = INVALID;
        }

        dup2(stdin_copy, 0);
        dup2(stdout_copy, 1);
        close(stdin_copy);
        close(stdout_copy);
        
        clear_buffer(buffer, sizeof(buffer));
    }
}

static int sish_file_mode(Env* env, char* file_name) {

    FILE* fp = fopen(file_name, "r");

    if (fp == NULL) {
        print_error(__func__, "Failed to open file");
        exit(EXIT_FAILURE);
    }

    char line[env->arg_max];

    while (fgets(line, sizeof(line), fp)) {
        line[strlen(line) - 1] = '\0';
        env->current_path_abs = get_current_path_abs(env->path_max);
        env->current_path = get_current_path(env->current_path_abs, env->home_dir);
        env->login_name = get_login_name(env->login_name_max);
        env->home_dir = get_home_dir(env->login_name);
                
        int pipe_index = -1;

        if (line[0] == '\n') {
            continue;
        }

        int arr_size;
        char** arr = parser(line, &arr_size, *env);

        for (int i = 0; i < arr_size - 1; i++) {
            if (!strcmp(arr[i], "|") && arr_size - 1 > i + 1) {
                pipe_index = i;
            }

            char* env = dollar(arr[i]);
            if (env != NULL) {
                arr[i] = env;
            }
        }

        char* command = arr[0];

        if (!strcmp(command, "cd")) {
            if (arr_size > 3) {
                printf("%d", arr_size);
                print_error(__func__, "Wrong directory : Too many arguments");
                continue;
            }

            char* path;

            if (arr_size == 2) {
                path = "";
            } else {
                path = arr[1];
            }

            cd(path, env);
            continue;
        }

        if (execute(command, arr, pipe_index) == -1) {
            print_error(__func__, "Failed to execute script");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}

static void clear_buffer(char* buffer, const int len) {
    if (buffer) {
        memset(buffer, '\0', len);
    } else {
        print_error(__func__, "Buffer is NULL");
        exit(1);
    }
}

static char* get_login_name(const int login_name_max) {
    char* login_name = getenv("USER");

    if (login_name == NULL) {
        char* login_name_buf = malloc_s(sizeof(*login_name_buf) * login_name_max, __func__);
        getlogin_r(login_name_buf, login_name_max);
        login_name = malloc_s(sizeof(*login_name) * (strlen(login_name_buf) + 1), __func__);
        strcpy(login_name, login_name_buf);
        free(login_name_buf);
    }

    return login_name;
}

static char* get_host_name(const int host_name_max) {
    char* host_name_buf = malloc_s(sizeof(*host_name_buf) * host_name_max, __func__);
    gethostname(host_name_buf, host_name_max);
    char* host_name = malloc_s(sizeof(*host_name) * (strlen(host_name_buf) + 1), __func__);
    strcpy(host_name, host_name_buf);
    free(host_name_buf);
    return host_name;
}

char* get_current_path_abs(const int path_max) {
    char* current_path_buf = malloc_s(sizeof(*current_path_buf) * path_max, __func__);
    getcwd(current_path_buf, path_max);
    char* current_path_abs = malloc_s(sizeof(*current_path_abs) * (strlen(current_path_buf) + 1), __func__);
    strcpy(current_path_abs, current_path_buf);
    free(current_path_buf);
    return current_path_abs;
}

char* get_current_path(char* const current_path_abs, const char* const home_dir) {
    char* current_path;

    if (strncmp(current_path_abs, home_dir, strlen(home_dir)) == 0) {
        current_path = malloc_s(sizeof(*current_path) * (strlen(current_path_abs) - strlen(home_dir) + 2), __func__);
        sprintf(current_path, "~%s", current_path_abs + strlen(home_dir));
    } else {
        current_path = current_path_abs;
    }

    return current_path;    
}

static char* get_home_dir(const char* const login_name) {
    char* home_dir = getenv("HOME");
    
    if (home_dir == NULL) {

        char* home = malloc_s(sizeof(*home) * 6 + strlen(login_name), __func__);
        strcpy(home, "/home/");
        strcat(home, login_name);

        int length = strlen(home);

        home_dir = malloc_s(sizeof(*home_dir) * (length + 1), __func__);

        strcpy(home_dir, home);
        
        free(home);
    }

    return home_dir;
}

static void set_Env(Env* env) {
    int login_name_max, host_name_max, path_max, arg_max;

#ifndef LOGIN_NAME_MAX
    login_name_max = sysconf(_SC_LOGIN_NAME_MAX);
#else
    login_name_max = LOGIN_NAME_MAX;
#endif

#ifndef HOST_NAME_MAX
    host_name_max = sysconf(_SC_HOST_NAME_MAX);
#else
    host_name_max = HOST_NAME_MAX;
#endif

#ifndef PATH_MAX
    path_max = pathconf("/", _PC_PATH_MAX);
#else
    path_max = PATH_MAX;
#endif

#ifdef ARG_MAX
    arg_max = ARG_MAX;
#else
    arg_max = sysconf(_SC_ARG_MAX);
#endif

    char* login_name = get_login_name(login_name_max);
    char* host_name = get_host_name(host_name_max);
    char* current_path_abs = get_current_path_abs(path_max);
    char* home_dir = get_home_dir(login_name);
    char* current_path = get_current_path(current_path_abs, home_dir);
    char* history = malloc_s(sizeof(*history) * (strlen(home_dir) + strlen("/.SiSH_history") + 1), __func__);
    strcpy(history, home_dir);
    strcat(history, "/.SiSH_history");

    env->login_name_max = login_name_max;
    env->host_name_max = host_name_max;
    env->path_max = path_max;
    env->login_name = login_name;
    env->host_name = host_name;
    env->current_path_abs = current_path_abs;
    env->current_path = current_path;
    env->home_dir = home_dir;
    env->arg_max = arg_max;
    env->history = history;

    return;
}
