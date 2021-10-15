#ifndef ARGS_H
#define ARGS_H

/*

Tiny commandline argument parser for C / C++

Example use:

```
OptionTemplate options[] = {
    INTOPT("debugPort"),
    BOOLOPT("help"),
    ENDOPT
};

if (!parseArgs(argc, argv, options)) {
    printf("Failed to parse args");
    exit(1);
}

// Option flags
int helpMode = getBoolOption("help", 0);
int debugPort = getIntOption("debugPort", 7676);

// Arguments
const char** args = getArgs();

```

```
$ myprog -debugPort 4711
```

*/

typedef struct {
    const char* name;
    const char* pattern;

    struct {
        int ok;
        const char* stringValue;
        int intValue;
        float floatValue;
    } parsed;
} OptionTemplate;

#define OPTDELIM "%4$n"
#define BOOLOPT(b) {b, "-" b OPTDELIM}
#define STRINGOPT(s) {s, "-" s "=%1$ms" OPTDELIM}
#define FLOATOPT(f) {f, "-" f "=%2$f" OPTDELIM}
#define INTOPT(i) {i, "-" i "=%3$d" OPTDELIM}
#define ENDOPT {0}

int
parseArgs(int argc, const char** argv, OptionTemplate* options);

int
getIntOption(const char* name, int fallback);

float
getFloatOption(const char* name, float fallback);

const char*
getStringOption(const char* name, const char* fallback);

int
getBoolOption(const char* name, int fallback);

int
getArgCount();

const char**
getArgs();

#endif // ARGS_H
