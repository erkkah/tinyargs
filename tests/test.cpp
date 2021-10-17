#include <map>
#include <vector>
#include <string>
#include <functional>
#include <iostream>
#include <cstring>
#include <assert.h>

#include "../args.h"

typedef std::function<void()> TestFunction;
typedef std::map<std::string, TestFunction> TestMap;

static const char* prog = "test";

struct Commandline {
    Commandline(std::initializer_list<std::string> args) : argc(args.size() + 1), arguments(args) {
        argumentVector.push_back(prog);
        for (const auto& arg : arguments) {
            argumentVector.push_back(arg.c_str());
        }
        argv = argumentVector.data();
    }

    int argc;
    const char** argv;
    std::vector<std::string> arguments;
    std::vector<const char*> argumentVector;
};

TestMap tests = { { "Bool option",
                    []() {
                        Commandline cmdline{ "-hilfe", "means", "help" };
                        OPTDEFS(options, BOOLOPT("hilfe"));
                        assert(parseArgs(&cmdline.argc, &cmdline.argv, options));
                        assert(getBoolOption(options, "hilfe"));
                        assert(cmdline.argc == 2);
                    } },
                  { "Int option",
                    []() {
                        Commandline cmdline{ "-count=321", "two", "more" };
                        OPTDEFS(options, INTOPT("count", "99"));
                        assert(parseArgs(&cmdline.argc, &cmdline.argv, options));
                        assert(getIntOption(options, "count") == 321);
                        assert(cmdline.argc == 2);

                        Commandline noOptions{ "one", "and", "a", "half" };
                        assert(parseArgs(&noOptions.argc, &noOptions.argv, options));
                        assert(getIntOption(options, "count") == 99);
                        assert(noOptions.argc == 4);
                    } },
                  { "Float option",
                    []() {
                        Commandline cmdline{ "-ratio=3.125" };
                        OPTDEFS(options, FLOATOPT("ratio", "-78.5"));
                        assert(parseArgs(&cmdline.argc, &cmdline.argv, options));
                        assert(getFloatOption(options, "ratio") == 3.125);
                        assert(cmdline.argc == 0);

                        Commandline noOptions{ "one", "and", "a", "half" };
                        assert(parseArgs(&noOptions.argc, &noOptions.argv, options));
                        assert(getFloatOption(options, "ratio") == -78.5);
                        assert(noOptions.argc == 4);
                    } },
                  { "String option",
                    []() {
                        Commandline cmdline{ "-name=Zerblatt" };
                        OPTDEFS(options, STRINGOPT("name", "Volvo"));
                        assert(parseArgs(&cmdline.argc, &cmdline.argv, options));
                        assert(strcmp(getStringOption(options, "name"), "Zerblatt") == 0);
                        assert(cmdline.argc == 0);

                        Commandline noOptions{ "one", "and", "a", "half" };
                        assert(parseArgs(&noOptions.argc, &noOptions.argv, options));
                        assert(strcmp(getStringOption(options, "name"), "Volvo") == 0);
                        assert(noOptions.argc == 4);
                    } },
                  { "Invalid options",
                    []() {
                        OPTDEFS(options, FLOATOPT("ratio", "-78.5"));

                        Commandline missing1{ "-myOption=3.125" };
                        assert(!parseArgs(&missing1.argc, &missing1.argv, options));
                        assert(missing1.argc == 0);

                        Commandline missing2{ "-myOption" };
                        assert(!parseArgs(&missing2.argc, &missing2.argv, options));
                        assert(missing2.argc == 0);
                    } },
                  { "Option list",
                    []() {
                        OPTDEFS(options, FLOATOPT("ratio", "-78.5"), INTOPT("count", "123"), BOOLOPT("help"),
                                STRINGOPT("name", "noname"));
                        const char* usage = listOptions(options);
                        assert(usage != nullptr);
                        std::string usageString(usage);
                        for (const auto word : { "ratio", "-78.5", "count", "123", "help", "name", "noname" }) {
                            assert(usageString.find(word) != usageString.npos);
                        }
                    } },
                  { "Bools with values are invalid", []() {
                       OPTDEFS(options, BOOLOPT("flag"));
                       Commandline cmdline{ "-flag=false" };
                       assert(!parseArgs(&cmdline.argc, &cmdline.argv, options));
                   } } };

int main() {
    for (const auto& test : tests) {
        std::cout << test.first << ":";
        test.second();
        std::cout << " OK" << std::endl;
    }
}
