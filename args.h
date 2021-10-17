#ifndef ARGS_H
#define ARGS_H

/*

Tiny commandline argument parser for C99 / C++ 11

Example use:

```
OPTDEFS(options,
    INTOPT("debugPort", 7676),
    BOOLOPT("help")
);

if (!parseArgs(argc, argv, options)) {
    printf("Failed to parse arguments\nValid options:\n%s", listOptions());
    exit(1);
}

// Option flags
int helpMode = getBoolOption("help");
int debugPort = getIntOption("debugPort");

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

#define OPTDEFS(name, ...) OptionTemplate name[] = {__VA_ARGS__, ENDOPT}

#define OPTDELIM "%4$n"
#define INTFMT "%3$d" OPTDELIM
#define FLOATFMT "%2$f" OPTDELIM
#define STRINGFMT(s) "-" s "=" OPTDELIM

// Defines a boolean option with default value FALSE.
#define BOOLOPT(name) \
    {name, "-" name OPTDELIM, INTFMT, "0"}

// Defines a string option with fallback value.
#define STRINGOPT(name, fallback) \
    {name, STRINGFMT(name), STRINGFMT(name), "-" name "=" fallback}

// Defines a float option with fallback string value.
#define FLOATOPT(name, fallback) \
    {name, "-" name "=" FLOATFMT, FLOATFMT, fallback}

// Defines an integer option with fallback string value.
#define INTOPT(name, fallback) \
    {name, "-" name "=" INTFMT, INTFMT, fallback}

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

const char*
listOptions();

#endif // ARGS_H
