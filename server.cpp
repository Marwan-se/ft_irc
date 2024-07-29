/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msekhsou <msekhsou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 17:20:07 by msekhsou          #+#    #+#             */
/*   Updated: 2024/07/29 21:53:24 by msekhsou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include <iostream>
#include <stdexcept>
#include <sys/socket.h>

struct sockaddr_in	Server::getServer_addr()
{
	return (Server_addr);
}

int	Server::getSocket_fd()
{
	return (Socket_fd);
}

void	Server::init_Socket(int domain, int type, int protocol, int port)
{
	//define address structure
	Server_addr.sin_family = domain;
	Server_addr.sin_port = htons(port);
	Server_addr.sin_addr.s_addr = INADDR_ANY;
	//establish socket
	Socket_fd = socket(domain, type, protocol);
	if (Socket_fd < 0)
		throw (std::runtime_error("Error: socket failed"));
	//bind socket
	if (bind(Socket_fd, (struct sockaddr *)&Server_addr, sizeof(Server_addr)) < 0)
		throw (std::runtime_error("Error: bind failed"));
	if (listen(Socket_fd, SOMAXCONN) < 0)
		throw (std::runtime_error("Error: listen failed"));
	
}

void	Server::Server_connection(int port)
{
	// struct pollfd fds[1];
	init_Socket(AF_INET, SOCK_STREAM, 0, port);
	
	std::cout << "Server <" << Socket_fd << "> is connected" << std::endl;
	std::cout << "Waiting for accept...." << std::endl;

	// fds[0].fd = listen_sd;
	// fds[0].events = POLLIN;
	// timeout = -1;

	// while (1)
	// {
	// 	if (poll(fds, 1, -1) < 0) 
	// 		throw (std::runtime_error("Error: poll failed"));
	// }
}


