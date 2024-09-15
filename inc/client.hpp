/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaidi <msaidi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 13:16:09 by msekhsou          #+#    #+#             */
/*   Updated: 2024/09/14 22:48:34 by msaidi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

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
#include <sys/socket.h>
#include <unistd.h>


class	Client
{
	private:
		int					Client_fd;
		std::string			Client_ip;
		std::string			nick;
		std::string			user;
		std::string			hostname;
		std::string			command;
		std::string			message;
		bool				isOp;
		
		bool				is_authenticated;
	public:
		bool				pass_received;
		bool				nick_received;
		bool				user_received;
		Client();
		~Client();
		
		int				getClient_fd();
		bool 			getisOp();
		bool			get_authenticated();
		std::string		getClient_nick();
		std::string		getClient_user();
		std::string		get_hostname();
		std::string		getClient_ip();
		
		
		
		void	setClient_nick(std::string n);
		void	setClient_user(std::string n);
		void	set_hostname();
		void	setClient_fd(int fd);
		void	setClient_ip(std::string ip);
		void 	setisOp(bool x);
		void	set_authenticated();
		void	set_command(std::string cmd);
		void	set_message(std::string msg);
};


void	pass_empty(int fd, std::map<int , Client> &client_info, Client &client, std::string command);
void	bad_pass(int fd, std::map<int , Client> &client_info, Client &client, std::string command);
void	nick_empty(int fd, Client &client, std::string command);
void	handle_pass_command(int fd, std::string password, std::string message, std::string rest_of_message, std::map<int , Client> &client_info, Client &client, std::string command);
void	handle_user_command(int fd, std::string message, std::string rest_of_message, std::map<int , Client> &client_info, Client &client, std::string command);
