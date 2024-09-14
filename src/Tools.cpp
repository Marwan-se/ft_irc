/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tools.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaidi <msaidi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 16:49:06 by yrrhaibi          #+#    #+#             */
/*   Updated: 2024/09/14 11:58:04 by msaidi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Tools.hpp"
#include <unistd.h>

void Server::remove_member(std::string nick, std::string ch_name)
{
	std::string rpl;
	std::map<std::string, Channel>::iterator it = this->channels.find(ch_name);
	for (size_t l = 0; l < it->second.getMembers().size(); l++) 
	{
		if (it->second.getMembers()[l].getClient_nick() == nick)
		{
			if (is_op(nick, ch_name))
				it->second.getMembers()[l].setisOp(false);
			it->second.getMembers().erase(it->second.getMembers().begin() + l);
		}
	}
	if (it->second.getMembers().empty())
		this->channels.erase(it);
}

Client Server::client_exist(std::string name)
{
	for (std::map<int, Client>::iterator it = this->client_info.begin(); it != this->client_info.end(); it++) 
	{
		if (it->second.getClient_nick() == name)
			return it->second;
	}
	return Client();
}

bool Server::is_member(std::string nick, std::string ch_name)
{
	std::map<std::string, Channel>::iterator it = this->channels.find(ch_name);
	std::vector<Client> members = it->second.getMembers();
	for (size_t l = 0; l < members.size(); l++) 
	{
		if (members[l].getClient_nick() == nick)
			return true;
	}
	return false;
}



bool Server::is_op(std::string nick, std::string ch_name)
{
	std::map<std::string, Channel>::iterator it = this->channels.find(ch_name);
	std::vector<Client> members = it->second.getMembers();
	for (size_t l = 0; l < members.size(); l++) 
	{
		if (members[l].getClient_nick() == nick && members[l].getisOp() == true)
			return true;
	}
	return false;

}

std::string comm_gen(std::string comm)
{
	std::string r(comm);
	if (r[0] == ' ')
		r.erase(0, 1);
	if (r[0] != ':' && r.find(' ') != std::string::npos)
		r.erase(r.find(' '));
	return r;
}

bool check_membre(std::vector<Client> vec, std::string name)
{

	for (size_t l = 0; l < vec.size(); l++) {
		if (vec[l].getClient_nick() == name)
			return true;
	}
	return false;
}

bool is_invite(std::vector<std::string> &vec, std::string name)
{

	for (size_t l = 0; l < vec.size(); l++)
	{
		if (vec[l] == name)
		{
			vec.erase(vec.begin() + l);
			return true;
		}
	}
	return false;
}

std::string join_members(std::vector<Client> members)
{
	std::string list;
	std::string tmp;
	for (size_t l = 0; l < members.size(); l++)
	{
		if (members[l].getisOp() && l == 0)
			tmp = '@' + members[l].getClient_nick();
		else if (members[l].getisOp())
			tmp = " @" + members[l].getClient_nick();
		else
			tmp = " " + members[l].getClient_nick();
		list.append(tmp);
		tmp.clear();
	}
	return list;
}

void Server::remove_from_ch(Client &client, std::map<std::string, Channel> &ch, bool flag)
{
	std::string rpl;

	for (std::map<std::string, Channel>::iterator it = ch.begin(); it != ch.end();) 
	{
		for (size_t l = 0; l < it->second.getMembers().size(); l++) 
		{
			if (it->second.getMembers()[l].getClient_nick() == client.getClient_nick())
			{
				if (is_op(it->second.getMembers()[l].getClient_nick(), it->second.getName()))
                    it->second.getMembers()[l].setisOp(false);
				it->second.getMembers().erase(it->second.getMembers().begin() + l);
				if (flag == 0)
				{
					rpl = ":" + client.getClient_nick() + "!~" + client.getClient_user() + "@" + client.getClient_ip() + " " + "PART" + " " + it->second.getName() + "\r\n";
					send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);
				}
			}
		}
		if (it->second.getMembers().empty())
		{
			std::map<std::string, Channel>::iterator tmp = it;
			it++;
		    ch.erase(tmp->second.getName());
		}
		else 
			it++;
		
	}
}

void Server::msg_chann(Client client, std::string msg, std::string ch_name,std::string target, std::string command)
{
	std::string rpl;
	std::vector<Client> members = channels[ch_name].getMembers();

	for (size_t l = 0; l < members.size(); l++)
	{
		rpl = ":" + client.getClient_nick() + "!~" + client.getClient_user() + "@" + client.getClient_ip() + " " + command + " " + target + msg + "\r\n";
		if (client.getClient_nick() != members[l].getClient_nick())
			send(members[l].getClient_fd(), rpl.c_str(), rpl.size(), 0);
	}
}



void Server::broadcastToChan(Client &client, Channel &chann, std::string tar, std::string t, int i)
{
	std::string rp;
	std::string rp1;

	if (i == 1)
	{
		rp1 = RPL_TOPIC(client.get_hostname(), client.getClient_nick(), chann.getName(), chann.getTopic());
		rp = RPL_TOPICWHOTIME(client.get_hostname(), client.getClient_nick(), chann.getName(), client.getClient_nick(), t);
		for (std::vector<Client>::iterator it = chann.getMembers().begin(); it != chann.getMembers().end(); it++)
		{
			if (client.getClient_nick() == it->getClient_nick())
				continue;
			send(it->getClient_fd(), rp1.c_str(), rp1.length(), 0);
			send(it->getClient_fd(), rp.c_str(), rp.length(), 0);
		}
	}
	else
		rp = RPL_CHANNELMODEIS(client.getClient_nick(), client.getClient_user(), client.getClient_ip(), chann.getName(), t, tar);
}
