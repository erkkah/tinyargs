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
$ myprog -debugPort=4711
```

*/

#ifndef STRINGOPTMAX
#define STRINGOPTMAX 128
#endif

typedef struct {
    char stringValue[STRINGOPTMAX];
    int intValue;
    float floatValue;
} ParsedOption;

typedef struct {
    const char* name;
    const char* pattern;
    const char* fallbackPattern;
    const char* fallback;

    ParsedOption parsed;
} OptionTemplate;

#define OPTIONLIST(name, ...) OptionTemplate name[] = {__VA_ARGS__, ENDOPT}

#define OPTDELIM "%4$n"

#define INTFMT "%3$d" OPTDELIM
#define FLOATFMT "%2$f" OPTDELIM
#define STRINGFMT(s) "-" s "=" OPTDELIM

#define BOOLOPT(b, fallback) {b, "-" b OPTDELIM, INTFMT, fallback}
#define STRINGOPT(s, fallback) {s, STRINGFMT(s), STRINGFMT(s), "-" s "=" fallback}
#define FLOATOPT(f, fallback) {f, "-" f "=" FLOATFMT, FLOATFMT, fallback}
#define INTOPT(i, fallback) {i, "-" i "=" INTFMT, INTFMT, fallback}
#define ENDOPT {0}

int
parseArgs(int argc, const char** argv, OptionTemplate* options);

int
getIntOption(const char* name);

float
getFloatOption(const char* name);

const char*
getStringOption(const char* name);

int
getBoolOption(const char* name);

int
getArgCount();

const char**
getArgs();

#endif // ARGS_H
