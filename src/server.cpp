/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msekhsou <msekhsou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 17:20:07 by msekhsou          #+#    #+#             */
/*   Updated: 2024/09/14 02:29:04 by msekhsou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/server.hpp"
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <string>
#include <sys/poll.h>
#include <sys/signal.h>
#include <vector>
#include <cstring>
#include <map>

bool	Server::signal_received_flag = false;

Server::Server()
{}
Server::~Server()
{}

void Server::SignalHandler(int signum)
{
	(void)signum;
	std::cout << std::endl << "Signal Received!" << std::endl;
	Server::signal_received_flag = true;
}

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
	{
		close(Socket_fd);
		throw (std::runtime_error("Error: setsockopt failed"));
	}
	if (fcntl(Socket_fd, F_SETFL, O_NONBLOCK) < 0)
	{
		close(Socket_fd);
		throw (std::runtime_error("Error: fcntl failed"));
	}
	if (bind(Socket_fd, (struct sockaddr *)&Server_addr, sizeof(Server_addr)) < 0)
	{
		close(Socket_fd);
		throw (std::runtime_error("Error: bind failed"));
	}
	if (listen(Socket_fd, SOMAXCONN) < 0)
	{
		close(Socket_fd);
		throw (std::runtime_error("Error: listen failed"));
	}
}

void trimString(std::string &str)
{
    size_t endpos = str.find_last_not_of(" \t\r\n");
    if (std::string::npos != endpos)
	{
        str = str.substr(0, endpos + 1);
    }
}

bool empty_line(std::string s)
{
	for (size_t l = 0;  l < s.size() - 2; l++) 
	{
		if (s[l] != ' ' && s[l] != '\t')
			return false;
	}
	return true;
}

 

void	Server::receive_data(int fd, std::string password)
{
	Client client = client_info[fd];
	char buffer[1024];
	memset(buffer, 0, sizeof(buffer));
	ssize_t	data = recv(fd, buffer, sizeof(buffer), 0);
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
		remove_from_ch(client_info[fd], this->channels, 1);
		client_info.erase(fd);
		ctrl_d.erase(fd);
		close(fd);
	}
	else if (data > 512)
	{
		if (send(fd, ERR_INPUTTOOLONG(client.get_hostname(), client.getClient_nick()).c_str(), ERR_INPUTTOOLONG(client.get_hostname(), client.getClient_nick()).size(), 0) < 0)
			std::cout << "Error: send failed" << std::endl;
		return;
	}
	else
	{
		std::stringstream line(buffer);
		if (line.str().find("\n") != std::string::npos)
		{
			ctrl_d[fd] += line.str();
			if (ctrl_d[fd].size() == 1 || empty_line(ctrl_d[fd]))
			{
				ctrl_d[fd].clear();
				return;
			}
			handle_auth(fd, password, ctrl_d[fd], client_info, client);
			if (client_info[fd].get_authenticated() == true)
				parsingMsg(ctrl_d[fd], client);
			ctrl_d[fd].clear();
		}
		else
			ctrl_d[fd] += line.str();
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
					new_client.fd = incoming_fd;
					new_client.events = POLLIN;
					new_client.revents = 0;

					client.setClient_fd(incoming_fd);
					client.setClient_ip(inet_ntoa(client_addr.sin_addr));
					client_info.insert(std::pair<int, Client>(incoming_fd, client));
					fdes.push_back(new_client);
					ctrl_d.insert(std::pair<int, std::string>(incoming_fd, ""));
					std::cout << "Client <" <<  incoming_fd << "> connected" << std::endl;
					client_info[incoming_fd].set_hostname();
				}
				else
					receive_data(fdes[i].fd, password);
			}
		}
	}
	close_allfds();
}
