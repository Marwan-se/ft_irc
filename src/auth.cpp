/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   auth.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msekhsou <msekhsou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 18:03:02 by msekhsou          #+#    #+#             */
/*   Updated: 2024/09/13 08:04:49 by msekhsou         ###   ########.fr       */
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
