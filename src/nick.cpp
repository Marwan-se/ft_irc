/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaidi <msaidi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 08:02:46 by msekhsou          #+#    #+#             */
/*   Updated: 2024/09/14 16:07:35 by msaidi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../inc/server.hpp"
#include "../inc/RPL.hpp"


//NICK 
void	handle_nick_command(int fd, std::string message, std::string rest_of_message, std::map<int , Client> &client_info, Client &client, std::string command)
{
	std::string message_collon;
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
		message_collon = message.substr(1, message.length() - 1);
		if (message_collon.empty())
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
		if (message_collon.find_first_of("#:,*?!@%. '\t'") != std::string::npos || message_collon[0] == '$' || message_collon[0] == '&' \
			|| isdigit(message_collon[0]))
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
		client_info[fd].setClient_nick(message_collon);
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
		client_info[fd].setClient_nick(message);
		client_info[fd].nick_received = true;
	}
}
