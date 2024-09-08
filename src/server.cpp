/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msekhsou <msekhsou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 17:20:07 by msekhsou          #+#    #+#             */
/*   Updated: 2024/09/08 15:35:28 by msekhsou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "replies.hpp"
#include "client.hpp"

#include <cstddef>
#include <iostream>
#include <iterator>
#include <string>
#include <sys/poll.h>
#include <sstream>
#include <sys/signal.h>
#include <vector>
#include <map>
#include <cstring>
#include <cctype>

bool	Server::signal_received_flag = false;

void Server::SignalHandler(int signum)
{
	(void)signum;
	std::cout << std::endl << "Signal Received!" << std::endl;
	Server::signal_received_flag = true;
}

// void	Server::close_allfds()
// {
// 	for (size_t i = 0; i < client_vec.size(); i++)
// 		close(client_vec[i].getClient_fd());
// 	if (Socket_fd != -1)
// 	{
// 		std::cout << "Server <" << Socket_fd << "> disconnected" << std::endl;
// 		std::cout << "All clients disconnected !!!." << std::endl;
// 		close(Socket_fd);
// 	}
// }

void	Server::close_allfds()
{
	for (size_t i = 0; i < client_info.size(); i++)
		close(client_info[i].getClient_fd());
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

void	Server::receive_data(int fd, std::string password)
{
	Client client = client_info[fd];
	char buffer[1024];
	memset(buffer, 0, sizeof(buffer));
	ssize_t	data = recv(fd, buffer, sizeof(buffer) -1, 0);
	if (data <= 0)
	{
		if (client_info[fd].nick_received == true)
			std::cout << "Client <" << client.getClient_nickname() << "> disconnected" << std::endl;
		else if (client_info[fd].nick_received == false)
			std::cout << "Guest <" << fd << "> disconnected" << std::endl;
		for (size_t i = 0; i < fdes.size(); i++)
		{
			if (fdes[i].fd == fd)
			{
				fdes.erase(fdes.begin() + i);
				break;
			}
		}

		for (size_t i = 0; i < client_info.size(); i++)
		{
			if (client_info[i].getClient_fd() == fdes[i].fd)
			{
				client_info.erase(i);
				break;
			}
		}

		close(fd);
	}
	else
	{
		std::stringstream line(buffer);
		std::string command;
		std::string message;

		line >> command;		
		client.set_command(command);

		for (size_t i = 0; i < command.length(); i++)
			command[i] = std::toupper(command[i]);

		line >> message;
		client.set_message(message);
		
		if (command == "PASS")
		{
			// if (client_info[fd].pass_received == true)
			// {
			// 	std::string msg = ": 462 " + client.getClient_nickname() + " :You may not reregister !" + "\r\n";
			// 	send(fd, msg.c_str(), msg.length(), 0);
			// 	return;
			// }
			if (message.empty())
			{
				std::string msg = ": 461 " + client.getClient_nickname() + " :Not enough parameters" + "\r\n";
				send(fd, msg.c_str(), msg.length(), 0);
				return;
			}
			if (message != password)
			{
				client_info[fd].pass_received = false;
				std::string msg = ": 464 " + client.getClient_nickname() + " :Password incorrect" + "\r\n";
				send(fd, msg.c_str(), msg.length(), 0);
				return;
			}
			if (message == password)
			{
				client_info[fd].pass_received = true;
				return;
			}
		}
		if (command == "NICK")
		{
			if (message.empty())
			{
				std::cerr << "Error: No nickname entered" << std::endl;
				return;
			}
			else
			{
				client_info[fd].setClient_nickname(message);
				client_info[fd].nick_received = true;
				// handle the nickname cases
				//1. if the nickname is already in use in another client
				
				
			}
		}
		if (command == "USER")
		{
			// call the user command function
			
		}
		// print the client fd and the client nickname
		
		
	}
	
}

void	Server::Server_connection(int port, std::string password)
{
	init_Socket(AF_INET, SOCK_STREAM, 0, port);
	
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
					// client_vec.push_back(client);
					client_info.insert(std::pair<int, Client>(incoming_fd, client));
					fdes.push_back(new_client);
					std::cout << "Client <" <<  incoming_fd << "> connected" << std::endl;
				}
				else
					receive_data(fdes[i].fd, password);
			}
		}
	}
	close_allfds();
}
