/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msekhsou <msekhsou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 17:20:07 by msekhsou          #+#    #+#             */
/*   Updated: 2024/09/07 19:45:31 by msekhsou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

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
bool	Client::pass_received = false;
bool	Client::nick_received = false;
bool	Client::user_received = false;	


void	Client::set_authenticated()
{
	is_authenticated = true;
}

void	Client::setClient_nickname(std::string nickname)
{
	Client_nickname = nickname;
}

std::string	Client::getClient_nickname()
{
	return (Client_nickname);
}

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

void	Server::receive_data(int fd, std::string password)
{
	char buffer[1024];
	memset(buffer, 0, sizeof(buffer));
	ssize_t	data = recv(fd, buffer, sizeof(buffer) -1, 0);
	if (data <= 0)
	{
		std::cout << "Client <" << fd << "> disconnected" << std::endl;
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
		// buffer[data] = '\0';
		// std::cout << "Client <" << fd << "> sent: " << buffer;
		std::stringstream line(buffer);
	
		std::string command;
		std::string rest_of_data;
		
		line >> command;
		//toupper for command
		
		for (size_t i = 0; i < command.length(); i++)
			command[i] = std::toupper(command[i]);
		
		//store the command in a variable && store the rest of the data in another variable
		line >> rest_of_data;
		
		//check if the command is PASS NICK or USER
		// Client client;
		if (command == "PASS")
		{
			// call the pass command function
			if (client_info[fd].pass_received == true)
			{
				std::cerr << "Error: Password already received" << std::endl;
				return;
			}
			if (rest_of_data.empty())
			{
				std::cerr << "Error: No password entered" << std::endl;
				return;
			}
			if (rest_of_data != password)
			{
				std::cerr << "Error: Incorrect password" << std::endl;
				return;
			}
			if (rest_of_data == password)
			{
				client_info[fd].pass_received = true;
				return;
			}
		}
		if (command == "USER")
		{
			// call the user command function
			
		}
		// else if (command == "NICK")
		// {
		// 	// call the nick command function
		// 	if (rest_of_data.empty())
		// 	{
		// 		std::cerr << "Error: No nickname entered" << std::endl;
		// 		return;
		// 	}
		// 	else
		// 	{
		// 		client_info[fd].setClient_nickname(rest_of_data);
		// 		client_info[fd].nick_received = true;
				
		// 		//print whats in the client_info map
		// 		// for (std::map<int, Client>::iterator it = client_info.begin(); it != client_info.end(); it++)
		// 		// {
		// 		// 	std::cout << "fd: " << it->first << " client nickname: " << it->second.getClient_nickname() << std::endl;
					
		// 		// }
				
		// 	}
		// }
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
					std::cout << "Client <" << client_info[incoming_fd].getClient_fd() << std::endl;
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
