/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrrhaibi <yrrhaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 17:20:22 by msekhsou          #+#    #+#             */
/*   Updated: 2024/09/11 20:18:30 by yrrhaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once



// #include "commands/Message.hpp"
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


class Channel;
class	Server;
class	Client
{
	private:
		int					Client_fd;
		std::string			Client_ip;
		std::string			nick;
		std::string			user;
		std::string			hostname;
		bool				isOp;
		
		bool				is_authenticated;
	public:
		int		getClient_fd();
		std::string		getClient_nick();
		std::string		getClient_user();
		std::string		gethostname();
		std::string		getClient_ip();
		
		void	setClient_nick(std::string n);
		void	sethostname(std::string n);
		void	setClient_fd(int fd);
		void	setClient_ip(std::string ip);
		void	set_authenticated();
		void sendMsg(const std::string &message);
		bool getisOp();
		void setisOp(bool x);
};

class	Client;
class Channel;
class Message;
class	Server
{
	private:
		int					Socket_fd;
		struct sockaddr_in	Server_addr;
		std::vector<struct pollfd > fdes;
		std::map<int ,Client> client_info;

	public:
		std::map<std::string ,Channel> channels;
	
		void	init_Socket(int domain, int type, int protocol, int port);
		void	Server_connection(int port, std::string password);

		struct sockaddr_in	getServer_addr();
		int		getSocket_fd();
		
		static bool signal_received_flag;
		static void SignalHandler(int signum);

		void	delete_client_data_inpoll(int fd);
		void	close_allfds();

		void	receive_data(int fd);
		//COMMANDS

		void parsingMsg(char *msg, Client &client);
		void join(Message &comm, Client &client);
		void kick(Message &comm, Client &client);
		void privmsg(Message &comm , Client &client);
		Client client_exist(std::string);
		bool is_member(std::string nick, std::string ch_name);
		bool is_op(std::string nick, std::string ch_name);
		void remove_member(std::string nick, std::string ch_name);
		void remove_from_ch(Client &client, std::map<std::string, Channel> &ch);
		void msg_chann(Client client, std::string msg, std::string ch_name, std::string command);
		void handlingINV(Message message, std::map<std::string, Channel> &channels, Client &client);
		void handlingMODE(Message message, std::map<std::string, Channel> &channels, Client &client);
		void handlingTOPIC(Message message, std::map<std::string, Channel> &channels, Client &client);

};

