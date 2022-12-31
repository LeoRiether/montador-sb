#include <catch2/catch_test_macros.hpp>
#include <lexer.hpp>
#include <preprocessor.hpp>
#include <sstream>

using std::stringstream;

#define SKIP_SECTION(...) if (false)

TEST_CASE("Stage 1 Preprocessor: EQUs and IFs", "[preprocessor]") {
    SKIP_SECTION("Basic EQU functionality") {
        stringstream equ_input{
            "X: EQU 10\n"
            "CONSTANT: CONST X\n"
            "FREESPACE: SPACE X\n"};

        stringstream expected{
            "CONSTANT: CONST 10\n"
            "FREESPACE: SPACE 10\n"};

        REQUIRE(preprocess_equs_ifs(lex(equ_input)) == lex(expected));
    }

    SKIP_SECTION("Basic IF functionality") {
        // dunno
    }
}

TEST_CASE("Stage 2 Preprocessor: MACROs", "[preprocessor]") {
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
