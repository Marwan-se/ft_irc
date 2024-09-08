/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaidi <msaidi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 14:17:11 by msaidi            #+#    #+#             */
/*   Updated: 2024/09/08 11:32:27 by msaidi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"

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

// void handlingINV(Message *message, Client client)
// {
// 	bool found = false;
// 	if (message->getCommand() == "INVITE"){
		
// 	}
// }

bool isMember(Channel chn, Client client)
{
	for (std::vector<Client>::iterator it = chn.getMembers().begin(); it != chn.getMembers().end(); it++)
	{
		if (client.getClient_nick() == it->getClient_nick())
			return true;
	}
	return false;
}


void handlingTOPIC(Message message, std::map<std::string, Channel> channels, Client client)
{
	if (message.getCommand() == "TOPIC")
	{
		if (message.getTarget().at(0) != '#' || channels.find(message.getTarget()) == channels.end())
		{
		   std::cout << "Error: channel not found" << std::endl;
		   return ;
        }
        if (isMember(channels[message.getTarget()], client))
		{
			if (!message.getMsg().empty())
			{
				channels[message.getTarget()].setTopic(message.getMsg());
				// send topic update to all members
			}
			else 
			{
				if (channels[message.getTarget()].getTopic().empty())
				{
					std::string m = RPL_NOTOPIC(client.getClient_nick(), channels[message.getTarget()].getName());
					send(client.getClient_fd(), m.c_str(), m.length(),0);
				}
				else
				{
					std::string m = RPL_TOPIC(client.getClient_nick(), channels[message.getTarget()].getName(), channels[message.getTarget()].getTopic());
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
		if (sample == "INVITE"){
			message.setCommand("INVITE");
		}
		else if (sample == "TOPIC"){
			message.setCommand("TOPIC");
		}
		else if (sample == "MODE"){
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
	ss >> sample;

	if (!sample.empty()){
		throw std::runtime_error("Error: too much params");
	}
	
	// handlingINV(message, client);
	handlingTOPIC(message, channels, client);
	// handlingMODE(message, channels, clients);

}
