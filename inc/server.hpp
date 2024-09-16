/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaidi <msaidi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 17:20:22 by msekhsou          #+#    #+#             */
/*   Updated: 2024/09/15 23:08:14 by msaidi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once


#include "Message.hpp"
#include "ChannelFile.hpp"
#include "client.hpp"
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


class	Server
{
	private:
		int					Socket_fd;
		struct sockaddr_in	Server_addr;
		std::vector<struct pollfd > fdes;
		std::map<int ,Client> client_info;

	public:
		std::map<std::string ,Channel> channels;
		std::map<int , std::string > ctrl_d;
		Server();
		~Server();
	
		void	init_Socket(int domain, int type, int protocol, int port);
		void	Server_connection(int port, std::string password);

		struct sockaddr_in	getServer_addr();
		int		getSocket_fd();
		
		static bool signal_received_flag;
		static void SignalHandler(int signum);
		
		void	close_allfds();
		void	receive_data(int fd, std::string password);
		//COMMANDS

		void parsingMsg(std::string msg, Client &client);
		void join(Message &comm, Client &client);
		void kick(Message &comm, Client &client);
		void privmsg(Message &comm , Client &client);
		Client client_exist(std::string);
		bool is_member(std::string nick, std::string ch_name);
		bool is_op(std::string nick, std::string ch_name);
		void remove_member(std::string nick, std::string ch_name);
		void remove_from_ch(Client &client, std::map<std::string, Channel> &ch, bool flag);
		void msg_chann(Client client, std::string msg, std::string ch_name,std::string target, std::string command, int i);
		void handlingINV(Message message, std::map<std::string, Channel> &channels, Client &client);
		void handlingMODE(Message message, std::map<std::string, Channel> &channels, Client &client);
		void handlingTOPIC(Message message, std::map<std::string, Channel> &channels, Client &client);
		void broadcastToChan(Client &client, Channel &chann, std::string tar, std::string t, int i);
		void	handle_nick_command(int fd, std::string message, std::string rest_of_message, Client &client, std::string command);
		void	handle_auth(int fd, std::string password, Client &client);
		void chan_nick(Client &, std::string);

};

