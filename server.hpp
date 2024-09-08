/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaidi <msaidi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 17:20:22 by msekhsou          #+#    #+#             */
/*   Updated: 2024/09/08 11:27:32 by msaidi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP


#include "commands/ChannelFile.hpp"
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
#include <map>

class	Client
{
	private:
		int					Client_fd;
		std::string			Client_ip;
		std::string			nick;
		std::string			user;
		
		bool				is_authenticated;
	public:
		bool				isOp;
		int		getClient_fd();
		std::string		getClient_nick();
		std::string		getClient_user();
		
		void	setClient_fd(int fd);
		void	setClient_ip(std::string ip);

		void	set_authenticated();
		void sendMsg(const std::string &message);
};

class	Server
{
	private:
		int					Socket_fd;
		struct sockaddr_in	Server_addr;
		std::vector<struct pollfd > fdes;
		std::vector<Client> client_vec;
		std::map<int, Client> client_info;
		std::map<std::string, Channel> channels;
	public:
	
		void	init_Socket(int domain, int type, int protocol, int port);
		void	Server_connection(int port, std::string password);

		struct sockaddr_in	getServer_addr();
		int		getSocket_fd();
		std::vector<Client> getClient_vec();

		static bool signal_received_flag;
		static void SignalHandler(int signum);

		void	receive_data(int fd);
		void	close_allfds();

		void parsingMsg(char *msg, Client &client);
};



#endif