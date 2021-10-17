#include "args.h"
#include <stdio.h>

int main(int argc, const char** argv) {
    static OPTIONLIST(options,
        INTOPT("blurgAmount", "9"),
        FLOATOPT("fudgeRatio", "-2.33"),
        STRINGOPT("trunkPrefix", "-=-"),
        BOOLOPT("help", "0")
    );

    if (!parseArgs(argc, argv, options)) {
        printf("Failed to parse cmdline arguments\n");
        return 42;
    }

    const char** args = getArgs();
    printf("Will operate on: [\n");
    for (int i = 0; i < getArgCount(); i++) {
        printf("\t%s\n", args[i]);
    }
    printf("]\n\n");

    printf("With a blurgAmount of %d, a fudgeRatio of %f, using trunkPrefix: %s, %shelp requested\n",
        getIntOption("blurgAmount"),
        getFloatOption("fudgeRatio"),
        getStringOption("trunkPrefix"),
        getBoolOption("help") ? "" : "no "
    );
}
