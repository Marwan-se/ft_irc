/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrrhaibi <yrrhaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 17:54:50 by yrrhaibi          #+#    #+#             */
/*   Updated: 2024/09/09 21:28:31 by yrrhaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"
#include <cstddef>
#include <map>
#include <string>
#include <iterator>
#include <sstream>
#include <sys/socket.h>
#include <vector>
#include "ChannelFile.hpp"
#include "../server.hpp"

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
		if (members[l].isOp && l == 0)
			tmp = '@' + members[l].getClient_nick();
		else if (members[l].isOp)
			tmp = " @" + members[l].getClient_nick();
		else
			tmp = " " + members[l].getClient_nick();
		list.append(tmp);
		tmp.clear();
	}
	return list;
}

void remove_from_ch(Client &client, std::map<std::string, Channel> &ch)
{
	std::string rpl;

	for (std::map<std::string, Channel>::iterator it = ch.begin(); it != ch.end(); it++) 
	{
		std::vector<Client> members = it->second.getMembers();
		for (size_t l = 0; l < members.size(); l++) 
		{
			if (members[l].getClient_nick() == client.getClient_nick())
			{
				members.erase(members.begin() + l);
				rpl = ":" + client.getClient_nick() + "!~" + client.getClient_user() + "@" + client.getClient_ip() + " " + "PART" + " " + it->second.getName() + "\r\n";
				send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);
			}
		}
	}
}

void Server::join(Message &comm , Client &client)
{
	if (comm.getCommand() != "JOIN")
		return;
	std::string rpl;
	std::map<std::string ,Channel>::iterator it;
	std::vector<std::string> ch_name;
	std::vector<std::string> ch_key;
	std::string tmp;
	std::stringstream ss;

	if (comm.getTarget().empty())
	{
		rpl = ERR_NEEDMOREPARAMS(client.gethostname(), client.getClient_nick(), "JOIN");
		send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);
	}
	if (comm.getTarget() == "0")
	{
		remove_from_ch(client, this->channels);
		return;
	}
	ss << comm.getTarget();
	while (std::getline(ss, tmp, ',')) 
		ch_name.push_back(tmp);
	ss.clear();
	ss << comm.getMsg();
	while (std::getline(ss, tmp, ',')) 
		ch_key.push_back(tmp);
	ss.clear();
	for (size_t l = 0; l < ch_name.size(); l++) 
	{
		std::string tmpn(ch_name.at(l));
		if (ch_name.at(l).at(0) == '#')
		{
			it = this->channels.find(tmpn);
				if (it == this->channels.end())
				{
					Channel t_ch(tmpn);
					client.isOp = true;
					t_ch.addMember(client);
					this->channels.insert(std::pair<std::string, Channel>(tmpn,t_ch));
					rpl = ":" + client.getClient_nick() + "!~" + client.getClient_user() + "@" + client.getClient_ip() + " " + "JOIN" + " :" + tmpn + "\r\n";
					send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);
					rpl = RPL_NAMREPLY(client.gethostname(), client.getClient_nick(), tmpn, join_members(channels[tmpn].getMembers()));
					send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);
					rpl = RPL_ENDOFNAMES(client.gethostname(), client.getClient_nick(), tmpn);
					send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);
				}
				else 
				{
					if (check_membre(it->second.getMembers(), client.getClient_nick()))
						continue;
					if (is_invite(it->second.getInvited(), client.getClient_nick()))
					{
						it->second.addMember(client);
						if (it->second.t)
						{
							rpl = RPL_TOPIC(client.gethostname(), client.getClient_nick(), tmpn, it->second.getTopic());
							send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);
							// rpl = RPL_TOPICWHOTIME(client.gethostname(), client.getClient_nick(), tmpn, )
							send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);
						}
						rpl = ":" + client.getClient_nick() + "!~" + client.getClient_user() + "@" + client.getClient_ip() + " " + "JOIN" + " :" + tmpn + "\r\n";
						send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);
						rpl = RPL_NAMREPLY(client.gethostname(), client.getClient_nick(), tmpn, join_members(channels[tmpn].getMembers()));
						send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);
						rpl = RPL_ENDOFNAMES(client.gethostname(), client.getClient_nick(), tmpn);
						send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);
					}
					else if (it->second.l == true && it->second.getMembers().size() >= (size_t)it->second.getlim())
					{
						rpl = ERR_CHANNELISFULL(client.gethostname(), client.getClient_nick(), tmpn);
						send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);
					}
					else if (it->second.k == false)
					{
						it->second.addMember(client);
						if (it->second.t)
						{
							rpl = RPL_TOPIC(client.gethostname(), client.getClient_nick(), tmpn, it->second.getTopic());
							send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);
							// rpl = RPL_TOPICWHOTIME(client.gethostname(), client.getClient_nick(), tmpn, )
							send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);
						}
						rpl = ":" + client.getClient_nick() + "!~" + client.getClient_user() + "@" + client.getClient_ip() + " " + "JOIN" + " :" + tmpn + "\r\n";
						send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);
						rpl = RPL_NAMREPLY(client.gethostname(), client.getClient_nick(), tmpn, join_members(channels[tmpn].getMembers()));
						send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);
						rpl = RPL_ENDOFNAMES(client.gethostname(), client.getClient_nick(), tmpn);
						send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);

					}
					else if (ch_key.empty() || l > ch_key.size() || ch_key[l] != it->second.getkey())
					{
						rpl = ERR_BADCHANNELKEY(client.gethostname(), client.getClient_nick(), tmpn);
						send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);
					}
					else if (it->second.i == true)
					{
						rpl = ERR_INVITEONLYCHAN(client.gethostname(), client.getClient_nick(), tmpn);
						send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);
					}
					else
					{
						it->second.addMember(client);
						if (it->second.t)
						{
							rpl = RPL_TOPIC(client.gethostname(), client.getClient_nick(), tmpn, it->second.getTopic());
							send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);
							// rpl = RPL_TOPICWHOTIME(client.gethostname(), client.getClient_nick(), tmpn, )
							send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);
						}
						rpl = ":" + client.getClient_nick() + "!~" + client.getClient_user() + "@" + client.getClient_ip() + " " + "JOIN" + " :" + tmpn + "\r\n";
						send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);
						rpl = RPL_NAMREPLY(client.gethostname(), client.getClient_nick(), tmpn, join_members(channels[tmpn].getMembers()));
						send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);
						rpl = RPL_ENDOFNAMES(client.gethostname(), client.getClient_nick(), tmpn);
						send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);
					}
				}
		}
		else 
		{
			rpl = ERR_NOSUCHCHANNEL(client.gethostname(), client.getClient_nick(), tmpn);
			send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);
		}
	}

}

void Server::remove_member(std::string nick, std::string ch_name)
{
	std::string rpl;
	std::map<std::string, Channel>::iterator it = this->channels.find(ch_name);
	std::vector<Client> members = it->second.getMembers();
	for (size_t l = 0; l < members.size(); l++) 
	{
		if (members[l].getClient_nick() == nick)
			members.erase(members.begin() + l);
	}
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
		if (members[l].getClient_nick() == nick && members[l].isOp == true)
			return true;
	}
	return false;

}


void Server::kick(Message &comm , Client &client)
{
	if (comm.getCommand() != "KICK")
		return;
	std::string rpl;
	std::map<std::string ,Channel>::iterator it;
	std::vector<std::string> ch_name;
	std::vector<std::string> client_name;
	std::string tmp;
	std::stringstream ss;


	ss << comm.getTarget();
	while (std::getline(ss, tmp, ',')) 
		ch_name.push_back(tmp);
	ss.clear();
	ss << comm.getMsg();
	while (std::getline(ss, tmp, ',')) 
		client_name.push_back(tmp);
	if (comm.getTarget().empty() || comm.getMsg().empty() || (client_name.size() != ch_name.size()))
	{
		rpl = ERR_NEEDMOREPARAMS(client.gethostname(), client.getClient_nick(), "KICK");
		send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);
	}
	ss.clear();
	for (size_t l = 0; l < ch_name.size(); l++)
	{
		if (this->channels.find(ch_name[l]) == this->channels.end())
		{
			rpl = ERR_NOSUCHCHANNEL(client.gethostname(), client.getClient_nick(), ch_name[l]);
			send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);
		}
		if (client_exist(client_name[l]).getClient_nick().empty())
		{
			rpl = ERR_NOSUCHNICK(client.gethostname(), client.getClient_nick(), client_name[l]);
			send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);
		}
		if (!is_member(client.getClient_nick(), ch_name[l]))
		{
			rpl = ERR_NOTONCHANNEL(client.gethostname(), client.getClient_nick(), ch_name[l]);
			send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);	
		}
		if (!is_op(client_name[l], ch_name[l]))
		{
			rpl = ERR_CHANOPRIVSNEEDED(client.gethostname(), client.getClient_nick(), ch_name[l]);
			send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);	
		}
		if (!is_member(client_name[l], ch_name[l]))
		{
			rpl = ERR_USERNOTINCHANNEL(client.gethostname(), client.getClient_nick(), client_name[l], ch_name[l]);
			send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);	
		}
		else 
		{
			Client kicked = client_exist(client_name[l]);
			remove_member(client_name[l], ch_name[l]);
			rpl = ":" + client.getClient_nick() + "!~" + client.getClient_user() + "@" + client.getClient_ip() + " " + "KICK" + " " + it->second.getName() + " " + client_name[l] + " " + client.getClient_nick() + "\r\n";
			send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);
			send(kicked.getClient_fd(), rpl.c_str(), rpl.size(), 0);
		}
	}
}
