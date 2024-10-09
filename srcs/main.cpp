#include "../inc/Server.hpp"

bool	allNum(std::string port)
{
	for (std::size_t i = 0; i < port.size(); i++)
	{
		if (isalpha(port[i]))
			return false;
	}
	return true;
}

void	parse_input(int ac, char** av)
{
	if (ac != 3)
		throw std::logic_error("Invalid number of arguments\nUsage <port> <password>");
	if (!strlen(av[1]) || atoi(av[1]) < 1024 || atoi(av[1]) > 65535 || allNum(av[1]) == false)
		throw std::logic_error("Invalid port. Try a port between 0 and 65535");

	std::string pass = av[2];
	if (pass.empty() == true)
		throw std::logic_error("Try to put a password pls");
}


int main(int ac, char **av)
{
	try
	{
		parse_input(ac, av);
		Server server(atoi(av[1]), av[2]);

		server.doPollLoop();
	}
	catch(const std::exception& e)
	{
		std::cerr << "Error: " <<  e.what() << '\n';
	}
		
}