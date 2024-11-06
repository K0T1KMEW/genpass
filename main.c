#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#pragma warning(disable : 4996)

#define MAX_PASSWORD_LENGTH 1000
#define MAX_ALPHABET_LENGTH 128


char random_char(const char* alphabet, int alphabet_len) {
    int index = rand() % alphabet_len;
    return alphabet[index];
}


void print_error_and_exit(const char* error_message) {
    fprintf(stderr, "Error: %s\n", error_message);
    exit(1);
}

void generate_password(int min_len, int max_len, const char* alphabet, int alphabet_len, int num_passwords) {
    srand(time(NULL));
    for (int i = 0; i < num_passwords; ++i) {
        int password_len = min_len + rand() % (max_len - min_len + 1);
        char password[MAX_PASSWORD_LENGTH];
        for (int j = 0; j < password_len; ++j) {
            password[j] = random_char(alphabet, alphabet_len);
        }
        password[password_len] = '\0';
        printf("%s\n", password);
    }
}
bool has_unique_chars(const char* str) {
    bool chars[256] = { 0 };
    while (*str) {
        if (chars[(unsigned char)*str]) {
            return false;
        }
        chars[(unsigned char)*str] = true;
        str++;
    }
    return true;
}
int main(int argc, char* argv[]) {
    int min_len = -1, max_len = -1, password_len = -1, num_passwords = 1;
    char alphabet[MAX_ALPHABET_LENGTH] = "";
    bool custom_alphabet = false;
    bool custom_char_set = false;
    char char_set[5] = "";
    bool m1_used = false, m2_used = false, n_used = false, c_used = false, a_used = false, C_used = false;
    bool read_argument_next = true;
    for (int i = 1; i < argc; ++i) {
        bool recognized = false;
        if (strcmp(argv[i], "-m1") == 0) {
            recognized = true;
            if (m1_used) {print_error_and_exit("Option -m1 is already used");}
            if (++i < argc && sscanf(argv[i], "%d", &min_len) != 1) {print_error_and_exit("Invalid argument for -m1");}
            m1_used = true;
            read_argument_next = false;
        }
        else if (strcmp(argv[i], "-m2") == 0) {
            recognized = true;
            if (m2_used) {print_error_and_exit("Option -m2 is already used");}
            if (++i < argc && sscanf(argv[i], "%d", &max_len) != 1) {print_error_and_exit("Invalid argument for -m2");}
            m2_used = true;
            read_argument_next = false;
        }
        else if (strcmp(argv[i], "-n") == 0) {
            recognized = true;
            if (n_used) {print_error_and_exit("Option -n is already used");}
            if (++i < argc && sscanf(argv[i], "%d", &password_len) != 1) {print_error_and_exit("Invalid argument for -n");}
            n_used = true;
            read_argument_next = false;
        }
        else if (strcmp(argv[i], "-c") == 0) {
            recognized = true;
            if (c_used) {print_error_and_exit("Option -c is already used");
            }
            if (++i < argc && sscanf(argv[i], "%d", &num_passwords) != 1) {print_error_and_exit("Invalid argument for -c");}
            c_used = true;
            read_argument_next = false;
        }
        else if (strcmp(argv[i], "-a") == 0) {
            recognized = true;
            if (a_used) { print_error_and_exit("Option -a is already used");} 
            if (custom_char_set) { print_error_and_exit("Options -a and -C are not compatible");}
            a_used = true;
            custom_alphabet = true;
            strcpy(alphabet, argv[i + 1]);
            if (strlen(alphabet) == 0) { print_error_and_exit("No alphabet specified after -a");}
            ++i;
        }
        else if (strcmp(argv[i], "-C") == 0) {
            recognized = true;
            if (C_used) {print_error_and_exit("Option -C is already used");}
            if (custom_alphabet) {print_error_and_exit("Options -a and -C are not compatible");}
            if (++i < argc && strlen(argv[i]) < 5) {
                if (has_unique_chars(argv[i]) == false) {
                    print_error_and_exit("Duplicate characters in -C option");
                } strcpy(char_set, argv[i]);
            }
            else {print_error_and_exit("Invalid argument for -C");}
            custom_char_set = true;
            C_used = true;
        }
        if (!recognized) { fprintf(stderr, "Warning: Unrecognized option '%s' ignored\n", argv[i]);}
    }
    if (min_len != -1 && max_len == -1) {print_error_and_exit("Option -m1 requires option -m2");}
    if (password_len != -1 && (min_len != -1 || max_len != -1)) {print_error_and_exit("Options -m1, -m2, and -n are not compatible together");}
    if (min_len > max_len) {print_error_and_exit("min_len bigger than max_len");}
    if (password_len == 0) {print_error_and_exit("lenght of password is zero");}
    for (int i = 0; i < strlen(char_set); i++) {
        if (char_set[i] == 'a' || char_set[i] == 'A' || char_set[i] == 'D' || char_set[i] == 'S') {}
        else {print_error_and_exit("wrond -C alphabet");}
    }
    if (custom_char_set) {
        alphabet[0] = '\0';
        if (strchr(char_set, 'a')) strcat(alphabet, "abcdefghijklmnopqrstuvwxyz");
        if (strchr(char_set, 'A')) strcat(alphabet, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
        if (strchr(char_set, 'D')) strcat(alphabet, "0123456789");
        if (strchr(char_set, 'S')) strcat(alphabet, "!@#$%^&*()-_+=[]{};:,.<>??");
    }
    int alphabet_len = strlen(alphabet);
    if (min_len != -1 && max_len != -1) {generate_password(min_len, max_len, alphabet, alphabet_len, num_passwords);}
    else if (password_len != -1) {generate_password(password_len, password_len, alphabet, alphabet_len, num_passwords);}
    else {print_error_and_exit("Password length not specified");}
    return 0;
}