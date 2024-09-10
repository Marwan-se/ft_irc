/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   auth.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msekhsou <msekhsou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 18:03:02 by msekhsou          #+#    #+#             */
/*   Updated: 2024/09/10 18:10:09 by msekhsou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "replies.hpp"
#include "client.hpp"

// void    handle_pass_collon(std::string &message)
// {
//     if (message[0] == ':')
//     {
//         message += rest_of_message;
// 				trimString(message);
// 				message_collon = message.substr(1, message.length() - 1);
// 				if (message_collon.empty())
// 				{
// 					std::string msg = ": 461 " + client.getClient_nickname() + " :Not enough parameters" + "\r\n";
// 					send(fd, msg.c_str(), msg.length(), 0);
// 					return;
// 				}
// 				if (message_collon != password)
// 				{
// 					client_info[fd].pass_received = false;
// 					std::string msg = ": 464 " + client.getClient_nickname() + " :Password incorrect" + "\r\n";
// 					send(fd, msg.c_str(), msg.length(), 0);
// 					return;
// 				}
// 				if (message_collon == password)
// 				{
// 					client_info[fd].pass_received = true;
// 					return;
// 				}
//     }
// }

// if (command == "PASS")
// 		{
// 			if (message.empty())
// 			{
// 				std::string msg = ": 461 " + client.getClient_nickname() + " :Not enough parameters" + "\r\n";
// 				send(fd, msg.c_str(), msg.length(), 0);
// 				return;
// 			}
// 			if (message[0] == ':')
// 			{
// 				message += rest_of_message;
// 				trimString(message);
// 				message_collon = message.substr(1, message.length() - 1);
// 				if (message_collon.empty())
// 				{
// 					std::string msg = ": 461 " + client.getClient_nickname() + " :Not enough parameters" + "\r\n";
// 					send(fd, msg.c_str(), msg.length(), 0);
// 					return;
// 				}
// 				if (message_collon != password)
// 				{
// 					client_info[fd].pass_received = false;
// 					std::string msg = ": 464 " + client.getClient_nickname() + " :Password incorrect" + "\r\n";
// 					send(fd, msg.c_str(), msg.length(), 0);
// 					return;
// 				}
// 				if (message_collon == password)
// 				{
// 					client_info[fd].pass_received = true;
// 					return;
// 				}
// 			}
// 			if (message != password)
// 			{
// 				client_info[fd].pass_received = false;
// 				std::string msg = ": 464 " + client.getClient_nickname() + " :Password incorrect" + "\r\n";
// 				send(fd, msg.c_str(), msg.length(), 0);
// 				return;
// 			}
// 			if (message == password)
// 			{
// 				client_info[fd].pass_received = true;
// 				return;
// 			}
// 		}
// 		if (command == "USER")
// 		{
// 			message += rest_of_message;
// 			std::stringstream for_split(message);
// 			std::vector<std::string> user_info;
// 			std::string temp;
// 			if (client_info[fd].pass_received == false)
// 			{
// 				std::string msg = ": 451 " + client.getClient_nickname() + " :You have not registered" + "\r\n";
// 				send(fd, msg.c_str(), msg.length(), 0);
// 				return;
// 			}
// 			for (size_t i = 0; i < 4; i++)
// 			{
// 				if (for_split >> temp)
// 					user_info.push_back(temp);
// 				else
// 					break;
// 			}
// 			if (user_info.size() < 4 || user_info[0][0] == ':')
// 			{
// 				std::string msg = ": 461 " + client.getClient_nickname() + " :Not enough parameters" + "\r\n";
// 				send(fd, msg.c_str(), msg.length(), 0);
// 				return;
// 			}
// 		}
// 		if (command == "NICK")
// 		{
// 			if (client_info[fd].pass_received == false)
// 			{
// 				std::string msg = ": 451 " + client.getClient_nickname() + " :You have not registered" + "\r\n";
// 				send(fd, msg.c_str(), msg.length(), 0);
// 				return;
// 			}
// 			if (message.empty())
// 			{
// 				std::string msg = ": 431 " + client.getClient_nickname() + " :No nickname given" + "\r\n";
// 				send(fd, msg.c_str(), msg.length(), 0);
// 				return;
// 			}
// 			if (message[0] == ':')
// 			{
// 				message += rest_of_message;
// 		 		message_collon = message.substr(1, message.length() - 1);
// 				trimString(message_collon);
// 				std::cout << "nick: " << "{" <<message_collon << "}" << std::endl;
// 				if (message_collon.find_first_of("#:,*?!@%. '\t'") != std::string::npos || message_collon[0] == '$' || message_collon[0] == '&' \
// 					|| isdigit(message_collon[0]))
// 				{
// 					std::string msg = ": 432 " + client.getClient_nickname() + " :Erroneous nickname" + "\r\n";
// 					send(fd, msg.c_str(), msg.length(), 0);
// 					return;
// 				}
// 				client_info[fd].setClient_nickname(message_collon);
// 				client_info[fd].nick_received = true;
// 				std::cout << "nick is: " << client_info[fd].getClient_nickname() << std::endl;
// 			}
// 			else
// 			{
// 				if (message.find_first_of("#:,*?!@%. '\t'") != std::string::npos || message[0] == '$' || message[0] == '&' \
// 					|| isdigit(message[0]))
// 				{
// 					std::string msg = ": 432 " + client.getClient_nickname() + " :Erroneous nickname" + "\r\n";
// 					send(fd, msg.c_str(), msg.length(), 0);
// 					return;
// 				}
// 				for (std::map<int, Client>::iterator it = client_info.begin(); it != client_info.end(); it++)
// 				{
// 					if (it->second.getClient_nickname() == message && it->first != fd)
// 					{
// 						std::string msg = ": 433 " + client.getClient_nickname() + " " + message + " :Nickname is already in use" + "\r\n";
// 						send(fd, msg.c_str(), msg.length(), 0);
// 						return;
// 					}
// 				}
// 				client_info[fd].setClient_nickname(message);
// 				client_info[fd].nick_received = true;
// 			}
// 		}