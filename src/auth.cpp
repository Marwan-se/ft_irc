/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   auth.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msekhsou <msekhsou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 18:03:02 by msekhsou          #+#    #+#             */
/*   Updated: 2024/09/11 23:29:26 by msekhsou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "replies.hpp"
#include "client.hpp"

void	check_isAuth(int fd, std::map<int , Client> &client_info)
{
	if ((client_info[fd].pass_received == true) &&( client_info[fd].nick_received == true) && (client_info[fd].user_received == true))
	{
		client_info[fd].set_authenticated();
		std::cout << "is_autheticated: " << client_info[fd].get_authenticated() << std::endl;
	}

}

void	pass_empty(int fd, std::map<int , Client> &client_info, Client &client, std::string command)
{
	std::string star("*");
	client_info[fd].pass_received = false;
	if (client_info[fd].getClient_nickname().empty())
	{
		if (send(fd, ERR_NEEDMOREPARAMS(client.get_hostname(), star, command).c_str(), ERR_NEEDMOREPARAMS(client.get_hostname(), star, command).length(), 0) < 0)
			std::cerr << "Error: send failed" << std::endl;
	}
	else if (send(fd, ERR_NEEDMOREPARAMS(client.get_hostname(), client.getClient_nickname(), command).c_str(), ERR_NEEDMOREPARAMS(client.get_hostname(), client.getClient_nickname(), command).length(), 0) < 0)
		std::cerr << "Error: send failed" << std::endl;
}

void	bad_pass(int fd, std::map<int , Client> &client_info, Client &client, std::string command)
{
	std::string star("*");
	client_info[fd].pass_received = false;
	if (client.getClient_nickname().empty())
	{
		if (send(fd, ERR_PASSWDMISMATCH(client.get_hostname(), star, command).c_str(), ERR_PASSWDMISMATCH(client.get_hostname(), star, command).length(), 0) < 0)
			std::cerr << "Error: send failed" << std::endl;
	}
	else if (send(fd, ERR_PASSWDMISMATCH(client.get_hostname(), client.getClient_nickname(), command).c_str(), ERR_PASSWDMISMATCH(client.get_hostname(), client.getClient_nickname(), command).length(), 0) < 0)
		std::cerr << "Error: send failed" << std::endl;
}

// void	nick_empty(int fd, Client &client, std::string command)
// {
// 	std::string star("*");
// 	if (send(fd, ERR_ERRONEUSNICKNAME(client.get_hostname(), star, command).c_str(), \
// 		ERR_ERRONEUSNICKNAME(client.get_hostname(), star, command).length(), 0) < 0)
// 		std::cerr << "Error: send failed" << std::endl;
// }