#include <argp.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

const char *argp_program_version = "padinf";

static char doc[] = "padinf -- prints padding information\n"
                    "Example: padinf -a 16384 a.bin";

static char args_doc[] = "[input=FILE]";

static struct argp_option options[] = {
    { "align",  'a', "NUM", 0, "Alignment [default=16384]" },
    { 0 }
};

struct arguments {
    /* options */
    uint32_t alignment;
    /* arguments */
    char *input;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = state->input;
    uint32_t i;
    switch (key) {
    case 'a':
        i = atoi(arg);
        if (i <= 0)
            argp_usage(state);
        arguments->alignment = i;
        break;
    case ARGP_KEY_ARG:
        if (state->arg_num >= 1)
            argp_usage(state);
        if (state->arg_num == 0)
            arguments->input = arg;
        break;
    case ARGP_KEY_END:
        break;
    default:
        return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static struct argp argp = { options, parse_opt, args_doc, doc };

int main(int argc, char **argv) {
    struct arguments arguments;
    arguments.alignment = 16384;
    arguments.input = NULL; /* stdin */
    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    FILE* file = arguments.input ? fopen(arguments.input, "rb") : stdin;
    if (!file) {
        fprintf(stderr, "Error: failed to open file\n");
        return EXIT_FAILURE;
    }

    unsigned char bank[arguments.alignment];
    size_t retval;
    size_t address = 0;
    long totalzeros = 0;
    while ((retval = fread(bank, 1, arguments.alignment, file)) > 0) {
        long zeros;
        for (zeros = retval - 1; zeros >= 0 && !bank[zeros]; zeros--)
            ;
        zeros = retval - zeros - 1;
        float perc = 100.f * zeros / retval;
        printf("0x%08zX: 0x%04zX bytes, 0x%04lX padding zeros, %.2f %%\n", address, retval, zeros, perc);
        address += retval;
        totalzeros += zeros;
    }
    float perc = 100.f * totalzeros / address;
    printf("0x%zX total bytes, 0x%lX padding zeros, %.2f %%\n", address, totalzeros, perc);

    if (arguments.input)
        fclose(file);

    return EXIT_SUCCESS;
}
