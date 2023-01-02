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
    SKIP_SECTION("Basic macro functionality") {
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

        REQUIRE(preprocess_equs_ifs(input) == expected);
    }
}
