/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaidi <msaidi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 14:17:11 by msaidi            #+#    #+#             */
/*   Updated: 2024/09/15 01:11:15 by msaidi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Message.hpp"
#include <cstddef>
#include <iterator>
#include <map>
#include <ostream>
#include <string>
#include <sstream>
#include <iostream>
#include <sys/socket.h>
#include <system_error>
#include <vector>
#include "../inc/ChannelFile.hpp"
#include "../inc/RPL.hpp"
#include "../inc/server.hpp"
#include "../inc/Tools.hpp"

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

bool isMember(Channel chn, std::string client)
{
	for (std::vector<Client>::iterator it = chn.getMembers().begin(); it != chn.getMembers().end(); it++)
	{
		if (client == it->getClient_nick())
			return true;
	}
	return false;
}

void Server::handlingTOPIC(Message message, std::map<std::string, Channel> &channels, Client &client)
{
	if (message.getCommand() == "TOPIC")
	{
		if (message.getTarget() == ":" || message.getTarget().empty())
		{
				std::string m = ERR_NEEDMOREPARAMS(client.get_hostname(), client.getClient_nick(), "TOPIC");
				send(client.getClient_fd(), m.c_str(), m.length(),0);
				return ;
		}
		if (channels.find(message.getTarget()) == channels.end())
		{
				std::string m = ERR_NOSUCHCHANNEL(client.get_hostname(),client.getClient_nick(), message.getTarget());
				send(client.getClient_fd(), m.c_str(), m.length(),0);
		   return ;
        }
        if (!is_member(client.getClient_nick(), channels[message.getTarget()].getName()))
		{
			std::string m = ERR_NOTONCHANNEL(client.get_hostname(), client.getClient_nick(), channels[message.getTarget()].getName());
			send(client.getClient_fd(), m.c_str(), m.length(),0);
			return ;
		}
		if (!channels[message.getTarget()].getClientMember(client.getClient_nick()).getisOp() && channels[message.getTarget()].getTopicRES())
		{
			std::string m = ERR_CHANOPRIVSNEEDED(client.get_hostname(), client.getClient_nick(), channels[message.getTarget()].getName());
			send(client.getClient_fd(), m.c_str(), m.length(),0);
			return ;
		}
		if (message.getMsg().empty() && channels[message.getTarget()].getTopic().empty())
		{
			std::string m = RPL_NOTOPIC(client.get_hostname(), client.getClient_nick(), channels[message.getTarget()].getName());
			send(client.getClient_fd(), m.c_str(), m.length(), 0);
			return ;
		}
		if (!channels[message.getTarget()].getTopic().empty() && message.getMsg().empty())
		{
			std::string m = RPL_TOPIC(client.get_hostname(), client.getClient_nick(), channels[message.getTarget()].getName(), channels[message.getTarget()].getTopic());
			send(client.getClient_fd(), m.c_str(), m.length(),0);
			return ;
		}
		if (channels[message.getTarget()].getClientMember(client.getClient_nick()).getisOp() || !channels[message.getTarget()].getTopicRES())
		{
			channels[message.getTarget()].setTime(std::time(NULL));
			std::stringstream t;
			t << channels[message.getTarget()].getTime();
			if (message.getMsg().at(0) == ':'){
				channels[message.getTarget()].setTopic(message.getMsg().substr(1));
			}
			else {
				channels[message.getTarget()].setTopic(message.getMsg());
			}
			channels[message.getTarget()].setTopicSetter(client.getClient_nick());
			std::string m = RPL_TOPIC(client.get_hostname(), client.getClient_nick(), channels[message.getTarget()].getName(), channels[message.getTarget()].getTopic());
			send(client.getClient_fd(), m.c_str(), m.length(),0);
			broadcastToChan(client, channels[message.getTarget()], "", t.str(), 1);
			return ;
		}
		else
		{
			//channel topic restercted
			std::string m = ERR_CHANOPRIVSNEEDED(client.get_hostname(), client.getClient_nick(), channels[message.getTarget()].getName());
			send(client.getClient_fd(), m.c_str(), m.length(),0);
			return ;
		}
	}
}

void Server::handlingINV(Message message, std::map<std::string, Channel> &channels, Client &client)
{
	if (message.getCommand() == "INVITE")
	{
		if (message.getTarget().empty() || message.getMsg().empty())
		{
			std::string m = ERR_NEEDMOREPARAMS(client.get_hostname(), client.getClient_nick(), "INVITE");
			send(client.getClient_fd(), m.c_str(), m.length(),0);
			return ;
		}
		if (channels.find(message.getMsg()) == channels.end())
		{
			std::string m = ERR_NOSUCHCHANNEL(client.get_hostname(), client.getClient_nick(), message.getMsg());
			send(client.getClient_fd(), m.c_str(), m.length(),0);
			std::cout << client.getClient_fd() << std::endl;
			return ;
		}
		if (client_exist(message.getTarget()).getClient_nick().empty())
		{
			std::string m = ERR_NOSUCHNICK(client.get_hostname(), client.getClient_nick(), message.getTarget());
			send(client.getClient_fd(), m.c_str(), m.length(),0);
			return ;
		}
		if (is_member(message.getTarget(),channels[message.getMsg()].getName()))
		{
			std::string m = ERR_USERONCHANNEL(client.getClient_nick(), message.getTarget(), message.getMsg());
			send(client.getClient_fd(), m.c_str(), m.length(),0);
			return ;
		}
		else if (channels[message.getMsg()].getClientMember(client.getClient_nick()).getisOp())
		{
			
			std::vector<std::string> tmp = channels[message.getMsg()].getInvited();
			if (std::find(tmp.begin(), tmp.end(), message.getTarget()) == tmp.end())
				channels[message.getMsg()].getInvited().push_back(message.getTarget());

			std::string m = RPL_INVITING(client.getClient_ip(), client.getClient_nick(), message.getTarget(), message.getMsg());
			send(client.getClient_fd(), m.c_str(), m.length(),0);
			m.clear();

			m = RPL_INVITE(client.getClient_nick(), client.getClient_user(), client.getClient_ip(), message.getTarget(), message.getMsg());
			send(client_exist(message.getTarget()).getClient_fd(), m.c_str(), m.length(), 0);
		}
		else {
			std::string m = ERR_CHANOPRIVSNEEDED(client.get_hostname(), client.getClient_nick(), message.getMsg());
			send(client.getClient_fd(), m.c_str(), m.length(),0);
		}
	}
}

void    Server::handlingMODE(Message message, std::map<std::string, Channel> &channels, Client &client)
{

	if (message.getCommand() == "MODE")
	{
		if (message.getTarget().empty() || message.getMsg().empty())
		{
			std::string m = ERR_NEEDMOREPARAMS(client.get_hostname(), client.getClient_nick(), message.getCommand());
			send(client.getClient_fd(), m.c_str(), m.length(),0);
			return ;
		}
		if (channels.find(message.getTarget()) == channels.end())
		{
			std::string m = ERR_NOSUCHCHANNEL(client.get_hostname(), client.getClient_nick(), message.getTarget());
			send(client.getClient_fd(), m.c_str(), m.length(),0);
			return ;
		}
		else if (!channels[message.getTarget()].getClientMember(client.getClient_nick()).getisOp())
		{
			std::string m = ERR_CHANOPRIVSNEEDED(client.get_hostname(), client.getClient_nick(), message.getTarget());
			send(client.getClient_fd(), m.c_str(), m.length(),0);
		}
		else {
			if (message.getMsg().length() <= 2 && (message.getMsg().at(0) == '+' || message.getMsg().at(0) == '-'))
			{
				if (message.getMsg() == "+t")
				{
					channels[message.getTarget()].setTopicRES(true);

					std::string m = RPL_CHANNELMODEIS(client.getClient_nick(), client.getClient_user(), client.getClient_ip(), message.getTarget(), "+t", message.getComm());
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
					if (message.getComm().empty())
					{
						std::string m = RPL_CHANNELMODEIS(client.getClient_nick(), client.getClient_user(), client.getClient_ip(), message.getTarget(), "+k", channels[message.getTarget()].getkey());
						send(client.getClient_fd(), m.c_str(), m.length(),0);
						return ;
					}
					channels[message.getTarget()].setKeyRES(true);
					channels[message.getTarget()].setKey(message.getComm());
					std::string m = RPL_CHANNELMODEIS(client.getClient_nick(), client.getClient_user(), client.getClient_ip(), message.getTarget(), "+k", message.getComm());
					send(client.getClient_fd(), m.c_str(), m.length(),0);
					return ;
				}
				else if (message.getMsg() == "-k"){
					channels[message.getTarget()].setKeyRES(false);
					channels[message.getTarget()].setKey("");
					std::string m = RPL_CHANNELMODEIS(client.getClient_nick(), client.getClient_user(), client.getClient_ip(), message.getTarget(), "-k", "");
					send(client.getClient_fd(), m.c_str(), m.length(),0);
					return ;
				}
				else if (message.getMsg() == "+l"){
					if (message.getComm().empty())
					{
						std::string m = ERR_NEEDMOREPARAMS(client.get_hostname(), client.getClient_nick(), message.getCommand());
						send(client.getClient_fd(), m.c_str(), m.length(),0);
						return ;
					}
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
					if (message.getComm().empty())
					{
						std::string m = ERR_NEEDMOREPARAMS(client.get_hostname(), client.getClient_nick(), message.getCommand());
						send(client.getClient_fd(), m.c_str(), m.length(),0);
						return ;
					}
					if (!isMember(channels[message.getTarget()], message.getComm()))
					{
						std::string m = ERR_NOSUCHNICK(client.get_hostname(), client.getClient_nick(), message.getComm());
						send(client.getClient_fd(), m.c_str(), m.length(),0);
						return ;
					}
					else
					{
						if (channels[message.getTarget()].getClientMember(message.getComm()).getisOp())
							return ;
						channels[message.getTarget()].getClientMember(message.getComm()).setisOp(true);
						std::string m = RPL_CHANNELMODEIS(client.getClient_nick(), client.getClient_user(), client.getClient_ip(), message.getTarget(), "+o", "");
						send(client.getClient_fd(), m.c_str(), m.length(),0);
					}
				}
				else if (message.getMsg() == "-o")
				{
					if (message.getComm().empty())
					{
						std::string m = ERR_NEEDMOREPARAMS(client.get_hostname(), client.getClient_nick(), message.getCommand());
						send(client.getClient_fd(), m.c_str(), m.length(),0);
						return ;
					}
					if (!is_member(message.getComm(), message.getTarget()))
					{
						std::string m = ERR_NOSUCHNICK(client.get_hostname(), client.getClient_nick(), message.getComm());
						send(client.getClient_fd(), m.c_str(), m.length(),0);
						return ;
					}
					else
					{	
						if (!channels[message.getTarget()].getClientMember(message.getComm()).getisOp())
							return ;
						channels[message.getTarget()].getClientMember(message.getComm()).setisOp(false);
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

void Server::parsingMsg(std::string msg, Client &client)
{
	Message message;
	std::string str = std::string(msg);
	std::stringstream ss(str);
	std::stringstream s1(str);
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
	else if (cmdUpper == "JOIN"){
		message.setCommand("JOIN");
	}
	else if (cmdUpper == "KICK"){
		message.setCommand("KICK");
	}
	else if (cmdUpper == "PRIVMSG"){
		message.setCommand("PRIVMSG");
	}
	else if (cmdUpper == "NICK" || cmdUpper == "PASS" || cmdUpper == "USER" || cmdUpper == "PONG")
	{
		return;
	}
	else
	{
		str = ERR_UNKNOWNCOMMAND(client.get_hostname(), client.getClient_nick(), cmdUpper);
		send(client.getClient_fd(), str.c_str(), str.size(), 0);
		return;
	}
	sample.clear();
	s1 >> str;
	str.clear();
	std::getline(s1, str);
	ss >> sample;
	if (sample.find(':') != std::string::npos)
	{
		message.setTarget(comm_gen(str));
		join(message, client);
		kick(message, client);
		privmsg(message, client);
		handlingINV(message, channels, client);
		handlingTOPIC(message, channels, client);
		handlingMODE(message, channels, client);
		return;
	}
	message.setTarget(sample);
	sample.clear();
	ss >> sample;
	if (sample.find(':') != std::string::npos)
	{
		size_t l = str.find(':');
		message.setMsg(comm_gen(std::string (str , l)));
		join(message, client);
		kick(message, client);
		privmsg(message, client);
		handlingINV(message, channels, client);
		handlingTOPIC(message, channels, client);
		handlingMODE(message, channels, client);
		return;
	}
	message.setMsg(sample);
	sample.clear();
	ss >> sample;
	if (sample.find(':') != std::string::npos)
	{
		size_t l = str.find(':');
		message.setComm(comm_gen(std::string (str , l)));
		join(message, client);
		kick(message, client);
		privmsg(message, client);
		handlingINV(message, channels, client);
		handlingTOPIC(message, channels, client);
		handlingMODE(message, channels, client);
		return;
	}
	message.setComm(sample);
	join(message, client);
	kick(message, client);
	privmsg(message, client);
	handlingINV(message, channels, client);
	handlingTOPIC(message, channels, client);
	handlingMODE(message, channels, client);
}
