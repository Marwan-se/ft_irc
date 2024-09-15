/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrrhaibi <yrrhaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 08:02:46 by msekhsou          #+#    #+#             */
/*   Updated: 2024/09/15 13:17:03 by yrrhaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../inc/server.hpp"
#include "../inc/RPL.hpp"
#include <string>
#include <sys/socket.h>

void Server::chan_nick(Client &client, std::string new_nick)
{
	for (std::map<std::string ,Channel>::iterator it = channels.begin(); it != channels.end(); it++)
	{
		if (is_member(client.getClient_nick(), it->second.getName()))
		{
			msg_chann(client,"", it->second.getName(), new_nick , "NICK");
			it->second.getClientMember(client.getClient_nick()).setClient_nick(new_nick);
		}
	}
}

//NICK 
void	Server::handle_nick_command(int fd, std::string message, std::string rest_of_message, Client &client, std::string command)
{
	std::string new_nick;
	std::string star("*");
	if (client_info[fd].pass_received == false)
	{
		if (send(fd, ERR_NOTREGISTERED(client.get_hostname(), star, command).c_str(), \
			ERR_NOTREGISTERED(client.get_hostname(), star, command).length(), 0) < 0)
			std::cerr << "Error: send failed" << std::endl;
		return;
	}
	if (message.empty())
	{
		if (client_info[fd].getClient_nick().empty())
		{
			if (send(fd, ERR_NONICKNAMEGIVEN(client.get_hostname(), star, command).c_str(), \
				ERR_NONICKNAMEGIVEN(client.get_hostname(), star, command).length(), 0) < 0)
				std::cerr << "Error: send failed" << std::endl;
			return;
		}
		else
		{
			if (send(fd, ERR_NONICKNAMEGIVEN(client.get_hostname(), client.getClient_nick(), command).c_str(), \
				ERR_NONICKNAMEGIVEN(client.get_hostname(), client.getClient_nick(), command).length(), 0) < 0)
				std::cerr << "Error: send failed" << std::endl;
			return;
		}
	}
	if (message[0] == ':')
	{
		message += rest_of_message;
		// trimString(message);
		new_nick = message.substr(1, message.length() - 1);
		if (new_nick.empty())
		{
			if (client_info[fd].getClient_nick().empty())
			{
				if (send(fd, ERR_NONICKNAMEGIVEN(client.get_hostname(), star, command).c_str(), \
					ERR_NONICKNAMEGIVEN(client.get_hostname(), star, command).length(), 0) < 0)
					std::cerr << "Error: send failed" << std::endl;
				return;
			}
			if (send(fd, ERR_NONICKNAMEGIVEN(client.get_hostname(), client.getClient_nick(), command).c_str(), \
				ERR_NONICKNAMEGIVEN(client.get_hostname(), client.getClient_nick(), command).length(), 0) < 0)
				std::cerr << "Error: send failed" << std::endl;
			return;
		}
		if (new_nick.find_first_of("#:,*?!@%. '\t'") != std::string::npos || new_nick[0] == '$' || new_nick[0] == '&' \
			|| isdigit(new_nick[0]))
		{
			if (client_info[fd].getClient_nick().empty())
			{
				if (send(fd, ERR_ERRONEUSNICKNAME(client.get_hostname(), star, command).c_str(), \
					ERR_ERRONEUSNICKNAME(client.get_hostname(), star, command).length(), 0) < 0)
					std::cerr << "Error: send failed" << std::endl;
				return;
			}
			else if (send(fd, ERR_ERRONEUSNICKNAME(client.get_hostname(), client.getClient_nick(), command).c_str(), \
				ERR_ERRONEUSNICKNAME(client.get_hostname(), client.getClient_nick(), command).length(), 0) < 0)
				std::cerr << "Error: send failed" << std::endl;
			return;
		}
		if (client_info[fd].get_authenticated() && client_info[fd].getClient_nick() != new_nick)
		{
			std::string rpl;
			chan_nick(client_info[fd], new_nick);
			rpl = ":" + client.getClient_nick() + "!~" + client.getClient_user() + "@" + client.getClient_ip() + " NICK " + new_nick + "\r\n";
			send(client_info[fd].getClient_fd(), rpl.c_str(), rpl.size(), 0);
		}
		client_info[fd].setClient_nick(new_nick);
		client_info[fd].nick_received = true;
	}
	else
	{
		if (message.find_first_of("#:,*?!@%. '\t'") != std::string::npos || message[0] == '$' || message[0] == '&' \
			|| isdigit(message[0]))
		{
			if (client_info[fd].getClient_nick().empty())
			{
				if (send(fd, ERR_ERRONEUSNICKNAME(client.get_hostname(), star, command).c_str(), \
					ERR_ERRONEUSNICKNAME(client.get_hostname(), star, command).length(), 0) < 0)
					std::cerr << "Error: send failed" << std::endl;
				return;
			}
			else if (send(fd, ERR_ERRONEUSNICKNAME(client.get_hostname(), client.getClient_nick(), command).c_str(), \
				ERR_ERRONEUSNICKNAME(client.get_hostname(), client.getClient_nick(), command).length(), 0) < 0)
				std::cerr << "Error: send failed" << std::endl;
			return;
		}
		for (std::map<int, Client>::iterator it = client_info.begin(); it != client_info.end(); it++)
		{
			if (it->second.getClient_nick() == message && it->first != fd)
			{
				if (client_info[fd].getClient_nick().empty())
				{
					if (send(fd, ERR_NICKNAMEINUSE(client.get_hostname(), star, command).c_str(), \
						ERR_NICKNAMEINUSE(client.get_hostname(), star, command).length(), 0) < 0)
						std::cerr << "Error: send failed" << std::endl;
					return;
				}
				else if (send(fd, ERR_NICKNAMEINUSE(client.get_hostname(), client.getClient_nick(), command).c_str(), \
					ERR_NICKNAMEINUSE(client.get_hostname(), client.getClient_nick(), command).length(), 0) < 0)
					std::cerr << "Error: send failed" << std::endl;
				return;
			}
		}
		if (client_info[fd].get_authenticated() && client_info[fd].getClient_nick() != message)
		{
			std::string rpl;
			chan_nick(client_info[fd], message);
			rpl = ":" + client.getClient_nick() + "!~" + client.getClient_user() + "@" + client.getClient_ip() + " NICK " + message + "\r\n";
			send(client_info[fd].getClient_fd(), rpl.c_str(), rpl.size(), 0);
		}
		client_info[fd].setClient_nick(message);
		client_info[fd].nick_received = true;
	}
}
