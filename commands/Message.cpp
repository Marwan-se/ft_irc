/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaidi <msaidi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 14:17:11 by msaidi            #+#    #+#             */
/*   Updated: 2024/09/11 18:34:42 by msaidi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"
#include <cstddef>
#include <cstdlib>
#include <cctype>
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
std::string Message::getComm()
{
	return this->comm;
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
void Message::setComm(std::string comm)
{
	this->comm = comm;
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
			std::cout << client.getClient_fd() << std::endl;
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
		else if (client.getisOp())
		{
			Client invited = channels[message.getMsg()].getClientMember(message.getTarget());
			std::vector<std::string> tmp = channels[message.getMsg()].getInvited();
			if (std::find(tmp.begin(), tmp.end(), invited.getClient_nick()) == tmp.end())
				channels[message.getMsg()].getInvited().push_back(invited.getClient_nick());

			std::string m = RPL_INVITING(client.getClient_ip(), client.getClient_nick(), message.getTarget(), message.getMsg());
			send(client.getClient_fd(), m.c_str(), m.length(),0);
			m.clear();

			m = RPL_INVITE(client.getClient_nick(), client.getClient_user(), client.getClient_ip(), message.getTarget(), message.getMsg());
			send(invited.getClient_fd(), m.c_str(), m.length(), 0);
		}
		else {
			std::string m = ERR_CHANOPRIVSNEEDED(client.getClient_nick(), message.getMsg());
			send(client.getClient_fd(), m.c_str(), m.length(),0);
		}
	}
}


void handlingTOPIC(Message message, std::map<std::string, Channel> &channels, Client &client)
{
	if (message.getCommand() == "TOPIC")
	{
		 if (message.getTarget() == ":" || message.getTarget().empty())
		{
				std::string m = ERR_NEEDMOREPARAMS(client.getClient_nick(), "TOPIC");
				send(client.getClient_fd(), m.c_str(), m.length(),0);
				return ;

		}
		if (message.getTarget().at(0) != '#' || channels.find(message.getTarget()) == channels.end())
		{
				std::string m = ERR_NOSUCHCHANNEL(client.getClient_nick(), message.getTarget());
				send(client.getClient_fd(), m.c_str(), m.length(),0);
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
				else if (client.getisOp())
				{
					std::time_t topicTime = std::time(NULL);
					channels[message.getTarget()].setTopic(message.getMsg());
					std::string m = RPL_TOPIC(client.getClient_nick(), channels[message.getTarget()].getName(), channels[message.getTarget()].getTopic());
					send(client.getClient_fd(), m.c_str(), m.length(),0);
					//brodcast to all clients of the channel that topic is changed.
					// RPL_TOPICWHOTIME(client, channel, nickname, time) brodcast this hh
				}
				else
				{
					//not an operator
					std::string m = ERR_CHANOPRIVSNEEDED(client.getClient_nick(), channels[message.getTarget()].getName());
					send(client.getClient_fd(), m.c_str(), m.length(),0);
				}
			}
		}
		else{
			// error not a member of the channel 
			std::string m = ERR_NOTONCHANNEL(client.getClient_nick(), channels[message.getTarget()].getName());
			send(client.getClient_fd(), m.c_str(), m.length(),0);
		}
	}
}


// command(mode) target(#chan) msg(+-mode) comm(client)

void    Server::handlingMODE(Message message, std::map<std::string, Channel> &channels, Client &client)
{

 (void)channels;
	if (message.getCommand() == "MODE")
	{
		if (message.getTarget().empty() || message.getMsg().empty() || message.getComm().empty())
		{
			std::string m = ERR_NEEDMOREPARAMS(client.getClient_nick(), message.getCommand());
			send(client.getClient_fd(), m.c_str(), m.length(),0);
			return ;
		}
		if (channels.find(message.getTarget()) == channels.end())
		{
			std::string m = ERR_NOSUCHCHANNEL(client.getClient_nick(), message.getTarget());
			send(client.getClient_fd(), m.c_str(), m.length(),0);
			return ;
		}
		else if (!client.getisOp())
		{
			std::string m = ERR_CHANOPRIVSNEEDED(client.getClient_nick(), message.getTarget());
			send(client.getClient_fd(), m.c_str(), m.length(),0);
		}
		else {
			if (message.getMsg().length() <= 2 && (message.getMsg().at(0) == '+' || message.getMsg().at(0) == '-'))
			{
				if (message.getMsg() == "+t")
				{
					channels[message.getTarget()].setTopicRES(true);
					std::string m = RPL_CHANNELMODEIS(client.getClient_nick(), client.getClient_user(), client.getClient_ip(), message.getTarget(), "+t", "");
					send(client.getClient_fd(), m.c_str(), m.length(),0);
				}
				else if (message.getMsg() == "-t")
				{
					channels[message.getTarget()].setTopicRES(false);
					std::string m = RPL_CHANNELMODEIS(client.getClient_nick(), client.getClient_user(), client.getClient_ip(), message.getTarget(), "-t", "");
					send(client.getClient_fd(), m.c_str(), m.length(),0);
					return ;
				}
				else if (message.getMsg() == "+i")
				{
					channels[message.getTarget()].setInviteOnly(true);
					std::string m = RPL_CHANNELMODEIS(client.getClient_nick(), client.getClient_user(), client.getClient_ip(), message.getTarget(), "+i", "");
					send(client.getClient_fd(), m.c_str(), m.length(),0);
					return ;
				}
				else if (message.getMsg() == "-i")
				{
					channels[message.getTarget()].setInviteOnly(false);
					std::string m = RPL_CHANNELMODEIS(client.getClient_nick(), client.getClient_user(), client.getClient_ip(), message.getTarget(), "-i", "");
					send(client.getClient_fd(), m.c_str(), m.length(),0);
					return ;
				}
				else if (message.getMsg() == "+k"){
					channels[message.getTarget()].setPass(true);
					channels[message.getTarget()].setKey(message.getComm());
					std::string m = RPL_CHANNELMODEIS(client.getClient_nick(), client.getClient_user(), client.getClient_ip(), message.getTarget(), "+k", message.getComm());
					send(client.getClient_fd(), m.c_str(), m.length(),0);
					return ;
				}
				else if (message.getMsg() == "-k"){
					channels[message.getTarget()].setPass(false);
					channels[message.getTarget()].setKey("");
					std::string m = RPL_CHANNELMODEIS(client.getClient_nick(), client.getClient_user(), client.getClient_ip(), message.getTarget(), "-k", "");
					send(client.getClient_fd(), m.c_str(), m.length(),0);
					return ;
				}
				else if (message.getMsg() == "+l"){
					channels[message.getTarget()].setLimit(true);
					std::stringstream tst(message.getComm());
					size_t num;
					tst >> num;
					channels[message.getTarget()].setLimNum(num);
					std::string m = RPL_CHANNELMODEIS(client.getClient_nick(), client.getClient_user(), client.getClient_ip(), message.getTarget(), "+l", message.getComm());
					send(client.getClient_fd(), m.c_str(), m.length(),0);
					return ;
				}
				else if (message.getMsg() == "-l"){
					channels[message.getTarget()].setLimit(false);
					channels[message.getTarget()].setLimNum(0);
					std::string m = RPL_CHANNELMODEIS(client.getClient_nick(), client.getClient_user(), client.getClient_ip(), message.getTarget(), "-l", message.getComm());
					send(client.getClient_fd(), m.c_str(), m.length(),0);
					return ;
				}
				else if (message.getMsg() == "+o")
				{
					if (!isMember(message.getTarget(), message.getComm()))
					{
						std::string m = ERR_NOSUCHNICK(client.getClient_nick(), message.getComm());
						send(client.getClient_fd(), m.c_str(), m.length(),0);
						return ;
					}
					else
					{
						Client targt = channels[message.getTarget()].getClientMember(message.getComm());
						if (targt.getisOp())
							return ;
						targt.setisOp(true);
						std::string m = RPL_CHANNELMODEIS(client.getClient_nick(), client.getClient_user(), client.getClient_ip(), message.getTarget(), "+o", "");
						send(client.getClient_fd(), m.c_str(), m.length(),0);
					}
				}
				else if (message.getMsg() == "-o")
				{
					if (!isMember(message.getTarget(), message.getComm()))
					{
						std::string m = ERR_NOSUCHNICK(client.getClient_nick(), message.getComm());
						send(client.getClient_fd(), m.c_str(), m.length(),0);
						return ;
					}
					else
					{
						Client targt = channels[message.getTarget()].getClientMember(message.getComm());
						if (!targt.getisOp())
							return ;
						targt.setisOp(false);
						std::string m = RPL_CHANNELMODEIS(client.getClient_nick(), client.getClient_user(), client.getClient_ip(), message.getTarget(), "-o", "");
						send(client.getClient_fd(), m.c_str(), m.length(),0);
						return ;
					}
				}
			}
			else {
				std::string m = ERR_UNKNOWNMODE(client.getClient_nick(), message.getMsg());
				send(client.getClient_fd(), m.c_str(), m.length(),0);
			}
		}

	}
}

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
	
	sample.clear();
	ss >> sample;
	message.setTarget(sample);
	sample.clear();
	ss >> sample;
	message.setMsg(sample);
	sample.clear();
	ss >> sample;
	message.setComm(sample);

	
	handlingINV(message, channels, client);
	handlingTOPIC(message, channels, client);
	handlingMODE(message, channels, client);

}
