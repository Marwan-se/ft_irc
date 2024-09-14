/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   auth.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msekhsou <msekhsou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 18:03:02 by msekhsou          #+#    #+#             */
/*   Updated: 2024/09/14 14:58:06 by msekhsou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/server.hpp"
#include "../inc/client.hpp"
#include <string>


void	welcome_theClient(int fd, Client &client)
{
	
	std::string msg1 = ":irc.1337 001 " + client.getClient_nick() + " :Welcome to the Internet Relay Network " + \
		client.getClient_nick() + "!" + client.getClient_user() + " @ " + client.get_hostname() + "\r\n";
	std::string msg2 = ":irc.1337 002 " + client.getClient_nick() + " :Your host is irc.1337, running version 1.0\r\n";
	std::string msg3 = ":irc.1337 003 " + client.getClient_nick() + " :This server was created 2024/09/13 18:32:19\r\n";
	std::string msg4 = ":irc.1337 004 " + client.getClient_nick() + " irc.1337 1.0 itkol\r\n";
	std::string msg5 = ":irc.1337 005 " + client.getClient_nick() + " CHANTYPES=# :are supported by this server\r\n";
	if (send(fd, msg1.c_str(), msg1.size(), 0) < 0)
		std::cout << "Error: send failed" << std::endl;
	if (send(fd, msg2.c_str(), msg2.size(), 0) < 0)
		std::cout << "Error: send failed" << std::endl;
	if (send(fd, msg3.c_str(), msg3.size(), 0) < 0)
		std::cout << "Error: send failed" << std::endl;
	if (send(fd, msg4.c_str(), msg4.size(), 0) < 0)
		std::cout << "Error: send failed" << std::endl;
	if (send(fd, msg5.c_str(), msg5.size(), 0) < 0)
		std::cout << "Error: send failed" << std::endl;	

}

void	handle_auth(int fd, std::string password, std::string ctrl_d, std::map<int , Client> &client_info, Client &client)
{
	std::stringstream ctrl_d_stream(ctrl_d);
	std::string command;
	std::string commands("JOIN KICK PRIVMSG MODE INVITE TOPIC");
	std::string message;
	std::string rest_of_message;

	ctrl_d_stream >> command;
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
	else if (commands.find(command) != std::string::npos && client.get_authenticated() == false)
	{
		if (client.getClient_nick().empty())
			message = ERR_NOTREGISTERED(client.get_hostname(), "*", command);
		else
			message = ERR_NOTREGISTERED(client.get_hostname(), client.getClient_nick(), command);
		send(client.getClient_fd(), message.c_str(), message.size(), 0);
		return;
	}
	else if (client.get_authenticated() == false)
	{
		if (client.getClient_nick().empty())
			message = ERR_UNKNOWNCOMMAND(client.get_hostname(), "*", command);
		else
			message = ERR_UNKNOWNCOMMAND(client.get_hostname(), client.getClient_nick(), command);
		send(client.getClient_fd(), message.c_str(), message.size(), 0);
		return;
	}
	if ((client_info[fd].pass_received == true) &&( client_info[fd].nick_received == true) && (client_info[fd].user_received == true))
	{
		if (client_info[fd].get_authenticated() == false)
			welcome_theClient(fd, client);
		client_info[fd].set_authenticated();
	}
	

}
