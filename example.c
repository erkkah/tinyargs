#include "args.h"
#include <stdio.h>

int main(int argc, const char** argv) {
    static OPTDEFS(options,
        INTOPT("blurgAmount", "9"),
        FLOATOPT("fudgeRatio", "-2.33"),
        STRINGOPT("trunkPrefix", "-=-"),
        BOOLOPT("help")
    );

    if (!parseArgs(argc, argv, options)) {
        printf("Usage: %s [options] [args]\n\nOptions:\n%s\n", argv[0], listOptions());
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
