[![Build and test](https://github.com/erkkah/tinyargs/actions/workflows/build.yml/badge.svg)](https://github.com/erkkah/tinyargs/actions/workflows/build.yml)
[![Scc Count Badge](https://sloc.xyz/github/erkkah/tinyargs?category=code)](https://github.com/boyter/scc/)

# tinyargs

Another commandline argument parser for C / C++.

This one is tiny, source only, and builds cleanly with `-Wall -pedantic` under C99 and C++11 on macOS, Linux and Windows.

See [header file](args.h) for reference docs.

Example use:
```C
#include "args.h"
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
```

```
$ ./example -fudgeRatio=3.14 -help A B C
Will operate on: [
	A
	B
	C
]
with a blurgAmount of 9, a fudgeRatio of 3.140000, using trunkPrefix: -=-, help requested.
```

```
$ ./example -notAnOption=66
Invalid option "-notAnOption=66".

Usage: ./build/cexample [options] [args]

Options:

-blurgAmount=<int>      (default=9)
-fudgeRatio=<float>     (default=-2.330000)
-trunkPrefix=<string>   (default="-=-")
-help
```
