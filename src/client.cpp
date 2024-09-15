/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaidi <msaidi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 13:17:22 by msekhsou          #+#    #+#             */
/*   Updated: 2024/09/15 20:05:49 by msaidi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/client.hpp"
#include <string>

Client::Client()
{
	Client_fd = -1;
	is_authenticated = false;
	pass_received = false;
	nick_received = false;
	user_received = false;
	isOp = false;
}

Client::~Client(){}


void	Client::set_hostname()
{
    char host[1024];
    host[1023] = '\0';
    gethostname(host, 1023);
	this->hostname = host;
}

void	Client::setClient_nick(std::string n)
{
	this->nick = n;
}

void	Client::setClient_user(std::string n)
{
	this->user = n;
}

void	Client::setClient_fd(int fd)
{
	this->Client_fd = fd;
}

void	Client::setClient_ip(std::string ip)
{
	this->Client_ip = ip;
}

void 	Client::setisOp(bool x)
{
	this->isOp = x;	
}

void	Client::set_authenticated()
{
	this->is_authenticated = true;	
}

void	Client::set_command(std::string cmd)
{
	this->command = cmd;
}

void	Client::set_message(std::string msg)
{
	this->message = msg;
}

int				Client::getClient_fd()
{
	return this->Client_fd;
}

bool 			Client::getisOp()
{
	return this->isOp;
	
}

bool			Client::get_authenticated()
{
	return this->is_authenticated;
}

std::string		Client::getClient_nick()
{
	return this->nick;
}

std::string		Client::getClient_user()
{
	return this->user;
}

std::string		Client::getClient_ip()
{
	return this->Client_ip;
}


std::string	Client::get_hostname()
{
    return this->hostname;
}
