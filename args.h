#ifndef ARGS_H
#define ARGS_H

/*

Tiny commandline argument parser for C99 / C++ 11.

Handles "options first, arguments later" invocation style:

```
prog -option -otherOption=21 argument1 argument2
```

The first argument found stops the search for options.

Example use:

```
// Define options
OPTDEFS(options,
    // Default value is 7676
    INTOPT("debugPort", 7676),
    BOOLOPT("help")
);

// Parse commandline
if (!parseArgs(&argc, &argv, options)) {
    printf("Failed to parse arguments\nValid options:\n%s", listOptions(options));
    exit(1);
}

// There are now argc arguments left at argv.

// Option flags
int helpMode = getBoolOption(options, "help");
int debugPort = getIntOption(options, "debugPort");

```

```
$ myprog -debugPort=4711 argument
```

*/

#include <stdbool.h>

typedef struct OptionTemplate OptionTemplate;

// Defines a list of option definitions, each argument
// after [name] is a call to one of the <TYPE>OPT macros.
#define OPTDEFS(name, ...) \
    OptionTemplate name[] = {__VA_ARGS__, ENDOPT}

// Defines a boolean option with default value FALSE.
#define BOOLOPT(name) \
    {name, "-" name OPTDELIM, INTFMT, "0"}

// Defines a string option with default value.
#define STRINGOPT(name, fallback) \
    {name, STRINGFMT(name), STRINGFMT(name), "-" name "=" fallback}

// Defines a float option with default string value.
#define FLOATOPT(name, fallback) \
    {name, "-" name "=" FLOATFMT, FLOATFMT, fallback}

// Defines an integer option with default string value.
#define INTOPT(name, fallback) \
    {name, "-" name "=" INTFMT, INTFMT, fallback}

// Parses the argument vector defined by the argc, argv pair referenced
// by [argcRef] and [argvRef] using the list of options templates.
//
// After successful invocation, argc and argv are updated to reference
// the remaining arguments. On failure, argc is updated to point to
// the argument that failed to parse (argv[argc] == invalid arg).
bool
parseArgs(int* argcRef, const char*** argvRef, OptionTemplate* templates);

// Returns the value of the named integer option.
// If the option is not known, zero is returned.
int
getIntOption(OptionTemplate* templates, const char* name);

// Returns the value of the named float option.
// If the option is not known, NaN is returned.
float
getFloatOption(OptionTemplate* templates, const char* name);

// Returns the value of the named string option.
// If the option is not known, NULL is returned.
const char*
getStringOption(OptionTemplate* templates, const char* name);

// Returns the value of the named bool option.
// If the option is not known, FALSE is returned.
bool
getBoolOption(OptionTemplate* templates, const char* name);

// Returns a textual description of the provided options.
// The returned text is allocated with malloc and should be
// freed by the caller.
const char*
listOptions(OptionTemplate* templates);

// Max length of string option values.
// Longer strings will be truncated.
// Redefine if needed.
#ifndef STRINGOPTMAX
#define STRINGOPTMAX 64
#endif

// --- Internals ---

typedef struct {
    char stringValue[STRINGOPTMAX];
    int intValue;
    float floatValue;
} ParsedOption;

struct OptionTemplate {
    const char* name;
    const char* pattern;
    const char* fallbackPattern;
    const char* fallback;

    ParsedOption parsed;
};

#define OPTDELIM "%3$n"
#define INTFMT "%2$d" OPTDELIM
#define FLOATFMT "%1$f" OPTDELIM
#define STRINGFMT(s) "-" s "=" OPTDELIM
#define ENDOPT {0}

#endif // ARGS_H
