#include "Headers/TestMessenger.hpp"

int main()
{
    TestMessenger tester;
    tester.runTests(5, 100); // Run 5 tests with data of size 100 bytes
    return 0;
}
