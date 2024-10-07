#include "../inc/Server.hpp"

int main(int argc, char** argv)
{
    (void) argc;
    (void) argv;
    Server serv(6667, "pass");
    serv.doPollLoop();

    return (0);
}