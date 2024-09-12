/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   auth.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msekhsou <msekhsou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 18:03:02 by msekhsou          #+#    #+#             */
/*   Updated: 2024/09/13 00:20:30 by msekhsou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "replies.hpp"
#include "client.hpp"
#include <string>


void	handle_auth(int fd, std::string password, std::string ctrl_d, std::map<int , Client> &client_info, Client &client)
{
	std::stringstream ctrl_d_stream(ctrl_d);
	std::string command;
	std::string message;
	std::string rest_of_message;

	ctrl_d_stream >> command;
	std ::cout << "command: " << command << std::endl;	
	client.set_command(command);

	for (size_t i = 0; i < command.length(); i++)
		command[i] = std::toupper(command[i]);

	ctrl_d_stream >> message;
	client.set_message(message);
		
	std::getline(ctrl_d_stream, rest_of_message);
	if (command == "PASS")
		handle_pass_command(fd, password, message, rest_of_message, client_info, client, command);	
	else if (command == "USER")
		handle_user_command(fd, message, rest_of_message, client_info, client, command);
	else if (command == "NICK")
		handle_nick_command(fd, message, rest_of_message, client_info, client, command);
	if ((client_info[fd].pass_received == true) &&( client_info[fd].nick_received == true) && (client_info[fd].user_received == true))
	{
		client_info[fd].set_authenticated();
		std::cout << "is_autheticated: " << client_info[fd].get_authenticated() << std::endl;
	}
	
	//
	
}

//PASS 

void	pass_empty(int fd, std::map<int , Client> &client_info, Client &client, std::string command)
{
	std::string star("*");
	client_info[fd].pass_received = false;
	if (client_info[fd].getClient_nickname().empty())
	{
		if (send(fd, ERR_NEEDMOREPARAMS(client.get_hostname(), star, command).c_str(), \
			ERR_NEEDMOREPARAMS(client.get_hostname(), star, command).length(), 0) < 0)
			std::cerr << "Error: send failed" << std::endl;
	}
	else if (send(fd, ERR_NEEDMOREPARAMS(client.get_hostname(), client.getClient_nickname(), command).c_str(), \
		ERR_NEEDMOREPARAMS(client.get_hostname(), client.getClient_nickname(), command).length(), 0) < 0)
		std::cerr << "Error: send failed" << std::endl;
}

void	bad_pass(int fd, std::map<int , Client> &client_info, Client &client, std::string command)
{
	std::string star("*");
	client_info[fd].pass_received = false;
	if (client.getClient_nickname().empty())
	{
		if (send(fd, ERR_PASSWDMISMATCH(client.get_hostname(), star, command).c_str(), \
			ERR_PASSWDMISMATCH(client.get_hostname(), star, command).length(), 0) < 0)
			std::cerr << "Error: send failed" << std::endl;
	}
	else if (send(fd, ERR_PASSWDMISMATCH(client.get_hostname(), client.getClient_nickname(), command).c_str(), \
		ERR_PASSWDMISMATCH(client.get_hostname(), client.getClient_nickname(), command).length(), 0) < 0)
		std::cerr << "Error: send failed" << std::endl;
}

void	handle_pass_command(int fd, std::string password, std::string message, std::string rest_of_message, std::map<int , Client> &client_info, Client &client, std::string command)
{
	std::string message_collon;

	std::string star("*");
	if (client.get_authenticated() == true)
	{
		if (send(fd, ERR_ALREADYREGISTERED(client.get_hostname(), client.getClient_nickname(), command).c_str(), \
			ERR_ALREADYREGISTERED(client.get_hostname(), client.getClient_nickname(), command).length(), 0) < 0)
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
		trimString(message);
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

//USER 
void	handle_user_command(int fd, std::string message, std::string rest_of_message, std::map<int , Client> &client_info, Client &client, std::string command)
{
	if (client.get_authenticated() == true)
	{
		if (send(fd, ERR_ALREADYREGISTERED(client.get_hostname(), client.getClient_nickname(), command).c_str(), \
			ERR_ALREADYREGISTERED(client.get_hostname(), client.getClient_nickname(), command).length(), 0) < 0)
			std::cerr << "Error: send failed" << std::endl;
		return;
	}
	message += rest_of_message;
	std::stringstream for_split(message);
	std::vector<std::string> user_info;
	std::string temp;
	std::string star("*");
	if (client_info[fd].pass_received == false)
	{
		if (client.getClient_nickname().empty())
		{
			if (send(fd, ERR_NOTREGISTERED(client.get_hostname(), star, command).c_str(), \
				ERR_NOTREGISTERED(client.get_hostname(), star, command).length(), 0) < 0)
				std::cerr << "Error: send failed" << std::endl;
			return;
		}
		if (send(fd, ERR_NOTREGISTERED(client.get_hostname(), client.getClient_nickname(), command).c_str(), \
		ERR_NOTREGISTERED(client.get_hostname(), client.getClient_nickname(), command).length(), 0) < 0)
			std::cerr << "Error: send failed" << std::endl;
		return;
	}
	for (size_t i = 0; i < 4; i++)
	{
		if (for_split >> temp)
			user_info.push_back(temp);
		else
			break;
	}
	if (user_info.size() < 4 || user_info[0][0] == ':')
	{
		if (client.getClient_nickname().empty())
		{
			if (send(fd, ERR_NEEDMOREPARAMS(client.get_hostname(), star, command).c_str(), \
				ERR_NEEDMOREPARAMS(client.get_hostname(), star, command).length(), 0) < 0)
				std::cerr << "Error: send failed" << std::endl;
			return;
		}		
		if (send(fd, ERR_NEEDMOREPARAMS(client.get_hostname(),client.getClient_nickname(), command).c_str(), \
			ERR_NEEDMOREPARAMS(client.get_hostname(),client.getClient_nickname(), command).length(), 0) < 0)
			std::cerr << "Error: send failed" << std::endl;
		return;
	}
	client_info[fd].user_name = user_info[0];
	client_info[fd].real_name = user_info[3];
	client_info[fd].user_received = true;
}

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
		if (client_info[fd].getClient_nickname().empty())
		{
			if (send(fd, ERR_NONICKNAMEGIVEN(client.get_hostname(), star, command).c_str(), \
				ERR_NONICKNAMEGIVEN(client.get_hostname(), star, command).length(), 0) < 0)
				std::cerr << "Error: send failed" << std::endl;
			return;
		}
		else
		{
			if (send(fd, ERR_NONICKNAMEGIVEN(client.get_hostname(), client.getClient_nickname(), command).c_str(), \
				ERR_NONICKNAMEGIVEN(client.get_hostname(), client.getClient_nickname(), command).length(), 0) < 0)
				std::cerr << "Error: send failed" << std::endl;
			return;
		}
	}
	if (message[0] == ':')
	{
		message += rest_of_message;
		trimString(message);
		message_collon = message.substr(1, message.length() - 1);
		std::cout << "message_collon: {" << message_collon << "}" << std::endl;
		std::cout << "message_collon size: {" << message_collon.size() << "}" << std::endl;
		if (message_collon.empty())
		{
			std::cout << "hahahahhahaha: "<< std::endl;
			if (client_info[fd].getClient_nickname().empty())
			{
				if (send(fd, ERR_NONICKNAMEGIVEN(client.get_hostname(), star, command).c_str(), \
					ERR_NONICKNAMEGIVEN(client.get_hostname(), star, command).length(), 0) < 0)
					std::cerr << "Error: send failed" << std::endl;
				return;
			}
			if (send(fd, ERR_NONICKNAMEGIVEN(client.get_hostname(), client.getClient_nickname(), command).c_str(), \
				ERR_NONICKNAMEGIVEN(client.get_hostname(), client.getClient_nickname(), command).length(), 0) < 0)
				std::cerr << "Error: send failed" << std::endl;
			return;
		}
		std::cout << "TESTTTTTTTTT: "<< std::endl;
		if (message_collon.find_first_of("#:,*?!@%. '\t'") != std::string::npos || message_collon[0] == '$' || message_collon[0] == '&' \
			|| isdigit(message_collon[0]))
		{
			if (client_info[fd].getClient_nickname().empty())
			{
				if (send(fd, ERR_ERRONEUSNICKNAME(client.get_hostname(), star, command).c_str(), \
					ERR_ERRONEUSNICKNAME(client.get_hostname(), star, command).length(), 0) < 0)
					std::cerr << "Error: send failed" << std::endl;
				return;
			}
			else if (send(fd, ERR_ERRONEUSNICKNAME(client.get_hostname(), client.getClient_nickname(), command).c_str(), \
				ERR_ERRONEUSNICKNAME(client.get_hostname(), client.getClient_nickname(), command).length(), 0) < 0)
				std::cerr << "Error: send failed" << std::endl;
			return;
		}
		client_info[fd].setClient_nickname(message_collon);
		client_info[fd].nick_received = true;
		std::cout << "nick size is {" << client_info[fd].getClient_nickname().size() << "}" << std::endl;
		std::cout << "nick is: " << client_info[fd].getClient_nickname() << std::endl;
	}
	else
	{
		if (message.find_first_of("#:,*?!@%. '\t'") != std::string::npos || message[0] == '$' || message[0] == '&' \
			|| isdigit(message[0]))
		{
			if (client_info[fd].getClient_nickname().empty())
			{
				if (send(fd, ERR_ERRONEUSNICKNAME(client.get_hostname(), star, command).c_str(), \
					ERR_ERRONEUSNICKNAME(client.get_hostname(), star, command).length(), 0) < 0)
					std::cerr << "Error: send failed" << std::endl;
				return;
			}
			else if (send(fd, ERR_ERRONEUSNICKNAME(client.get_hostname(), client.getClient_nickname(), command).c_str(), \
				ERR_ERRONEUSNICKNAME(client.get_hostname(), client.getClient_nickname(), command).length(), 0) < 0)
				std::cerr << "Error: send failed" << std::endl;
			return;
		}
		for (std::map<int, Client>::iterator it = client_info.begin(); it != client_info.end(); it++)
		{
			if (it->second.getClient_nickname() == message && it->first != fd)
			{
				if (client_info[fd].getClient_nickname().empty())
				{
					if (send(fd, ERR_NICKNAMEINUSE(client.get_hostname(), star, command).c_str(), \
						ERR_NICKNAMEINUSE(client.get_hostname(), star, command).length(), 0) < 0)
						std::cerr << "Error: send failed" << std::endl;
					return;
				}
				else if (send(fd, ERR_NICKNAMEINUSE(client.get_hostname(), client.getClient_nickname(), command).c_str(), \
					ERR_NICKNAMEINUSE(client.get_hostname(), client.getClient_nickname(), command).length(), 0) < 0)
					std::cerr << "Error: send failed" << std::endl;
				return;
			}
		}
		client_info[fd].setClient_nickname(message);
		client_info[fd].nick_received = true;
	}
}
