#include "../third_party/catch.hpp"
#include "../include/CommandProcessor.hpp"

TEST_CASE("CommandProcessor can register and execute commands", "[CommandProcessor]") {
    CommandProcessor processor;

    REQUIRE_NOTHROW(processor.processCommand("start"));  // Should not throw an error
    REQUIRE_NOTHROW(processor.processCommand("stop"));   // Should not throw an error
}
