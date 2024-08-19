/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msekhsou <msekhsou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 17:20:28 by msekhsou          #+#    #+#             */
/*   Updated: 2024/08/19 12:25:19 by msekhsou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include <cstdlib>
#include <iostream>

bool     check_port(char *num)
{
	int i = 0;
	while (num[i])
	{
		if (!isdigit(num[i]))
			return (false);
		i++;
	}
	return (true);
}

int main(int ac, char **av)
{
	Server server;

	signal(SIGINT, server.signal_received);
	if (ac != 3 || !av[1] || !av[2])
	{
		std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
		return (1);
	}
	// get port from input
	int port = ft_stoi(av[1]);
	//get password from input
	std::string password = av[2];
	try
	{
		if (!check_port(av[1]))
			throw std::runtime_error("Invalid port number");
		if (port < 1024 || port > 65535)
			throw std::runtime_error("Invalid port number");
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return (1);
	}
	try
	{
		while (!Server::signal_received_flag)
			server.Server_connection(port, password);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
 
	return (0);
}
