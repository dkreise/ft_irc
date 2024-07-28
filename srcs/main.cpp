#include "../inc/Server.hpp"

int main(int argc, char** argv)
{
    Server serv(7777, "pass");
    serv.doPollLoop();

    return (0);
}