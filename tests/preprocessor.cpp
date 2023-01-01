#include <catch2/catch_test_macros.hpp>
#include <lexer.hpp>
#include <preprocessor.hpp>
#include <sstream>

using std::stringstream;

#define SKIP_SECTION(...) if (false)

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
        stringstream macro_input{
            "M: MACRO &X \n"
            "LOAD &X ; read x \n"
            "ADD TWO ; sum 2 \n"
            "ENDMACRO \n"
            "\n"
            "M X\n"
            "M Y\n"
            "A B C\n"};

        stringstream expected{
            "LOAD X \n"
            "ADD TWO \n"
            "LOAD Y \n"
            "ADD TWO \n"
            "A B C\n"};

        REQUIRE(preprocess_equs_ifs(lex(macro_input)) == lex(expected));
    }
}
