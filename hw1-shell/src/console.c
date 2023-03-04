#include "console.h"

static void print_valid_output(char valid);

void erase_console(const int count) {
        for (int i = 0; i < count; i++) {
        fprintf(stdout, "\b \b");
    }
}

void print_line_format(const Env env, const Validity valid) {
    print_valid_output(valid);
    fprintf(stdout, ANSI_BOLD_ON ANSI_COLOR_GREEN_LIGHT "%s@%s" ANSI_COLOR_RESET ":" ANSI_BOLD_ON ANSI_COLOR_BLUE_LIGHT "%s" ANSI_COLOR_RESET "$ " ANSI_COLOR_RESET, env.login_name, env.host_name, env.current_path);
}

static void print_valid_output(const Validity valid) {
    switch(valid) {
        case VALID:
            fprintf(stdout, ANSI_COLOR_BLUE "● " ANSI_COLOR_RESET);
            break;
        case INVALID:
            fprintf(stdout, ANSI_COLOR_RED "● " ANSI_COLOR_RESET);
            break;
        case INIT:
        default:
            fprintf(stdout, "○ ");
            break;
    }
}

void empty_line(void) {
    fprintf(stdout, "\n");
}

void print_logo(void) {
    fprintf(stdout, ANSI_COLOR_BLUE "                   &&&&&&&&               \n" ANSI_COLOR_RESET);
    fprintf(stdout, ANSI_COLOR_BLUE "                &&&&&&&&&&&"ANSI_COLOR_YELLOW"@@@@"ANSI_COLOR_BLUE"           \n" ANSI_COLOR_RESET);
    fprintf(stdout, ANSI_COLOR_BLUE "            &&&&&&&&&&"ANSI_COLOR_YELLOW"@@@@"ANSI_COLOR_BLUE"&&&&"ANSI_COLOR_YELLOW"@@@"ANSI_COLOR_BLUE"&        \n" ANSI_COLOR_RESET);
    fprintf(stdout, ANSI_COLOR_BLUE "          &&&&&&&&"ANSI_COLOR_YELLOW"@@@"ANSI_COLOR_BLUE"&&&&&"ANSI_COLOR_YELLOW"@@@@@@@@@@"ANSI_COLOR_BLUE"&&     \n" ANSI_COLOR_RESET);
    fprintf(stdout, ANSI_COLOR_BLUE "       &&&&&&&&&&"ANSI_COLOR_YELLOW"@@@"ANSI_COLOR_BLUE"&&"ANSI_COLOR_YELLOW"@@@@"ANSI_COLOR_BLUE"&&"ANSI_COLOR_YELLOW"@@@"ANSI_COLOR_BLUE"&&&"ANSI_COLOR_YELLOW"@@@"ANSI_COLOR_BLUE"&&   \n" ANSI_COLOR_RESET);
    fprintf(stdout, ANSI_COLOR_BLUE "      &&&&&&&&&&&"ANSI_COLOR_YELLOW"@@@"ANSI_COLOR_BLUE"&"ANSI_COLOR_YELLOW"@@@"ANSI_COLOR_BLUE"&"ANSI_COLOR_YELLOW"@@@"ANSI_COLOR_BLUE"&&"ANSI_COLOR_YELLOW"@@@"ANSI_COLOR_BLUE"&&&"ANSI_COLOR_YELLOW"@@@"ANSI_COLOR_BLUE"&  \n" ANSI_COLOR_RESET);
    fprintf(stdout, ANSI_COLOR_BLUE "     &&&&&&"ANSI_COLOR_YELLOW"@@"ANSI_COLOR_BLUE"&&&&&"ANSI_COLOR_YELLOW"@@@@@"ANSI_COLOR_BLUE"&&"ANSI_COLOR_YELLOW"@@@"ANSI_COLOR_BLUE"&&&&"ANSI_COLOR_YELLOW"@@@"ANSI_COLOR_BLUE"&&&&&& \n" ANSI_COLOR_RESET);
    fprintf(stdout, ANSI_COLOR_BLUE "    &&&&&&&&&&"ANSI_COLOR_YELLOW"@@@@"ANSI_COLOR_BLUE"&&&&&&"ANSI_COLOR_YELLOW"@@@"ANSI_COLOR_BLUE"&&&"ANSI_COLOR_YELLOW"@@@"ANSI_COLOR_BLUE"&&&&&&&&&\n" ANSI_COLOR_RESET);
    fprintf(stdout, ANSI_COLOR_BLUE "     &&&&&"ANSI_COLOR_YELLOW"@@@"ANSI_COLOR_BLUE"&&&&"ANSI_COLOR_YELLOW"@@@@"ANSI_COLOR_BLUE"&&&&&&&"ANSI_COLOR_YELLOW"@@@"ANSI_COLOR_BLUE"&&&&&&&&&& \n" ANSI_COLOR_RESET);
    fprintf(stdout, ANSI_COLOR_BLUE "      &&&"ANSI_COLOR_YELLOW"@@@"ANSI_COLOR_BLUE"&&"ANSI_COLOR_YELLOW"@@@@"ANSI_COLOR_BLUE"&&&"ANSI_COLOR_YELLOW"@@"ANSI_COLOR_BLUE"&&&"ANSI_COLOR_YELLOW"@@@"ANSI_COLOR_BLUE"&&&&&&&&&&&  \n" ANSI_COLOR_RESET);
    fprintf(stdout, ANSI_COLOR_BLUE "       &&"ANSI_COLOR_YELLOW"@@@"ANSI_COLOR_BLUE"&"ANSI_COLOR_YELLOW"@@@"ANSI_COLOR_BLUE"&"ANSI_COLOR_YELLOW"@@@"ANSI_COLOR_BLUE"&&&&"ANSI_COLOR_YELLOW"@@@"ANSI_COLOR_BLUE"&&&&&&&&&&&&   \n" ANSI_COLOR_RESET);
    fprintf(stdout, ANSI_COLOR_BLUE "          "ANSI_COLOR_YELLOW"@@@@@"ANSI_COLOR_BLUE"&&"ANSI_COLOR_YELLOW"@@@"ANSI_COLOR_BLUE"&&&"ANSI_COLOR_YELLOW"@@@"ANSI_COLOR_BLUE"&&&&&&&&&&      \n" ANSI_COLOR_RESET);
    fprintf(stdout, ANSI_COLOR_BLUE "            &&&&"ANSI_COLOR_YELLOW"@@@"ANSI_COLOR_BLUE"&&&"ANSI_COLOR_YELLOW"@@@"ANSI_COLOR_BLUE"&&&&&&&&&        \n" ANSI_COLOR_RESET);
    fprintf(stdout, ANSI_COLOR_BLUE "                &&&&&"ANSI_COLOR_YELLOW"@@@"ANSI_COLOR_BLUE"&&&&&&            \n" ANSI_COLOR_RESET);
    fprintf(stdout, ANSI_COLOR_BLUE "                   &&&&&&&&               \n" ANSI_COLOR_RESET);
}

void print_welcome(void) {
    fprintf(stdout, ANSI_COLOR_MAGENTA "WELCOME TO SiSH!      \n" ANSI_COLOR_RESET);
    fprintf(stdout, ANSI_COLOR_MAGENTA "SiSH is Simple Shell  \n" ANSI_COLOR_RESET);
    fprintf(stdout, ANSI_COLOR_MAGENTA "Documentation: https://github.com/mobile-os-dku-cis-mse/2022-os-hw1/tree/32181928#readme\n" ANSI_COLOR_RESET);
    fprintf(stdout, ANSI_COLOR_MAGENTA "Version: 1.0.0        \n" ANSI_COLOR_RESET);
    fprintf(stdout, ANSI_COLOR_CYAN "Developed by charlieppark" ANSI_COLOR_RESET);
}
