#include <catch2/catch_test_macros.hpp>
#include <lexer.hpp>
#include <sstream>

TEST_CASE ("Lexer tests", "[lexer]") {
    std::stringstream input{ "LOAD A\nADD B\nSTORE C" };
    auto tokens = lex(input);

    vector<string> expected = { "LOAD", "A", "\n", "ADD", "B", "\n", "STORE", "C" };
    REQUIRE(tokens.size() == expected.size());
    for (size_t i = 0; i < tokens.size(); i++)
        REQUIRE(tokens[i] == expected[i]);
}

