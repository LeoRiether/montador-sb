#include <catch2/catch_test_macros.hpp>
#include <lexer.hpp>
#include <preprocessor.hpp>
#include <sstream>

using std::stringstream;

#define SKIP_SECTION(...) if (false)

inline Token t(const char* s) {
    return Token{0, 0, s};
}

TEST_CASE("Stage 1 Preprocessor: EQUs", "[preprocessor][equs]") {
    SKIP_SECTION("Basic EQU functionality") {
        stringstream input{
            "X: EQU 0x12E4\n"
            "CONSTANT: CONST X\n"
            "FREESPACE: SPACE X\n"};

        stringstream expected{
            "CONSTANT: CONST 0x12E4\n"
            "FREESPACE: SPACE 0x12E4\n"};

        REQUIRE(preprocess_equs_ifs(lex(input)) == lex(expected));
    }
}

TEST_CASE("Stage 1 Preprocessor: IFs", "[preprocessor][ifs]") {
    SKIP_SECTION("Basic IF functionality") {
        {
            stringstream input{
                "DEBUG: EQU 1\n"
                "IF DEBUG\n"
                "PRINT: HELLO WORLD\n"
                "JMP PRINT\n"};
            stringstream expected{
                "DEBUG: EQU 1\n"
                "PRINT: HELLO WORLD\n"
                "JMP PRINT\n"};
            REQUIRE(preprocess_equs_ifs(lex(input)) == lex(expected));
        }
        {
            stringstream input{
                "DEBUG: EQU 0\n"
                "IF DEBUG\n"
                "PRINT: HELLO WORLD\n"
                "JMP PRINT\n"};
            stringstream expected{
                "DEBUG: EQU 1\n"
                "JMP PRINT\n"};
            REQUIRE(preprocess_equs_ifs(lex(input)) == lex(expected));
        }
    }
}

TEST_CASE("Stage 2 Preprocessor: MACROs", "[preprocessor][macros]") {
    SECTION("Basic macro functionality") {
        vector<Token> input = {
            t("M"), t(":"), t("MACRO"), t("&X"), t("\n"),
            t("LOAD"), t("&X"), t("\n"),
            t("ADD"), t("TWO"), t("\n"),
            t("ENDMACRO"), t("\n"),
            t("M"), t("X"), t("\n"),
            t("M"), t("Y"), t("\n"),
            t("A"), t("B"), t("C"), t("\n"),
        };
        vector<Token> expected = {
            t("LOAD"), t("X"), t("\n"),
            t("LOAD"), t("TWO"), t("\n"),
            t("LOAD"), t("Y"), t("\n"),
            t("LOAD"), t("TWO"), t("\n"),
            t("A"), t("B"), t("C"), t("\n"),
        };

        REQUIRE(preprocess_macros(input) == expected);
    }
    SECTION("Simple macro without arguments") {
        vector<Token> input = {
            t("FIM"), t(":"), t("MACRO"), t("\n"),
            t("JMP"), t("END"), t("\n"),
            t("ENDMACRO"), t("\n"),
            t("X"), t("Y"), t("\n"),
            t("FIM"), t("\n"),
            t("W"), t("\n"),
            t("FIM"), t("\n"),
            t("Z"), t("\n"),
        };
        vector<Token> expected = {
            t("X"), t("Y"), t("\n"),
            t("JMP"), t("END"), t("\n"),
            t("W"), t("\n"),
            t("JMP"), t("END"), t("\n"),
            t("Z"), t("\n"),
        };

        REQUIRE(preprocess_macros(input) == expected);
    }
    SECTION("Macro with 3 arguments, with argument names equal to local variables names") {
        vector<Token> input = {
            t("SUM"), t(":"), t("MACRO"), t("&X"), t("&Y"), t("&Z"), t("\n"),
            t("ADD"), t("&X"), t("\n"),
            t("ADD"), t("&Y"), t("\n"),
            t("ADD"), t("&Z"), t("\n"),
            t("ENDMACRO"), t("\n"),
            t("ADD"), t("A"), t("\n"),
            t("SUM"), t("Y"), t("Z"), t("X"), t("\n"),
            t("ADD"), t("B"), t("\n"),
            t("SUM"), t("A"), t("B"), t("C"), t("\n"),
            t("ADD"), t("C"), t("\n"),
        };
        vector<Token> expected = {
            t("ADD"), t("A"), t("\n"),
            t("ADD"), t("Y"), t("\n"),
            t("ADD"), t("Z"), t("\n"),
            t("ADD"), t("X"), t("\n"),
            t("ADD"), t("B"), t("\n"),
            t("ADD"), t("A"), t("\n"),
            t("ADD"), t("B"), t("\n"),
            t("ADD"), t("C"), t("\n"),
            t("ADD"), t("C"), t("\n"),
        };

        REQUIRE(preprocess_macros(input) == expected);
    }
    SECTION("Macro with more than 3 arguments") {
        vector<Token> input = {
            t("SUM"), t(":"), t("MACRO"), t("&X"), t("&Y"), t("&W"), t("&Z"), t("\n"),
            t("ADD"), t("&X"), t("\n"),
            t("ADD"), t("&Y"), t("\n"),
            t("ADD"), t("&W"), t("\n"),
            t("ADD"), t("&Z"), t("\n"),
            t("ENDMACRO"), t("\n"),
        };

        REQUIRE_THROWS(preprocess_macros(input));
    }
    SECTION("Calling macro with non matching arguments") {
        vector<Token> input = {
            t("SUM"), t(":"), t("MACRO"), t("&X"), t("&Y"), t("&Z"), t("\n"),
            t("ADD"), t("&X"), t("\n"),
            t("ADD"), t("&Y"), t("\n"),
            t("ADD"), t("&W"), t("\n"),
            t("ADD"), t("&Z"), t("\n"),
            t("ENDMACRO"), t("\n"),
            t("SUM"), t("A"), t("B"), t("\n"),
        };

        REQUIRE_THROWS(preprocess_macros(input));
    }
    SECTION("Define more than 2 macros") {
        vector<Token> input = {
            t("X"), t(":"), t("MACRO"), t("\n"),
            t("ADD"), t("UM"), t("\n"),
            t("ENDMACRO"), t("\n"),
            t("Y"), t(":"), t("MACRO"), t("\n"),
            t("ADD"), t("DOIS"), t("\n"),
            t("ENDMACRO"), t("\n"),
            t("Z"), t(":"), t("MACRO"), t("\n"),
            t("ADD"), t("TRES"), t("\n"),
            t("ENDMACRO"), t("\n"),
        };

        REQUIRE_THROWS(preprocess_macros(input));
    }
}
