/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msekhsou <msekhsou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 17:20:07 by msekhsou          #+#    #+#             */
/*   Updated: 2024/09/10 18:02:17 by msekhsou         ###   ########.fr       */
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

void trimString(std::string &str)
{
    size_t endpos = str.find_last_not_of(" \t\r\n");
    if (std::string::npos != endpos)
	{
        str = str.substr(0, endpos + 1);
    }
}

void	Server::receive_data(int fd, std::string password)
{
	Client client = client_info[fd];
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
		// for (size_t i = 0; i < client_info.size(); i++)
		// {
		// 	if (client_info[i].getClient_fd() == fdes[i].fd)
		// 	{
		// 		client_info.erase(i);
		// 		break;
		// 	}
		// }		
		client_info.erase(fd);

		close(fd);
	}
	else
	{
		std::stringstream line(buffer);
		std::string command;
		std::string message;
		std::string rest_of_message;
		std::string message_collon;

		line >> command;		
		client.set_command(command);

		for (size_t i = 0; i < command.length(); i++)
			command[i] = std::toupper(command[i]);

		line >> message;
		client.set_message(message);

		std::getline(line, rest_of_message);

		if (command == "PASS")
		{
			if (message.empty())
			{
				std::string msg = ": 461 " + client.getClient_nickname() + " :Not enough parameters" + "\r\n";
				send(fd, msg.c_str(), msg.length(), 0);
				return;
			}
			if (message[0] == ':')
			{
				message += rest_of_message;
				trimString(message);
				message_collon = message.substr(1, message.length() - 1);
				if (message_collon.empty())
				{
					std::string msg = ": 461 " + client.getClient_nickname() + " :Not enough parameters" + "\r\n";
					send(fd, msg.c_str(), msg.length(), 0);
					return;
				}
				if (message_collon != password)
				{
					client_info[fd].pass_received = false;
					std::string msg = ": 464 " + client.getClient_nickname() + " :Password incorrect" + "\r\n";
					send(fd, msg.c_str(), msg.length(), 0);
					return;
				}
				if (message_collon == password)
				{
					client_info[fd].pass_received = true;
					return;
				}
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
		if (command == "USER")
		{
			message += rest_of_message;
			std::stringstream for_split(message);
			std::vector<std::string> user_info;
			std::string temp;
			if (client_info[fd].pass_received == false)
			{
				std::string msg = ": 451 " + client.getClient_nickname() + " :You have not registered" + "\r\n";
				send(fd, msg.c_str(), msg.length(), 0);
				return;
			}
			for (size_t i = 0; i < 4; i++)
			{
				if (for_split >> temp)
					user_info.push_back(temp);
				else
					break;
			}
			if (user_info.size() < 4 || user_info[0][0] == ':')
			{
				std::string msg = ": 461 " + client.getClient_nickname() + " :Not enough parameters" + "\r\n";
				send(fd, msg.c_str(), msg.length(), 0);
				return;
			}
		}
		if (command == "NICK")
		{
			if (client_info[fd].pass_received == false)
			{
				std::string msg = ": 451 " + client.getClient_nickname() + " :You have not registered" + "\r\n";
				send(fd, msg.c_str(), msg.length(), 0);
				return;
			}
			if (message.empty())
			{
				std::string msg = ": 431 " + client.getClient_nickname() + " :No nickname given" + "\r\n";
				send(fd, msg.c_str(), msg.length(), 0);
				return;
			}
			if (message[0] == ':')
			{
				message += rest_of_message;
		 		message_collon = message.substr(1, message.length() - 1);
				trimString(message_collon);
				std::cout << "nick: " << "{" <<message_collon << "}" << std::endl;
				if (message_collon.find_first_of("#:,*?!@%. '\t'") != std::string::npos || message_collon[0] == '$' || message_collon[0] == '&' \
					|| isdigit(message_collon[0]))
				{
					std::string msg = ": 432 " + client.getClient_nickname() + " :Erroneous nickname" + "\r\n";
					send(fd, msg.c_str(), msg.length(), 0);
					return;
				}
				client_info[fd].setClient_nickname(message_collon);
				client_info[fd].nick_received = true;
				std::cout << "nick is: " << client_info[fd].getClient_nickname() << std::endl;
			}
			else
			{
				if (message.find_first_of("#:,*?!@%. '\t'") != std::string::npos || message[0] == '$' || message[0] == '&' \
					|| isdigit(message[0]))
				{
					std::string msg = ": 432 " + client.getClient_nickname() + " :Erroneous nickname" + "\r\n";
					send(fd, msg.c_str(), msg.length(), 0);
					return;
				}
				for (std::map<int, Client>::iterator it = client_info.begin(); it != client_info.end(); it++)
				{
					if (it->second.getClient_nickname() == message && it->first != fd)
					{
						std::string msg = ": 433 " + client.getClient_nickname() + " " + message + " :Nickname is already in use" + "\r\n";
						send(fd, msg.c_str(), msg.length(), 0);
						return;
					}
				}
				client_info[fd].setClient_nickname(message);
				client_info[fd].nick_received = true;
			}
		}
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
					// std::cout << "client ip: " << client.getClient_ip() << std::endl;
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
