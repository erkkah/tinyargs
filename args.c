#include <stdio.h>
#include <string.h>

#include "args.h"

static OptionTemplate* templates = 0;
static int argumentCount = 0;
static const char** arguments = 0;

static int parseArg(const char* arg) {
    for (OptionTemplate* t = templates; t->name != 0; t++) {
        size_t consumed = 0;
        int parsed = sscanf(arg, t->pattern, &t->parsed.stringValue, &t->parsed.floatValue, &t->parsed.intValue,
                            &consumed);
        if (parsed > 0) {
            t->parsed.ok = 1;
            return 1;
        }
        if (consumed == strlen(arg)) {
            t->parsed.ok = 1;
            t->parsed.intValue = 1;
            return 1;
        }
    }
    return 0;
}

int parseArgs(int argc, const char** argv, OptionTemplate* tmpl) {
    templates = tmpl;

    for (int i = 1; i < argc; i++) {
        const char* arg = argv[i];
        if (arg[0] != '-') {
            arguments = argv + i;
            argumentCount = argc - i;
            break;
        }
        int ok = parseArg(arg);
        if (!ok) {
            return 0;
        }
    }
    return 1;
}

static const OptionTemplate* findOption(const char* name) {
    for (OptionTemplate* t = templates; t != 0; t++) {
        if (strcmp(name, t->name) == 0) {
            return t;
        }
    }
    return 0;
}

int getIntOption(const char* name, int fallback) {
    const OptionTemplate* t = findOption(name);
    if (t && t->parsed.ok) {
        return t->parsed.intValue;
    }
    return fallback;
}

float getFloatOption(const char* name, float fallback) {
    const OptionTemplate* t = findOption(name);
    if (t && t->parsed.ok) {
        return t->parsed.floatValue;
    }
    return fallback;
}

const char* getStringOption(const char* name, const char* fallback) {
    const OptionTemplate* t = findOption(name);
    if (t && t->parsed.ok) {
        return t->parsed.stringValue;
    }
    return fallback;
}

int getBoolOption(const char* name, int fallback) {
    return getIntOption(name, fallback);
}

int getArgCount() {
    return argumentCount;
}

const char** getArgs() {
    return arguments;
}
