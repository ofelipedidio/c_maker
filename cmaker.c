#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <inttypes.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "ERROR: Invalid number of arguments!\n");
        fprintf(stderr, "Correct usage: %s <file.c>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Could not open the input file");
        return EXIT_FAILURE;
    }

    uint64_t current_time = time(0);

    char buffer[1024];
    sprintf(buffer, "temp%lu.c", current_time);

    FILE *file_out = fopen(buffer, "w");
    if (file == NULL) {
        perror("Could not open the output file");
        return EXIT_FAILURE;
    }

    fprintf(file_out, "#include <stdio.h>\n");
    fprintf(file_out, "#include <stdlib.h>\n");
    fprintf(file_out, "\n");
    fprintf(file_out, "int main(int argc, char **argv) {\n");
    fprintf(file_out, "    FILE *file = fopen(\"temp%lu.c\", \"w\");\n", current_time+1);
    fprintf(file_out, "    if (file == NULL) {\n");
    fprintf(file_out, "        fprintf(stderr, \"Could not open temp.c\\n\");\n");
    fprintf(file_out, "        return EXIT_FAILURE;\n");
    fprintf(file_out, "    }\n");

    fprintf(file_out, "    fprintf(file, \"");
    while (1) {
        int _c = fgetc(file);
        if (_c <= 0) {
            break;
        }
        char c = _c;
        if (c == '\n') {
            fprintf(file_out, "\\n\");\n    fprintf(file, \"");
        } else if (c == '\\') {
            fprintf(file_out, "\\\\");
        } else if (c == '"') {
            fprintf(file_out, "\\\"");
        } else if (c == '%') {
            fprintf(file_out, "%%%%");
        } else {
            fprintf(file_out, "%c", c);
        }
    }
    fprintf(file_out, "\");\n");

    fprintf(file_out, "    fclose(file);\n");
    fprintf(file_out, "    system(\"gcc -o temp%lu temp%lu.c\");\n", current_time+1, current_time+1);
    fprintf(file_out, "    char buffer[0x10000];\n");
    fprintf(file_out, "    char *ptr = buffer;\n");
    fprintf(file_out, "    ptr += sprintf(ptr, \"./temp%lu\");\n", current_time+1);
    fprintf(file_out, "    for (int i = 1; i < argc; i++) {;\n");
    fprintf(file_out, "        ptr += sprintf(ptr, \" %%s\", argv[i]);\n");
    fprintf(file_out, "    }\n");
    fprintf(file_out, "    system(buffer);\n");
    fprintf(file_out, "    return EXIT_SUCCESS;\n");
    fprintf(file_out, "}\n");

    fclose(file);
    fclose(file_out);

    sprintf(buffer, "gcc -o do_as_you_please temp%lu.c", current_time);
    system(buffer);

    return EXIT_SUCCESS;
}
