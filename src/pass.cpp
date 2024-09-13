/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msekhsou <msekhsou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 08:02:48 by msekhsou          #+#    #+#             */
/*   Updated: 2024/09/13 17:07:30 by msekhsou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/server.hpp"
#include "../inc/RPL.hpp"

//PASS 

void	pass_empty(int fd, std::map<int , Client> &client_info, Client &client, std::string command)
{
	std::string star("*");
	client_info[fd].pass_received = false;
	if (client_info[fd].getClient_nick().empty())
	{
		if (send(fd, ERR_NEEDMOREPARAMS(client.get_hostname(), star, command).c_str(), \
			ERR_NEEDMOREPARAMS(client.get_hostname(), star, command).length(), 0) < 0)
			std::cerr << "Error: send failed" << std::endl;
	}
	else if (send(fd, ERR_NEEDMOREPARAMS(client.get_hostname(), client.getClient_nick(), command).c_str(), \
		ERR_NEEDMOREPARAMS(client.get_hostname(), client.getClient_nick(), command).length(), 0) < 0)
		std::cerr << "Error: send failed" << std::endl;
}

void	bad_pass(int fd, std::map<int , Client> &client_info, Client &client, std::string command)
{
	std::string star("*");
	client_info[fd].pass_received = false;
	if (client.getClient_nick().empty())
	{
		if (send(fd, ERR_PASSWDMISMATCH(client.get_hostname(), star, command).c_str(), \
			ERR_PASSWDMISMATCH(client.get_hostname(), star, command).length(), 0) < 0)
			std::cerr << "Error: send failed" << std::endl;
	}
	else if (send(fd, ERR_PASSWDMISMATCH(client.get_hostname(), client.getClient_nick(), command).c_str(), \
		ERR_PASSWDMISMATCH(client.get_hostname(), client.getClient_nick(), command).length(), 0) < 0)
		std::cerr << "Error: send failed" << std::endl;
}

void	handle_pass_command(int fd, std::string password, std::string message, std::string rest_of_message, std::map<int , Client> &client_info, Client &client, std::string command)
{
	std::string message_collon;

	std::string star("*");
	if (client.get_authenticated() == true)
	{
		if (send(fd, ERR_ALREADYREGISTERED(client.get_hostname(), client.getClient_nick(), command).c_str(), \
			ERR_ALREADYREGISTERED(client.get_hostname(), client.getClient_nick(), command).length(), 0) < 0)
			std::cerr << "Error: send failed" << std::endl;
		return;
	}
	if (message.empty())
	{
		pass_empty(fd, client_info, client, command);
		return;
	}
	if (message[0] == ':')
	{
		message += rest_of_message;
		// trimString(message);
		message_collon = message.substr(1, message.length() - 1);
		if (message_collon.empty())
		{
			pass_empty(fd, client_info, client, command);
			return;
		}
		if (message_collon != password)
		{
			bad_pass(fd, client_info, client, command);
			return;
		}
		if (message_collon == password)
		{
			client_info[fd].pass_received = true;
			return;
		}
	}
	if (message != password)
	{
		bad_pass(fd, client_info, client, command);
		return;
	}
	if (message == password)
	{
		client_info[fd].pass_received = true;
		return;
	}
}