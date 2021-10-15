#include "args.h"
#include <stdio.h>

int main(int argc, const char** argv) {
    OptionTemplate options[] = {
        INTOPT("blurgAmount"),
        FLOATOPT("fudgeRatio"),
        STRINGOPT("prefix"),
        BOOLOPT("help"),
        ENDOPT
    };

    if (!parseArgs(argc, argv, options)) {
        printf("Failed to parse cmdline arguments\n");
        return 42;
    }

    if (getArgCount() == 0) {
        printf("Expected some kind of argument. Try again\n");
        return 42;
    }

    const char** args = getArgs();
    printf("Will operate on: [\n");
    for (int i = 0; i < getArgCount(); i++) {
        printf("\t%s\n", args[i]);
    }
    printf("]\n\n");

    printf("With a blurgAmount of %d, a fudgeRatio of %f, using prefix: %s, %shelp requested\n",
        getIntOption("blurgAmount", 9),
        getFloatOption("fudgeRatio", -2.33),
        getStringOption("prefix", "--->"),
        getBoolOption("help", 0) ? "" : "no "
    );

}
