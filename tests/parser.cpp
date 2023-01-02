#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <lexer.hpp>
#include <parser.hpp>
#include <sstream>

using std::stringstream;

inline Token t(const char* s) {
    return Token{0, 0, s};
}

TEST_CASE("Parser tests", "[parser]") {
    SECTION("Basic parser functionality") {
        stringstream input{
            "SECTION TEXT  \n"
            "LOOP: LOAD X  \n"
            "A:    ADD Y+0xAB3   \n"
            "      STORE X+2 \n"
            "J:            \n"
            "      JMP LOOP\n"};

        vector<Line> expected{
            Line{Line::IsSection, {t("TEXT")}},
            Line{Line::IsLabel, {t("LOOP")}},
            Line{Line::IsInstruction, {t("LOAD"), t("X")}, 0},
            Line{Line::IsLabel, {t("A")}},
            Line{Line::IsInstruction, {t("ADD"), t("Y")}, 0xAB3},
            Line{Line::IsInstruction, {t("STORE"), t("X")}, 2},
            Line{Line::IsLabel, {t("J")}},
            Line{Line::IsInstruction, {t("JMP"), t("LOOP")}},
        };

        REQUIRE(parse(lex(input)) == expected);
    }

    SECTION("COPY instruction") {
        stringstream input{
            "COPY A,B\n"
            "COPY X+2,Y\n"
            "COPY DATA,DATA+0x9F\n"
            "COPY Q+1,Q+3\n"};

        vector<Line> expected{
            Line{Line::IsInstruction, {t("COPY"), t("A"), t("B")}, 0, 0},
            Line{Line::IsInstruction, {t("COPY"), t("X"), t("Y")}, 2, 0},
            Line{Line::IsInstruction, {t("COPY"), t("DATA"), t("DATA")}, 0, 0x9f},
            Line{Line::IsInstruction, {t("COPY"), t("Q"), t("Q")}, 1, 3},
        };

        REQUIRE(parse(lex(input)) == expected);
    }

    SECTION("Weird instructions that don't exist") {
        stringstream input{"MULT X\n"};
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
            vector<Line> expected{Line{Line::IsDirective, {t("CONST")}, 12345}};
            REQUIRE(parse(lex(input)) == expected);
        }
        {
            stringstream input{"CONST -12345"};
            vector<Line> expected{
                Line{Line::IsDirective, {t("CONST")}, -12345}};
            REQUIRE(parse(lex(input)) == expected);
        }
        {
            stringstream input{"SPACE"};
            vector<Line> expected{Line{Line::IsDirective, {t("SPACE")}, 1}};
            REQUIRE(parse(lex(input)) == expected);
        }
        {
            stringstream input{"SPACE 23"};
            vector<Line> expected{Line{Line::IsDirective, {t("SPACE")}, 23}};
            REQUIRE(parse(lex(input)) == expected);
        }
        {
            stringstream input{"SPACE 0xAbc"};
            vector<Line> expected{Line{Line::IsDirective, {t("SPACE")}, 0xABC}};
            REQUIRE(parse(lex(input)) == expected);
        }
        {
            stringstream input{"SPACE -0xAbc"};
            vector<Line> expected{
                Line{Line::IsDirective, {t("SPACE")}, -0xABC}};
            REQUIRE(parse(lex(input)) == expected);
        }
    }

    SECTION("SECTION directives") {
        {
            stringstream input{"SECTION TEXT"};
            vector<Line> expected{Line{Line::IsSection, {t("TEXT")}}};
            REQUIRE(parse(lex(input)) == expected);
        }
        {
            stringstream input{"SECTION DATA"};
            vector<Line> expected{Line{Line::IsSection, {t("DATA")}}};
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

    SECTION("Two labels on the same line should throw") {
        stringstream input{"A: LOAD A"};
        REQUIRE_NOTHROW(parse(lex(input)));

        input = stringstream{"A: B: LOAD A"};
        REQUIRE_THROWS(parse(lex(input)));
    }
}

TEST_CASE("Parser Segfaults", "[parser][segv]") {
    SECTION("This used to segfault sometimes") {
        {
            stringstream input{"SUB"};
            auto tokens = lex(input);
            REQUIRE_THROWS(parse(tokens));
        }
    }
}
