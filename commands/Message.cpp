/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaidi <msaidi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 14:17:11 by msaidi            #+#    #+#             */
/*   Updated: 2024/08/19 19:06:16 by msaidi           ###   ########.fr       */
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

// void handlingNICK(Message *message, std::vector<Client> *clients)
// {
//     if (message->getCommand() == "NICK"){
//         if (message->getTarget() == ""){
//             std::cout << "Error: empty nickname" << std::endl;
//         }
//         if (message->getTarget()[0] == '#'){
//             std::cout << "Error: invalid nickname" << std::endl;
//         }
		// laylahaylalah...
//     }
// }

void handlingINV(Message *message, std::vector<Channel> *channels)
{
	bool found = false;
	if (message->getCommand() == "INVITE"){
		if (message->getTarget() == "" || message->getMsg() == ""){
			std::cout << "Error: empty target or message" << std::endl;
		}
		if (message->getMsg()[0] != '#'){
			std::cout << "Error: invalid channel name" << std::endl;
		}
		for (std::vector<Channel>::iterator it = channels->begin(); it != channels->end(); it++){
			if (it->getName() == message->getMsg()){
				it->addMember(message->getTarget());
				found = true;
				break ;
			}
		}
		if (!found){
			std::cout << "Channel not found" << std::endl;
		}
	}
}

void handlingTOPIC(Message *message, std::vector<Channel> *channels)
{
	if (message->getCommand() == "TOPIC")
	{
		if (message->getTarget() == "" ){
		   std::cout << "Error: empty target" << std::endl;
		if (message->getMsg()[0] != '#')
			std::cout << "Error: invalid channel name"<< std::endl;
		}
		for (std::vector<Channel>::iterator it = channels->begin(); it != channels->end(); it++){
			if (it->getName() == message->getTarget()){
				for ( std::vector<std::string>::iterator it2 = it->getOps().begin(); it2 != it->getOps().end(); it2++){
					if (*it2 == message->getTarget()){
						it->setTopic(message->getMsg());
						return ;
					}
					else{
						std::cout << "You are not an operator" << std::endl;
						return ;
					}
				}
			}
		}
	}
}

bool parseModes(std::string args)
{
	if ((args[0] != '+' || args[0] != '-') && (args[1] == '+' || args[1] == '-')){
		std::cout << "Error: invalid mode params!!!!!!!!!!!!" << std::endl;
		return 1;
	}
	if (args[args.length() - 1] == '+' || args[args.length() - 1] == '-'){
		std::cout << "Error: invalid mode params" << std::endl;
		return 1;
	}

	std::deque<char> base = {'i', 't', 'k', 'o', 'l'};
	std::deque<char> check;
	std::deque<char> sign;
	sign.push_back(args[0]);
	bool none = false;
	for (int j = 1; j < args.length(); j++)
	{
		if (args[j] != '+' && args[j] != '-')
		{
			for (std::deque<char>::iterator it = base.begin() ; it != base.end(); it++)
			{
				if (*it == args[j]){
					check.push_back(args[j]);
					break ;
				}
			}
			if (check.back() != args[j] ){
			    std::cout << "Error: mode args!" << std::endl;
			    return 0;
			}
				if (check.back() == args[j+1]){
					std::cout << "Error: mode args!" << std::endl;
					return 0;
				}
		}
		else if (args[j] == '+' || args[j] == '-'){
			sign.push_back(args[j]);
		}
	}
	// for (std::deque<char>::iterator ii = check.begin(); ii != check.end(); ii++)
	// {
	//     std::cout << *ii << "-";
	// }
	// std::cout << std::endl;
	// for (std::deque<char>::iterator s = sign.begin(); s != sign.end(); s++)
	// {
	//     std::cout << *s << " | ";
	// }
	// std::cout << std::endl;
	return 1;
}

void    handlingMODE(Message *message, std::vector<Channel> *channels, std::vector<Client> *clients)
{
	if (message->getCommand() == "MODE")
	{
		if (!message->getTarget().empty()  && message->getTarget()[0] == '#')
		{
			if (!parseModes(message->getMsg())){
				return ;
			}
		}
		else if (!message->getTarget().empty())
		{
			
		}
	}
}

void parsingMsg(char *msg, Message *message, std::vector<Channel> *channels, std::vector<Client> *clients)
{
	std::string str = std::string(msg);
	std::stringstream ss(str);
	if (ss.eof()){
		throw std::runtime_error("Error: empty message");
	}
	std::string sample;
	ss >> sample;
		if (sample == "INVITE"){
			message->setCommand("INVITE");
		}
		else if (sample == "TOPIC"){
			message->setCommand("TOPIC");
		}
		else if (sample == "MODE"){
			message->setCommand("MODE");
		}
		// else if (sample == "NICK")
		//     message->setCommand("NICK");
		else{
			return ;
		}
	if (ss.eof()){
		throw std::runtime_error("Error: no target");
	}
	sample.clear();
	ss >> sample;
	message->setTarget(sample);
	sample.clear();
	ss >> sample;
	message->setMsg(sample);
	if (!ss.eof()){
		throw std::runtime_error("Error: too much params");
	}

	// handlingNICK(message, clients);
	handlingINV(message, channels);
	handlingTOPIC(message, channels);
	handlingMODE(message, channels, clients);

}
