/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaidi <msaidi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 14:17:11 by msaidi            #+#    #+#             */
/*   Updated: 2024/09/08 19:29:56 by msaidi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"

#include <cctype>
#include <deque>
#include <iterator>
#include <string>
#include <sstream>
#include <iostream>
#include <sys/socket.h>
#include <vector>
#include "ChannelFile.hpp"
#include "../server.hpp"
#include "RPL.hpp"

Message::Message()
{
	this->command = "";
	this->target = "";
	this->msg = "";
}

Message::~Message()
{
}

std::string Message::getCommand()
{
	return this->command;
}

std::string Message::getTarget()
{
	return this->target;
}

std::string Message::getMsg()
{
	return this->msg;
}

void Message::setMsg(std::string msg)
{
	this->msg = msg;
}

void Message::setTarget(std::string target)
{
	this->target = target;
}

void Message::setCommand(std::string command)
{
	this->command = command;
}

Client& Server::getClientByNick(std::string nick)
{
	std::map<int, Client>::iterator it = client_info.begin();
	while( it != client_info.end())
	{
		if (nick == it->second.getClient_nick())
			return it->second;
		it++;
	}
	return it->second;
}

bool client_exist(std::string nickname, std::vector<Client> &clients)
{
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); it++)
	{
		if (nickname == it->getClient_nick())
			return true;
	}
	return false;
}


bool isMember(Channel chn, std::string client)
{
	for (std::vector<Client>::iterator it = chn.getMembers().begin(); it != chn.getMembers().end(); it++)
	{
		if (client == it->getClient_nick())
			return true;
	}
	return false;
}

void Server::handlingINV(Message message, std::map<std::string, Channel> &channels, Client &client)
{
	if (message.getCommand() == "INVITE")
	{
		if (message.getTarget().empty() || message.getMsg().empty())
		{
			std::string m = ERR_NEEDMOREPARAMS(client.getClient_nick(), "INVITE");
			send(client.getClient_fd(), m.c_str(), m.length(),0);
			return ;
		}
		if (channels.find(message.getMsg()) == channels.end())
		{
			std::string m = ERR_NOSUCHCHANNEL(client.getClient_nick(), message.getMsg());
			send(client.getClient_fd(), m.c_str(), m.length(),0);
			return ;
		}
		if (!client_exist(message.getTarget(), client_vec))
		{
			std::string m = ERR_NOSUCHNICK(client.getClient_nick(), message.getTarget());
			send(client.getClient_fd(), m.c_str(), m.length(),0);
			return ;
		}
		if (isMember(channels[message.getMsg()], message.getTarget()))
		{
			std::string m = ERR_USERONCHANNEL(client.getClient_nick(), message.getTarget(), message.getMsg());
			send(client.getClient_fd(), m.c_str(), m.length(),0);
			return ;
		}
		else
		{
			Client invited = getClientByNick(message.getTarget());
			std::vector<std::string> tmp = channels[message.getMsg()].getInvited();
			if (std::find(tmp.begin(), tmp.end(), invited.getClient_nick()) == tmp.end())
				channels[message.getMsg()].getInvited().push_back(invited.getClient_nick());

			std::string m = RPL_INVITING(client.getClient_ip(), client.getClient_nick(), message.getTarget(), message.getMsg());
			send(client.getClient_fd(), m.c_str(), m.length(),0);
			m.clear();

			m = RPL_INVITE(client.getClient_nick(), client.getClient_user(), client.getClient_ip(), message.getTarget(), message.getMsg());
			send(invited.getClient_fd(), m.c_str(), m.length(), 0);
		}
	}
}


void handlingTOPIC(Message message, std::map<std::string, Channel> channels, Client &client)
{
	if (message.getCommand() == "TOPIC")
	{
		if (message.getTarget().empty())
		{
				std::string m = ERR_NEEDMOREPARAMS(client.getClient_nick(), "TOPIC");
				send(client.getClient_fd(), m.c_str(), m.length(),0);

		}
		if (message.getTarget().at(0) != '#' || channels.find(message.getTarget()) == channels.end())
		{
		   std::cout << "Error: channel not found" << std::endl;
		   return ;
        }
        if (isMember(channels[message.getTarget()], client.getClient_nick()))
		{
			if (!message.getMsg().empty())
			{
				//show topic of the channel 
				std::string m = RPL_TOPIC(client.getClient_nick(), channels[message.getTarget()].getName(), channels[message.getTarget()].getTopic());
				send(client.getClient_fd(), m.c_str(), m.length(), 0);
			}
			else 
			{
				if (channels[message.getTarget()].getTopic().empty())
				{
					std::string m = RPL_NOTOPIC(client.getClient_nick(), channels[message.getTarget()].getName());
					send(client.getClient_fd(), m.c_str(), m.length(),0);
				}
				else if (client.isOp)
				{
					// std::time_t topicTime = std::time(NULL);
					channels[message.getTarget()].setTopic(message.getMsg());
					std::string m = RPL_TOPIC(client.getClient_nick(), channels[message.getTarget()].getName(), channels[message.getTarget()].getTopic());
					send(client.getClient_fd(), m.c_str(), m.length(),0);
					//brodcast to all clients of the channel that topic is changed.
				}
				else{
					//not an operator
					std::string m = ERR_CHANOPRIVSNEEDED(client.getClient_nick(), channels[message.getTarget()].getName());
					send(client.getClient_fd(), m.c_str(), m.length(),0);
				}
			}
		}
		else {
			// error not a member of the channel 
			std::string m = ERR_NOTONCHANNEL(client.getClient_nick(), channels[message.getTarget()].getName());
			send(client.getClient_fd(), m.c_str(), m.length(),0);
		}
	}
}


// bool parseModes(std::string args)
// {
// 	if ((args[0] != '+' || args[0] != '-') && (args[1] == '+' || args[1] == '-')){
// 		std::cout << "Error: invalid mode params!!!!!!!!!!!!" << std::endl;
// 		return 1;
// 	}
// 	if (args[args.length() - 1] == '+' || args[args.length() - 1] == '-'){
// 		std::cout << "Error: invalid mode params" << std::endl;
// 		return 1;
// 	}

// 	std::deque<char> base = {'i', 't', 'k', 'o', 'l'};
// 	std::deque<char> check;
// 	std::deque<char> sign;
// 	sign.push_back(args[0]);
// 	bool none = false;
// 	for (int j = 1; j < args.length(); j++)
// 	{
// 		if (args[j] != '+' && args[j] != '-')
// 		{
// 			for (std::deque<char>::iterator it = base.begin() ; it != base.end(); it++)
// 			{
// 				if (*it == args[j]){
// 					check.push_back(args[j]);
// 					break ;
// 				}
// 			}
// 			if (check.back() != args[j] ){
// 			    std::cout << "Error: mode args!" << std::endl;
// 			    return 0;
// 			}
// 				if (check.back() == args[j+1]){
// 					std::cout << "Error: mode args!" << std::endl;
// 					return 0;
// 				}
// 		}
// 		else if (args[j] == '+' || args[j] == '-'){
// 			sign.push_back(args[j]);
// 		}
// 	}
// 	// for (std::deque<char>::iterator ii = check.begin(); ii != check.end(); ii++)
// 	// {
// 	//     std::cout << *ii << "-";
// 	// }
// 	// std::cout << std::endl;
// 	// for (std::deque<char>::iterator s = sign.begin(); s != sign.end(); s++)
// 	// {
// 	//     std::cout << *s << " | ";
// 	// }
// 	// std::cout << std::endl;
// 	return 1;
// }

// void    handlingMODE(Message *message, std::vector<Channel> *channels, std::vector<Client> *clients)
// {
// 	if (message->getCommand() == "MODE")
// 	{
// 		if (!message->getTarget().empty()  && message->getTarget()[0] == '#')
// 		{
// 			if (!parseModes(message->getMsg())){
// 				return ;
// 			}
// 		}
// 		else if (!message->getTarget().empty())
// 		{
			
// 		}
// 	}
// }
std::string toUpper(std::string str)
{
	for (int i = 0; (unsigned long)i < str.length(); ++i)
	{
		str[i] = std::toupper(str[i]);
	}
	return str;
}

void Server::parsingMsg(char *msg, Client &client)
{
	Message message;

	std::string str = std::string(msg);
	std::stringstream ss(str);
	if (ss.eof()){
		throw std::runtime_error("Error: empty message");
	}
	std::string sample;
	ss >> sample;
	std::string cmdUpper = toUpper(sample);
		if (cmdUpper == "INVITE"){
			message.setCommand("INVITE");
		}
		else if (cmdUpper == "TOPIC"){
			message.setCommand("TOPIC");
		}
		else if (cmdUpper == "MODE"){
			message.setCommand("MODE");
		}

		else{
			return ;
		}
	if (ss.eof()){
		throw std::runtime_error("Error: no target");
	}
	sample.clear();
	ss >> sample;
	message.setTarget(sample);
	sample.clear();
	ss >> sample;
	message.setMsg(sample);
	sample.clear();

	
	handlingINV(message, channels, client);
	handlingTOPIC(message, channels, client);
	// handlingMODE(message, channels, clients);

}
