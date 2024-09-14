/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaidi <msaidi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 17:54:50 by yrrhaibi          #+#    #+#             */
/*   Updated: 2024/09/14 11:40:40 by msaidi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Message.hpp"
#include <cstddef>
#include <map>
#include <string>
#include <sstream>
#include <sys/socket.h>
#include <vector>
#include "../inc/ChannelFile.hpp"
#include "../inc/server.hpp"
#include "../inc/Tools.hpp"

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
		rpl = ERR_NEEDMOREPARAMS(client.get_hostname(), client.getClient_nick(), "JOIN");
		send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);
		return;
	}
	if (comm.getTarget() == "0")
	{
		remove_from_ch(client, this->channels, 0);
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
					t_ch.addMember(client);
					t_ch.getMembers()[0].setisOp(true);
					// this->channels.insert(std::pair<std::string, Channel>(tmpn,t_ch));
					channels[tmpn] = t_ch;
					rpl = ":" + client.getClient_nick() + "!~" + client.getClient_user() + "@" + client.getClient_ip() + " " + "JOIN " + tmpn + "\r\n";
					send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);
					rpl = RPL_NAMREPLY(client.get_hostname(), client.getClient_nick(), tmpn, join_members(channels[tmpn].getMembers()));
					send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);
					rpl = RPL_ENDOFNAMES(client.get_hostname(), client.getClient_nick(), tmpn);
					send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);
				}
				else 
				{
					if (check_membre(it->second.getMembers(), client.getClient_nick()))
						continue;
					if (is_invite(it->second.getInvited(), client.getClient_nick()))
					{
						it->second.addMember(client);
						if (it->second.getTopicRES())
						{
							rpl = RPL_TOPIC(client.get_hostname(), client.getClient_nick(), tmpn, it->second.getTopic());
							send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);
							// rpl = RPL_TOPICWHOTIME(client.get_hostname(), client.getClient_nick(), tmpn, )
							send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);
						}
						rpl = ":" + client.getClient_nick() + "!~" + client.getClient_user() + "@" + client.getClient_ip() + " " + "JOIN " + tmpn + "\r\n";
						send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);
						rpl = RPL_NAMREPLY(client.get_hostname(), client.getClient_nick(), tmpn, join_members(channels[tmpn].getMembers()));
						send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);
						rpl = RPL_ENDOFNAMES(client.get_hostname(), client.getClient_nick(), tmpn);
						send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);
					}
					else if (it->second.getLimit() == true && it->second.getMembers().size() >= it->second.getLimNum())
					{
						rpl = ERR_CHANNELISFULL(client.get_hostname(), client.getClient_nick(), tmpn);
						send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);
					}
					else if (it->second.getKeyRES() == false && it->second.getInviteOnly() == false)
					{
						it->second.addMember(client);
						if (it->second.getTopicRES())
						{
							rpl = RPL_TOPIC(client.get_hostname(), client.getClient_nick(), tmpn, it->second.getTopic());
							send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);
							// rpl = RPL_TOPICWHOTIME(client.get_hostname(), client.getClient_nick(), tmpn, )
							send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);
						}
						rpl = ":" + client.getClient_nick() + "!~" + client.getClient_user() + "@" + client.getClient_ip() + " " + "JOIN " + tmpn + "\r\n";
						send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);
						msg_chann(client, "", tmpn, tmpn, "JOIN");
						rpl = RPL_NAMREPLY(client.get_hostname(), client.getClient_nick(), tmpn, join_members(channels[tmpn].getMembers()));
						send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);
						rpl = RPL_ENDOFNAMES(client.get_hostname(), client.getClient_nick(), tmpn);
						send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);

					}
					else if (it->second.getKeyRES() == true && (ch_key.empty() || l > ch_key.size() || ch_key[l] != it->second.getkey()))
					{
						rpl = ERR_BADCHANNELKEY(client.get_hostname(), client.getClient_nick(), tmpn);
						send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);
					}
					else if (it->second.getInviteOnly() == true)
					{
						rpl = ERR_INVITEONLYCHAN(client.get_hostname(), client.getClient_nick(), tmpn);
						send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);
					}
					else
					{
						it->second.addMember(client);
						if (it->second.getTopicRES())
						{
							rpl = RPL_TOPIC(client.get_hostname(), client.getClient_nick(), tmpn, it->second.getTopic());
							send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);
							// rpl = RPL_TOPICWHOTIME(client.get_hostname(), client.getClient_nick(), tmpn, )
							send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);
						}
						rpl = ":" + client.getClient_nick() + "!~" + client.getClient_user() + "@" + client.getClient_ip() + " " + "JOIN " + tmpn + "\r\n";
						send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);
						msg_chann(client, "", tmpn,tmpn, "JOIN");
						rpl = RPL_NAMREPLY(client.get_hostname(), client.getClient_nick(), tmpn, join_members(channels[tmpn].getMembers()));
						send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);
						rpl = RPL_ENDOFNAMES(client.get_hostname(), client.getClient_nick(), tmpn);
						send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);
					}
				}
		}
		else 
		{
			rpl = ERR_NOSUCHCHANNEL(client.get_hostname(), client.getClient_nick(), tmpn);
			send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);
		}
	}

}


void Server::kick(Message &comm , Client &client)
{
	std::string rpl;
	if (comm.getCommand() != "KICK")
		return;
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
		rpl = ERR_NEEDMOREPARAMS(client.get_hostname(), client.getClient_nick(), "KICK");
		send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);
		return;
	}
	ss.clear();
	for (size_t l = 0; l < ch_name.size(); l++)
	{
		if (this->channels.find(ch_name[l]) == this->channels.end())
		{
			rpl = ERR_NOSUCHCHANNEL(client.get_hostname(), client.getClient_nick(), ch_name[l]);
			send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);
		}
		else if (client_exist(client_name[l]).getClient_nick().empty())
		{
			rpl = ERR_NOSUCHNICK(client.get_hostname(), client.getClient_nick(), client_name[l]);
			send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);
		}
		else if (!is_member(client.getClient_nick(), ch_name[l]))
		{
			rpl = ERR_NOTONCHANNEL(client.get_hostname(), client.getClient_nick(), ch_name[l]);
			send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);	
		}
		else if (!is_op(client.getClient_nick(), ch_name[l]))
		{
			rpl = ERR_CHANOPRIVSNEEDED(client.get_hostname(), client.getClient_nick(), ch_name[l]);
			send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);	
		}
		else if (!is_member(client_name[l], ch_name[l]))
		{
			rpl = ERR_USERNOTINCHANNEL(client.get_hostname(), client.getClient_nick(), client_name[l], ch_name[l]);
			send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);	
		}
		else 
		{
			Client kicked = client_exist(client_name[l]);
			remove_member(client_name[l], ch_name[l]);
			if (comm.getComm().empty())
			{
				rpl = ":" + client.getClient_nick() + "!~" + client.getClient_user() + "@" + client.getClient_ip() + " " + "KICK" + " " + ch_name[l] + " " + client_name[l] + " " + client.getClient_nick() + " :NO COMMENT GIVEN\r\n";
				msg_chann(client, " :NO COMMENT GIVEN", ch_name[l],ch_name[l], "KICK");
			}
			else
			{
				rpl = ":" + client.getClient_nick()  + "!~" + client.getClient_user() + "@" + client.getClient_ip() + " " + "KICK" + " " + ch_name[l] + " " + client_name[l] + " " + client.getClient_nick() + " " + comm_gen(comm.getComm()) + "\r\n";
				msg_chann(client, " " + comm_gen(comm.getComm()), ch_name[l],ch_name[l], "KICK");

			}
			send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);
			if (client.getClient_fd() != kicked.getClient_fd())
				send(kicked.getClient_fd(), rpl.c_str(), rpl.size(), 0);
			if (channels[ch_name[l]].getMembers().empty())
				channels.erase(ch_name[l]);
		}
	}
}

void Server::privmsg(Message &comm , Client &client)
{
	if (comm.getCommand() != "PRIVMSG")
		return;	
	std::string rpl;
	std::vector<std::string> msg;
	std::vector<std::string> client_name;
	std::string tmp;
	std::stringstream ss;
	std::map<std::string ,Channel>::iterator it;
	bool is_channel;

	if (comm.getTarget().empty())
	{
		rpl = ERR_NEEDMOREPARAMS(client.get_hostname(), client.getClient_nick(), "PRIVMSG");
		send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);
		return;
	}
	ss << comm.getTarget();
	while (std::getline(ss, tmp, ',')) 
		client_name.push_back(tmp);
	ss.clear();
	msg.push_back(comm_gen(comm.getMsg()));
	for (size_t l = 0; l < client_name.size(); l++)
	{
		if (client_name[l].find('#') != std::string::npos)
		{
			is_channel = true;
			it = channels.find(client_name[l]);
		}
		if (is_channel && it == this->channels.end())
		{
			rpl = ERR_NOSUCHCHANNEL(client.get_hostname(), client.getClient_nick(), client_name[l]);
			send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);
		}
		else if (!is_channel && client_exist(client_name[l]).getClient_nick().empty())
		{
			rpl = ERR_NOSUCHNICK(client.get_hostname(), client.getClient_nick(), client_name[l]);
			send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);
		}
		else if (is_channel && !is_member(client.getClient_nick(), client_name[l]))
		{                                                                                                                                                                                                                                     
			rpl = ERR_CANNOTSENDTOCHAN(client.get_hostname(), client.getClient_nick(), client_name[l]);
			send(client.getClient_fd(), rpl.c_str(), rpl.size(), 0);
		}
		else if (!is_channel)
		{
			rpl = ":" + client.getClient_nick() + "!~" + client.getClient_user() + "@" + client.getClient_ip() + " " + "PRIVMSG" + " " + client_name[l] + " " + msg[0] + "\r\n";
			send(client_exist(client_name[l]).getClient_fd(), rpl.c_str(), rpl.size(), 0);
		}
		else 
			msg_chann(client," " + msg[0], it->second.getName(),it->second.getName(), "PRIVMSG");
	}
	
}