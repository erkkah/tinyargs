#include "../args.h"
#include <stdio.h>

int main(int argc, const char** argv) {
    static OPTDEFS(opts,
        INTOPT("blurgAmount", "9"),
        FLOATOPT("fudgeRatio", "-2.33"),
        STRINGOPT("trunkPrefix", "-=-"),
        BOOLOPT("help"));

    if (!parseArgs(&argc, &argv, opts)) {
        printf("Invalid option \"%s\".\n\n", argv[argc]);
        printf(
            "Usage: %s [options] [args]\n\n"
            "Options:\n\n%s\n",
            argv[0],
            listOptions(opts));
        return 42;
    }

    printf("Will operate on: [\n");
    for (int i = 0; i < argc; i++) {
        printf("\t%s\n", argv[i]);
    }
    printf("]\n");

    printf(
        "with a blurgAmount of %d, "
        "a fudgeRatio of %f, "
        "using trunkPrefix: %s, "
        "%shelp requested.\n",
        getIntOption(opts, "blurgAmount"),
        getFloatOption(opts, "fudgeRatio"),
        getStringOption(opts, "trunkPrefix"),
        getBoolOption(opts, "help") ? "" : "no ");
}
