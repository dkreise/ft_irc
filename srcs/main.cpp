#include "../inc/Server.hpp"

int main(int argc, char** argv)
{
    (void) argc;
    (void) argv;
    Server serv(7777, "pass");
    serv.doPollLoop();

    return (0);
}