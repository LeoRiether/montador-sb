#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <lexer.hpp>
#include <sstream>

using std::stringstream;

bool operator==(const vector<Token>& a, const vector<string>& b) {
    if (a.size() != b.size())
        return false;

    for (size_t i = 0; i < a.size(); i++) {
        if (a[i] != b[i])
            return false;
    }
    return true;
}

TEST_CASE("Lexer tests", "[lexer]") {
    SECTION("Generates the correct tokens") {
        stringstream input{"LOAD A\nAdd B\nSTORE C"};
        vector<string> expected = {"LOAD", "A",     "\n", "ADD", "B",
                                   "\n",   "STORE", "C",  "\n"};

        REQUIRE(lex(input) == expected);
    }

    SECTION("Resulting tokens are all uppercased") {
        stringstream input{"load A \n a: space 10"};
        vector<string> expected = {"LOAD", "A",     "\n", "A",
                                   ":",    "SPACE", "10", "\n"};

        REQUIRE(lex(input) == expected);
    }

    SECTION("There are no two adjacent newlines") {
        stringstream input{
            "\n \n   input x \n add y  \n\n \n  ; comment\n section text \n"};
        vector<string> expected = {"INPUT", "X",       "\n",   "ADD", "Y",
                                   "\n",    "SECTION", "TEXT", "\n"};

        REQUIRE(lex(input) == expected);
    }

    SECTION("Identifier format") {
        {
            stringstream input{"2X: SPACE"};
            REQUIRE_THROWS(lex(input));
        }
        {
            stringstream input{"LOAD 2X"};
            REQUIRE_THROWS(lex(input));
        }
        {
            stringstream input{"X10: SPACE"};
            vector<string> expected = {"X10", ":", "SPACE", "\n"};
            REQUIRE(lex(input) == expected);
        }
        {
            stringstream input{"X10!: SPACE"};
            REQUIRE_THROWS(lex(input));
        }
        {
            stringstream input{"INPUT L4B3L"};
            vector<string> expected = {"INPUT", "L4B3L", "\n"};
            REQUIRE(lex(input) == expected);
        }
        {
            stringstream input{"INPUT M$NEY"};
            REQUIRE_THROWS(lex(input));
        }
        {
            stringstream input{"INPUT X;comment"};
            vector<string> expected = {"INPUT", "X", "\n"};
            REQUIRE(lex(input) == expected);
        }
        {
            stringstream input{"INPUT (N)"};
            REQUIRE_THROWS(lex(input));
        }
        {
            stringstream input{"INPUT (N)"};
            REQUIRE_THROWS(lex(input));
        }
        {
            stringstream input{"ADD LABEL_WITH_UNDERSCORES"};
            vector<string> expected = {"ADD", "LABEL_WITH_UNDERSCORES", "\n"};
            REQUIRE(lex(input) == expected);
        }
    }

    SECTION("Number format") {
        {
            stringstream input{"N: CONST 10"};
            vector<string> expected = {"N", ":", "CONST", "10", "\n"};
            REQUIRE(lex(input) == expected);
        }
        {
            stringstream input{"N: CONST 0x123ABC"};
            vector<string> expected = {"N", ":", "CONST", "0X123ABC", "\n"};
            REQUIRE(lex(input) == expected);
        }
        {
            stringstream input{"N: CONST -10"};
            vector<string> expected = {"N", ":", "CONST", "-10", "\n"};
            REQUIRE(lex(input) == expected);
        }
        {
            stringstream input{"N: CONST -0x123ABC"};
            vector<string> expected = {"N", ":", "CONST", "-0X123ABC", "\n"};
            REQUIRE(lex(input) == expected);
        }
        {
            stringstream input{"N: CONST 1x234"};
            REQUIRE_THROWS(lex(input));
        }
        {
            stringstream input{"N: SPACE 12A5"};
            REQUIRE_THROWS(lex(input));
        }
    }

    SECTION("Macro arguments") {
        stringstream input{
            "LOADD: MACRO &X, &Y\n"
            "LOAD &X\n"
            "ADD &Y\n"
            "ENDMACRO\n"};
        vector<string> expected = {"LOADD", ":",    "MACRO",    "&X", "&Y",
                                   "\n",    "LOAD", "&X",       "\n", "ADD",
                                   "&Y",    "\n",   "ENDMACRO", "\n"};
        REQUIRE(lex(input) == expected);
    }
    SECTION("Label + Offset") {
        stringstream input{
            "LOAD X+2\n"
            "INPUT DATA+0\n"
            "COPY SOMETHING+0x1A\n"
            "COPY X+1,X+2\n"
            "OOPS DATA+;comment"};
        vector<string> expected = {
            "LOAD", "X", "+", "2", "\n",
            "INPUT", "DATA", "+", "0", "\n",
            "COPY", "SOMETHING", "+", "0X1A", "\n",
            "COPY", "X", "+", "1", "X", "+", "2", "\n",
            "OOPS", "DATA", "+", "\n",
        };
        REQUIRE(lex(input) == expected);
    }
}
