/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msekhsou <msekhsou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 13:16:09 by msekhsou          #+#    #+#             */
/*   Updated: 2024/09/08 15:27:13 by msekhsou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

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

class	Client
{
	private:
		int					Client_fd;
		std::string			Client_ip;
		bool				is_authenticated;
		std::string			Client_nickname;
		std::string			command;
		std::string			message;

	public:
		Client();
		int		getClient_fd();
		
		void	setClient_fd(int fd);
		void	setClient_ip(std::string ip);

		void	set_command(std::string cmd);
		void	set_message(std::string msg);

		void	set_authenticated();
		
		bool				pass_received;
		bool				nick_received;
		bool				user_received;
		
		//set client nickname
		void	setClient_nickname(std::string nickname);
		std::string	getClient_nickname();

};


#endif