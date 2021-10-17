#include <stdio.h>
#include <string.h>
#include <math.h>

#include "args.h"

static OptionTemplate* templates = 0;
static int argumentCount = 0;
static const char** arguments = 0;

static int parseOptionTemplate(OptionTemplate* t, const char* option, const char* pattern) {
    size_t consumed = 0;
    int parsed = sscanf(option, pattern, t->parsed.stringValue, &t->parsed.floatValue, &t->parsed.intValue, &consumed);
    if (parsed > 0) {
        return 1;
    }
    if (consumed == strlen(option)) {
        t->parsed.intValue = 1;
        return 1;
    }
    if (consumed > 0 && consumed < strlen(option)) {
        strncpy(t->parsed.stringValue, option + consumed, sizeof(t->parsed.stringValue));
        return 1;
    }
    return 0;
}

static int initOptions() {
    for (OptionTemplate* t = templates; t->name != 0; t++) {
        if (!parseOptionTemplate(t, t->fallback, t->fallbackPattern)) {
            return 0;
        }
    }
    return 1;
}

static int parseOption(const char* arg) {
    for (OptionTemplate* t = templates; t->name != 0; t++) {
        if (parseOptionTemplate(t, arg, t->pattern)) {
            return 1;
        }
    }
    return 0;
}

int parseArgs(int argc, const char** argv, OptionTemplate* tmpl) {
    templates = tmpl;

    if (!initOptions()) {
        return 0;
    }

    for (int i = 1; i < argc; i++) {
        const char* arg = argv[i];
        if (arg[0] != '-') {
            arguments = argv + i;
            argumentCount = argc - i;
            break;
        }
        int ok = parseOption(arg);
        if (!ok) {
            return 0;
        }
    }
    return 1;
}

static const OptionTemplate* findOption(const char* name) {
    for (OptionTemplate* t = templates; t->name != 0; t++) {
        if (strcmp(name, t->name) == 0) {
            return t;
        }
    }
    return 0;
}

int getIntOption(const char* name) {
    const OptionTemplate* t = findOption(name);
    if (t) {
        return t->parsed.intValue;
    }
    return 0;
}

float getFloatOption(const char* name) {
    const OptionTemplate* t = findOption(name);
    if (t) {
        return t->parsed.floatValue;
    }
    return NAN;
}

const char* getStringOption(const char* name) {
    const OptionTemplate* t = findOption(name);
    if (t) {
        return t->parsed.stringValue;
    }
    return 0;
}

int getBoolOption(const char* name) {
    return getIntOption(name);
}

int getArgCount() {
    return argumentCount;
}

const char** getArgs() {
    return arguments;
}
