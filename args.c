#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "args.h"

static bool parseOptionTemplate(OptionTemplate* t, const char* option, const char* pattern) {
    size_t consumed = 0;
    int parsed = sscanf(option, pattern, &t->parsed.floatValue, &t->parsed.intValue, &consumed);
    if (parsed > 0) {
        return true;
    }
    if (consumed == strlen(option)) {  // "-flag"
        t->parsed.intValue = 1;
        return true;
    }
    if (consumed > 0 && consumed == 1 + strlen(t->name) + 1) {  // '-' + <name> + '='
        strncpy(t->parsed.stringValue, option + consumed, sizeof(t->parsed.stringValue));
        return true;
    }
    return false;
}

static bool initOptions(OptionTemplate* t) {
    for (; t->name != 0; t++) {
        if (!parseOptionTemplate(t, t->fallback, t->fallbackPattern)) {
            return false;
        }
    }
    return true;
}

static bool parseOption(OptionTemplate* t, const char* arg) {
    for (; t->name != 0; t++) {
        if (parseOptionTemplate(t, arg, t->pattern)) {
            return true;
        }
    }
    return false;
}

bool parseArgs(int* argcRef, const char*** argvRef, OptionTemplate* tmpl) {
    int argc = *argcRef;
    *argcRef = 0;
    const char** argv = *argvRef;

    if (!initOptions(tmpl)) {
        return false;
    }

    for (int i = 1; i < argc; i++) {
        const char* arg = argv[i];
        if (arg[0] != '-') {
            *argvRef = argv + i;
            *argcRef = argc - i;
            break;
        }
        int ok = parseOption(tmpl, arg);
        if (!ok) {
            *argcRef = i;
            return false;
        }
    }
    return true;
}

static const OptionTemplate* findOption(OptionTemplate* t, const char* name) {
    for (; t->name != 0; t++) {
        if (strcmp(name, t->name) == 0) {
            return t;
        }
    }
    return NULL;
}

int getIntOption(OptionTemplate* templates, const char* name) {
    const OptionTemplate* t = findOption(templates, name);
    if (t) {
        return t->parsed.intValue;
    }
    return 0;
}

float getFloatOption(OptionTemplate* templates, const char* name) {
    const OptionTemplate* t = findOption(templates, name);
    if (t) {
        return t->parsed.floatValue;
    }
    return NAN;
}

const char* getStringOption(OptionTemplate* templates, const char* name) {
    const OptionTemplate* t = findOption(templates, name);
    if (t) {
        return t->parsed.stringValue;
    }
    return NULL;
}

bool getBoolOption(OptionTemplate* templates, const char* name) {
    return getIntOption(templates, name);
}

extern char* strdup(const char*);

#define ASPRINTF(result, fmt, ...)                        \
    {                                                     \
        size_t len = snprintf(NULL, 0, fmt, __VA_ARGS__); \
        *(result) = (char*)malloc(len + 1);               \
        snprintf(*(result), len + 1, fmt, __VA_ARGS__);   \
    }

const char* listOptions(OptionTemplate* tmpl) {
    initOptions(tmpl);

    char* result = strdup("");

    for (OptionTemplate* t = tmpl; t->name != 0; t++) {
        const char* previous = result;
        if (!strstr(t->pattern, "=")) {
            // bool
            ASPRINTF(&result, "%s-%s\n", previous, t->name);
        } else if (strstr(t->pattern, INTFMT)) {
            // int
            ASPRINTF(&result, "%s-%s=<int>\t(default=%d)\n", previous, t->name, t->parsed.intValue);
        } else if (strstr(t->pattern, FLOATFMT)) {
            // float
            ASPRINTF(&result, "%s-%s=<float>\t(default=%G)\n", previous, t->name, t->parsed.floatValue);
        } else {
            // string
            ASPRINTF(&result, "%s-%s=<string>\t(default=\"%s\")\n", previous, t->name, t->parsed.stringValue);
        }
        free((void*)previous);
    }

    return result;
}
