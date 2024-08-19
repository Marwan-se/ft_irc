/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msekhsou <msekhsou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 17:20:22 by msekhsou          #+#    #+#             */
/*   Updated: 2024/08/19 12:20:43 by msekhsou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP


#include <iostream>
#include <string>
#include <sys/poll.h>
#include <vector>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>
#include <csignal>

class	Server
{
	private:
		int					Socket_fd;
		struct sockaddr_in	Server_addr;
		std::vector<struct pollfd > fdes;
	public:
		void	init_Socket(int domain, int type, int protocol, int port);
		void	Server_connection(int port, std::string password);
		struct sockaddr_in	getServer_addr();
		int		getSocket_fd();
		static bool signal_received_flag;
		static void signal_received(int sig);
};

class	Client
{
	private:
		int					Client_fd;
		std::string			Client_ip;
	public:
		int		getClient_fd();
		
		void	setClient_fd(int fd);
		void	setClient_ip(std::string ip);
};

int ft_stoi(std::string str);

#endif