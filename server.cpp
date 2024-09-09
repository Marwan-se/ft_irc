/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrrhaibi <yrrhaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 17:20:07 by msekhsou          #+#    #+#             */
/*   Updated: 2024/09/09 13:56:40 by yrrhaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include <cstddef>
#include <iostream>
#include <iterator>
#include <string>
#include <sys/poll.h>
#include <sys/signal.h>
#include <vector>
#include <cstring>
#include <map>


bool	Server::signal_received_flag = false;

void	Client::sethostname(std::string n)
{
	this->hostname = n;
}

void	Client::setClient_nick(std::string n)
{
	this->nick = n;
}

std::string		Client::getClient_nick()
{
	return this->nick;
}

std::string		Client::getClient_ip()
{
	return this->Client_ip;
}

std::string		Client::getClient_user()
{
	return this->user;
}

std::string		Client::gethostname()
{

	return this->hostname;
}



void Server::SignalHandler(int signum)
{
	(void)signum;
	std::cout << std::endl << "Signal Received!" << std::endl;
	Server::signal_received_flag = true;
}

void	Server::delete_client_data_inpoll(int fd)
{
	for (size_t i = 0; i < fdes.size(); i++)
	{
		if (fdes[i].fd == fd)
		{
			fdes.erase(fdes.begin() + i);
			break;
		}
	}
	// for(size_t i = 0; i < client_vec.size(); i++)
	// {
	// 	if (client_vec[i].getClient_fd() == fdes[i].fd)
	// 	{
	// 		client_vec.erase(client_vec.begin() + i);
	// 		break;
	// 	}
	// }
}

void	Server::close_allfds()
{
	// for (size_t i = 0; i < client_vec.size(); i++)
	// 	close(client_vec[i].getClient_fd());
	if (Socket_fd != -1)
	{
		std::cout << "Server <" << Socket_fd << "> disconnected" << std::endl;
		std::cout << "All clients disconnected !!!." << std::endl;
		close(Socket_fd);
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

void	Server::init_Socket(int domain, int type, int protocol, int port)
{
	Server_addr.sin_family = domain;
	Server_addr.sin_port = htons(port);
	Server_addr.sin_addr.s_addr = INADDR_ANY;
	Socket_fd = socket(domain, type, protocol);
	if (Socket_fd < 0)
		throw (std::runtime_error("Error: socket failed"));
	int opt = 1;
	if (setsockopt(Socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		throw (std::runtime_error("Error: setsockopt failed"));
	if (fcntl(Socket_fd, F_SETFL, O_NONBLOCK) < 0)
		throw (std::runtime_error("Error: fcntl failed"));
	if (bind(Socket_fd, (struct sockaddr *)&Server_addr, sizeof(Server_addr)) < 0)
		throw (std::runtime_error("Error: bind failed"));
	if (listen(Socket_fd, SOMAXCONN) < 0)
		throw (std::runtime_error("Error: listen failed"));
}

void	Server::receive_data(int fd)
{
	char buffer[1024];
	memset(buffer, 0, sizeof(buffer));
	ssize_t	data = recv(fd, buffer, sizeof(buffer) -1, 0);
	if (data <= 0)
	{
		std::cout << "Client <" << fd << "> disconnected" << std::endl;
		delete_client_data_inpoll(fd);
		// for(size_t i = 0; i < client_vec.size(); i++)
		// {
		// 	if (client_vec[i].getClient_fd() == fdes[i].fd)
		// 	{
		// 		client_vec.erase(client_vec.begin() + i);
		// 		break;
		// 	}
		// }
		// close(fd);
	}
	else
	{
		buffer[data] = '\0';
		client_info[fd].setClient_nick("yah");
		client_info[fd].sethostname("locall");
		client_info[fd].setClient_fd(fd);
		Server::parsingMsg(buffer, client_info[fd]);
	}
	
}

void	Server::Server_connection(int port, std::string password)
{
	init_Socket(AF_INET, SOCK_STREAM, 0, port);

	(void)password;
	
	struct pollfd new_poll;
	new_poll.fd = Socket_fd;
	new_poll.events = POLLIN;
	new_poll.revents = 0;
	fdes.push_back(new_poll);


	int timeout = -1;
	std::cout << "Server <" << Socket_fd << "> connected" << std::endl;
	std::cout << "Waiting to connect clients..." << std::endl;
	while (Server::signal_received_flag == false)
	{
		if ((poll(fdes.data(), fdes.size(), timeout) < 0) && (Server::signal_received_flag == false))
			throw (std::runtime_error("Error: poll failed"));
		for (size_t i = 0; i < fdes.size(); i++)
		{
			if (fdes[i].revents & POLLIN)
			{
				if (fdes[i].fd == Socket_fd)
				{
					Client client;
					std::vector<Client>	client_vec;
					struct sockaddr_in client_addr;
					struct pollfd new_client;
					socklen_t client_len = sizeof(client_addr);

					int incoming_fd = accept(Socket_fd, (sockaddr *)&(client_addr), &client_len);
					if (incoming_fd < 0)
						throw (std::runtime_error("Error: accept failed"));
					if (fcntl(incoming_fd, F_SETFL, O_NONBLOCK) < 0)
						throw (std::runtime_error("Error: fcntl failed"));
					new_client.fd = incoming_fd;
					new_client.events = POLLIN;
					new_client.revents = 0;

					client.setClient_fd(incoming_fd);
					client.setClient_ip(inet_ntoa(client_addr.sin_addr));
					client_vec.push_back(client);
					fdes.push_back(new_client);
					std::cout << "Client <" <<  incoming_fd << "> connected" << std::endl;
				}
				else
					receive_data(fdes[i].fd);
			}
		}
	}
	close_allfds();
}
