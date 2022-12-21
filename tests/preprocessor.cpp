#include <catch2/catch_test_macros.hpp>
#include <lexer.hpp>
#include <preprocessor.hpp>
#include <sstream>

using std::stringstream;

TEST_CASE ("Stage 1 Preprocessor: EQUs and IFs", "[preprocessor]") {
}

TEST_CASE ("Stage 2 Preprocessor: MACROs", "[preprocessor]") {
    // SECTION ("Basic macro functionality") {
    //     stringstream macro_input {
    //         "M: MACRO &X \n"
    //         "LOAD &X ; read x \n"
    //         "ADD TWO ; sum 2 \n"
    //         "ENDMACRO \n"
    //         "\n"
    //         "M X\n"
    //         "M Y\n"
    //     };

    //     stringstream expected {
    //         "LOAD X \n"
    //         "ADD TWO \n"
    //         "LOAD Y \n"
    //         "ADD TWO \n"
    //     };

    //     REQUIRE(preprocess_equs_ifs(lex(macro_input)) == lex(expected));
    // }
}
