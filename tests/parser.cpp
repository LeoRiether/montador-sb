#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <lexer.hpp>
#include <parser.hpp>
#include <sstream>

using std::stringstream;

Token t(const char* s) {
    return Token{0, 0, s};
}

TEST_CASE("Parser tests", "[parser]") {
    SECTION("Basic parser functionality") {
        stringstream input{
            "SECTION TEXT  \n"
            "LOOP: LOAD X  \n"
            "A:    ADD Y   \n"
            "      STORE X \n"
            "J:            \n"
            "      JMP LOOP\n"};

        vector<Line> expected {
            Line{Line::IsSection,     {t("TEXT")}},
            Line{Line::IsLabel,       {t("LOOP")}},
            Line{Line::IsInstruction, {t("LOAD"),  t("X")}},
            Line{Line::IsLabel,       {t("A")}},
            Line{Line::IsInstruction, {t("ADD"),   t("Y")}},
            Line{Line::IsInstruction, {t("STORE"), t("X")}},
            Line{Line::IsLabel,       {t("J")}},
            Line{Line::IsInstruction, {t("JMP"),   t("LOOP")}},
        };

        REQUIRE(parse(lex(input)) == expected);
    }

    SECTION("Weird instructions that don't exist") {
        stringstream input { "MULT X\n" };
        REQUIRE_THROWS(parse(lex(input)));
    }

    SECTION("Wrong number of arguments throws") {
        {
            stringstream input{
                "SUB ONE\n"
                "ADD X Y\n"};
            auto tokens = lex(input);
            REQUIRE_THROWS(parse(tokens));
        }
        {
            stringstream input{"SUB"};
            auto tokens = lex(input);
            REQUIRE_THROWS(parse(tokens));
        }
        {
            stringstream input{"CONST"};
            auto tokens = lex(input);
            REQUIRE_THROWS(parse(tokens));
        }
    }

    SECTION("DATA directives") {
        {
            stringstream input{"CONST 12345"};
            vector<Line> expected {
                Line{Line::IsDirective, { t("CONST") }, 12345}
            };
            REQUIRE(parse(lex(input)) == expected);
        }
        {
            stringstream input{"SPACE"};
            vector<Line> expected {
                Line{Line::IsDirective, { t("SPACE") }, 1}
            };
            REQUIRE(parse(lex(input)) == expected);
        }
        {
            stringstream input{"SPACE 23"};
            vector<Line> expected {
                Line{Line::IsDirective, { t("SPACE") }, 23}
            };
            REQUIRE(parse(lex(input)) == expected);
        }
        {
            stringstream input{"SPACE 0xAbc"};
            vector<Line> expected {
                Line{Line::IsDirective, { t("SPACE") }, 0xABC}
            };
            REQUIRE(parse(lex(input)) == expected);
        }
    }

    SECTION("SECTION directives") {
        {
            stringstream input{"SECTION TEXT"};
            vector<Line> expected {
                Line{Line::IsSection, { t("TEXT") }}
            };
            REQUIRE(parse(lex(input)) == expected);
        }
        {
            stringstream input{"SECTION DATA"};
            vector<Line> expected {
                Line{Line::IsSection, { t("DATA") }}
            };
            REQUIRE(parse(lex(input)) == expected);
        }
        {
            stringstream input{"SECTION"};
            auto tokens = lex(input);
            REQUIRE_THROWS(parse(tokens));
        }
        {
            stringstream input{"SECTION SOMETHINGIMADEUP"};
            auto tokens = lex(input);
            REQUIRE_THROWS(parse(tokens));
        }
    }
}

