/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msekhsou <msekhsou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 17:20:07 by msekhsou          #+#    #+#             */
/*   Updated: 2024/08/19 07:39:45 by msekhsou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include <iostream>
#include <iterator>
#include <sys/poll.h>
#include <sys/signal.h>
#include <vector>
#include <cstring>

bool	Server::signal_received_flag = false;

void	Server::signal_received(int signal)
{
    if (signal == SIGINT || signal == SIGQUIT)
	{
		std::cout << "singal received. Exiting..." << std::endl;
        signal_received_flag = true;
    }
}



struct sockaddr_in	Server::getServer_addr()
{
	return (Server_addr);
}

int	Server::getSocket_fd()
{
	return (Socket_fd);
}

int	Client::getClient_fd()
{
	return (Client_fd);
}

void	Client::setClient_fd(int fd)
{
	Client_fd = fd;
}

void	Client::setClient_ip(std::string ip)
{
	Client_ip = ip;
}

int ft_stoi(std::string str)
{
	int res = 0;
	for (size_t i = 0; i < str.size(); i++)
	{
		if (str[i] < '0' || str[i] > '9')
			throw (std::runtime_error("Error: stoi failed"));
		res = res * 10 + str[i] - '0';
	}
	return (res);
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
	int opt = 1;
	if (setsockopt(Socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		throw (std::runtime_error("Error: setsockopt failed"));
	if (fcntl(Socket_fd, F_SETFL, O_NONBLOCK) < 0)
		throw (std::runtime_error("Error: fcntl failed"));
	//bind socket
	if (bind(Socket_fd, (struct sockaddr *)&Server_addr, sizeof(Server_addr)) < 0)
		throw (std::runtime_error("Error: bind failed"));
	if (listen(Socket_fd, SOMAXCONN) < 0)
		throw (std::runtime_error("Error: listen failed"));
}

void	Server::Server_connection(int port)
{
	init_Socket(AF_INET, SOCK_STREAM, 0, port);
	
	bool signal = Server::signal_received_flag = false;
	struct pollfd new_poll;
	new_poll.fd = Socket_fd;
	new_poll.events = POLLIN;
	new_poll.revents = 0;
	fdes.push_back(new_poll);

	int timeout = -1;
	std::cout << "Server <" << Socket_fd << "> connected" << std::endl;
	std::cout << "Waiting to connect clients..." << std::endl;
	while (signal == 0)
	{
		if ((poll(fdes.data(), fdes.size(), timeout) < 0) && (signal == 0))
			throw (std::runtime_error("Error: poll failed"));
		for (size_t i = 0; i < fdes.size(); i++)
		{
			if (fdes[i].revents & POLLIN)
			{
				if (fdes[i].fd == Socket_fd)
				{
					Client client;
					std::vector<Client>	clientsss;
					struct sockaddr_in client_addr;
					struct pollfd new_client;
					socklen_t client_len = sizeof(client_addr);
					int incoming_fd = accept(Socket_fd, (struct sockaddr *)&client_addr, &client_len);
					if (incoming_fd < 0)
						throw (std::runtime_error("Error: accept failed"));
					if (fcntl(incoming_fd, F_SETFL, O_NONBLOCK) < 0)
						throw (std::runtime_error("Error: fcntl failed"));
					new_client.fd = incoming_fd;
					new_client.events = POLLIN;
					new_client.revents = 0;

					client.setClient_fd(incoming_fd);
					client.setClient_ip(inet_ntoa(client_addr.sin_addr));
					clientsss.push_back(client);
					fdes.push_back(new_client);
					std::cout << "Client <" << client.getClient_fd() << "> is connected" << std::endl;
					
				}
				else
				{
					char buffer[512];
					memset(buffer, 0, sizeof(buffer));
					ssize_t	data = recv(fdes[i].fd, buffer, sizeof(buffer), 0);
					if (data <= 0)
					{
						// std::cout << "client '" << Socket_fd << "' disconnected" << std::endl;
						throw (std::runtime_error("client disconnected"));
					}
					else
					{
						buffer[data] = '\0';
						std::cout << "Client <" << fdes[i].fd << "> : " << buffer;
					}
				}
			}
		}
	}
}

