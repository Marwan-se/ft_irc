/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msekhsou <msekhsou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 08:02:47 by msekhsou          #+#    #+#             */
/*   Updated: 2024/09/13 17:40:29 by msekhsou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../inc/server.hpp"
#include "../inc/RPL.hpp"


//USER 

bool	parse_user(std::string message, Client &client, std::string command, int fd, std::map<int , Client> &client_info)
{
	std::string full_message = message;
	std::stringstream allparam(full_message);
	std::string param;
	int j = 0;
	
	while (j <= 3)
	{
		allparam >> param;
		std::cout << "param: " << param << std::endl;
		if (param.empty() || param[0] == '\0') {
			std::cout << "param is empty" << std::endl;
			break;
		}
		j++;
		int i = 0;
		while (param[i] != '\0' && j <= 3)
		{
			if (param[i] == ':')
			{
				if (send(fd, ERR_NEEDMOREPARAMS(client.get_hostname(), client.getClient_nick(), command).c_str(), \
					ERR_NEEDMOREPARAMS(client.get_hostname(), client.getClient_nick(), command).length(), 0) < 0)
					std::cerr << "Error: send failed" << std::endl;
				return false;
			}
			i++;
		}
		if (j == 1)
			client_info[fd].setClient_user(param);
		param.clear();
		std::cout << "j: " << j << std::endl;
	}
	if (j <= 3)
	{
		if (send(fd, ERR_NEEDMOREPARAMS(client.get_hostname(), client.getClient_nick(), command).c_str(), \
			ERR_NEEDMOREPARAMS(client.get_hostname(), client.getClient_nick(), command).length(), 0) < 0)
			std::cerr << "Error: send failed" << std::endl;
		return false;
	}
	return true;
}


void	handle_user_command(int fd, std::string message, std::string rest_of_message, std::map<int , Client> &client_info, Client &client, std::string command)
{
	if (client.get_authenticated() == true)
	{
		if (send(fd, ERR_ALREADYREGISTERED(client.get_hostname(), client.getClient_nick(), command).c_str(), \
			ERR_ALREADYREGISTERED(client.get_hostname(), client.getClient_nick(), command).length(), 0) < 0)
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
		if (client.getClient_nick().empty())
		{
			if (send(fd, ERR_NOTREGISTERED(client.get_hostname(), star, command).c_str(), \
				ERR_NOTREGISTERED(client.get_hostname(), star, command).length(), 0) < 0)
				std::cerr << "Error: send failed" << std::endl;
			return;
		}
		if (send(fd, ERR_NOTREGISTERED(client.get_hostname(), client.getClient_nick(), command).c_str(), \
		ERR_NOTREGISTERED(client.get_hostname(), client.getClient_nick(), command).length(), 0) < 0)
			std::cerr << "Error: send failed" << std::endl;
		return;
	}
	if (!parse_user(message, client, command, fd, client_info))
		return;
	client_info[fd].user_received = true;
}
