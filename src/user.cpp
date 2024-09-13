/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msekhsou <msekhsou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 08:02:47 by msekhsou          #+#    #+#             */
/*   Updated: 2024/09/13 10:26:54 by msekhsou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../inc/server.hpp"
#include "../inc/RPL.hpp"


//USER 
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
	for (size_t i = 0; i < 4; i++)
	{
		if (for_split >> temp)
			user_info.push_back(temp);
		else
			break;
	}
	if (user_info.size() < 4 || user_info[0][0] == ':')
	{
		if (client.getClient_nick().empty())
		{
			if (send(fd, ERR_NEEDMOREPARAMS(client.get_hostname(), star, command).c_str(), \
				ERR_NEEDMOREPARAMS(client.get_hostname(), star, command).length(), 0) < 0)
				std::cerr << "Error: send failed" << std::endl;
			return;
		}		
		if (send(fd, ERR_NEEDMOREPARAMS(client.get_hostname(),client.getClient_nick(), command).c_str(), \
			ERR_NEEDMOREPARAMS(client.get_hostname(),client.getClient_nick(), command).length(), 0) < 0)
			std::cerr << "Error: send failed" << std::endl;
		return;
	}
	client_info[fd].setClient_user(user_info[0]);
	client_info[fd].user_received = true;
}
