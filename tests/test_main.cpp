#include <catch2/catch_test_macros.hpp>
#include "mylib.hpp"

TEST_CASE("Addition works", "[add]") {
    REQUIRE(add(1, 2) == 3);
}