/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msekhsou <msekhsou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 13:17:22 by msekhsou          #+#    #+#             */
/*   Updated: 2024/09/11 17:02:52 by msekhsou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"
#include <string>

Client::Client()
{
	Client_fd = -1;
	is_authenticated = false;
	pass_received = false;
	nick_received = false;
	user_received = false;
	
}

void	Client::set_hostname(std::string host)
{
	hostname = host;
}

std::string	Client::get_hostname()
{
	std::string host;
    char hostname[1024];
    hostname[1023] = '\0';
    gethostname(hostname, 1023);
    return host = hostname;
}

void 	Client::set_command(std::string cmd)
{
	command = cmd;
}

void	Client::set_message(std::string msg)
{
	message = msg;
}

void	Client::set_authenticated()
{
	is_authenticated = true;
}

bool	Client::get_authenticated()
{
	return (is_authenticated);
}

void	Client::setClient_nickname(std::string nickname)
{
	Client_nickname = nickname;
}

std::string	Client::getClient_nickname()
{
	return (Client_nickname);
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

std::string	Client::getClient_ip()
{
	return (Client_ip);
}

std::string	Client::get_message()
{
	return (message);
}

