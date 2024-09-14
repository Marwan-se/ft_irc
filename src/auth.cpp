/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   auth.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaidi <msaidi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 18:03:02 by msekhsou          #+#    #+#             */
/*   Updated: 2024/09/14 14:49:32 by msaidi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/server.hpp"
#include "../inc/client.hpp"
#include <string>


void	handle_auth(int fd, std::string password, std::string ctrl_d, std::map<int , Client> &client_info, Client &client)
{
	std::stringstream ctrl_d_stream(ctrl_d);
	std::string command;
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
	else if ( client.get_authenticated() == false)
	{
		if (client.getClient_nick().empty())
			message = ERR_NOTREGISTERED(client.get_hostname(), "*", command);
		else
			message = ERR_NOTREGISTERED(client.get_hostname(), client.getClient_nick(), command);
		send(client.getClient_fd(), message.c_str(), message.size(), 0);
		return;
	}
	if ((client_info[fd].pass_received == true) &&( client_info[fd].nick_received == true) && (client_info[fd].user_received == true))
		client_info[fd].set_authenticated();
	
}
