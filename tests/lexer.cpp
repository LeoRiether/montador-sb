#include <catch2/catch_test_macros.hpp>
#include <lexer.hpp>
#include <sstream>

bool operator==(const vector<Token>& a, const vector<string>& b) {
    if (a.size() != b.size())
        return false;

    for (size_t i = 0; i < a.size(); i++) {
        if (a[i] != b[i])
            return false;
    }
    return true;
}

TEST_CASE ("Lexer tests", "[lexer]") {
    SECTION ("Generates the correct tokens") {
        std::stringstream input{ "LOAD A\nADD B\nSTORE C" };
        vector<string> expected = { "LOAD", "A", "\n", "ADD", "B", "\n", "STORE", "C", "\n" };

        REQUIRE(lex(input) == expected);
    }

    SECTION ("Resulting tokens are all uppercased") {
        std::stringstream input{ "load A \n a: space 10" };
        vector<string> expected = { "LOAD", "A", "\n", "A", ":", "SPACE", "10", "\n" };

        REQUIRE(lex(input) == expected);
    }

    SECTION ("There are no two adjacent newlines") {
        std::stringstream input{ "\n \n   input x \n add y  \n\n \n  ; comment\n section text \n" };
        vector<string> expected = { "INPUT", "X", "\n", "ADD", "Y", "\n", "SECTION", "TEXT", "\n" };

        REQUIRE(lex(input) == expected);
    }
}

