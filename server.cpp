/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msekhsou <msekhsou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 17:20:07 by msekhsou          #+#    #+#             */
/*   Updated: 2024/08/07 20:03:11 by msekhsou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include <iterator>
#include <sys/_types/_ssize_t.h>
#include <sys/poll.h>
#include <vector>


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
	init_Socket(AF_INET, SOCK_STREAM, 0, port);
	
	struct pollfd new_poll;
	new_poll.fd = Socket_fd;
	new_poll.events = POLLIN;
	new_poll.revents = 0;
	
	fdes.push_back(new_poll);
	
	
	int timeout = -1;
	
	std::cout << "Server <" << Socket_fd << "> is connected" << std::endl;
	std::cout << "Waiting for accept...." << std::endl;

	while (1)
	{
		if (poll(fdes.data(), fdes.size(), timeout) < 0)
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
					bzero(buffer, sizeof(buffer));

					ssize_t	data = recv(fdes[i].fd, buffer, sizeof(buffer), 0);
					if (data < 0)
						throw (std::runtime_error("Client disconnected"));
					else
					{
						buffer[data] = '\0';
						std::cout << "Client <" << fdes[i].fd << "> : " << buffer << std::endl;
					}
				}
			}
		}
	}
}

