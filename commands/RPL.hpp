/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPL.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaidi <msaidi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 10:33:48 by msaidi            #+#    #+#             */
/*   Updated: 2024/09/08 19:09:21 by msaidi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// TOPIC
# define RPL_TOPIC(client, channel, topic) (":irc.1337.ma 332 " + client + " " + channel + " " + topic + "\r\n")
# define RPL_NOTOPIC(client, channel) (":irc.1337.ma 331 " + client + channel + " :No topic is set\r\n")
// # define RPL_TOPICWHOTIME(client, channel, nickname, time) (":irc.1337.ma 333 " + client + " " + channel + " " + nickname + " " + time + "\r\n")
# define ERR_NOTONCHANNEL(client, channel) (":irc.1337.ma 442 " + client + channel + " :not on this channel.\r\n")
# define ERR_NEEDMOREPARAMS(client, command) (":irc.1337.ma 461 " + client + " " + command + " :Not enough parameters.\r\n")

# define ERR_NOSUCHCHANNEL(client, channel) (":irc.1337.ma 403 " + client + channel + " :No such channel\r\n")
# define ERR_NOSUCHNICK(client, nickname) (":irc.1337.ma 401 " + client + nickname + " :No such nick\r\n")

# define ERR_CHANOPRIVSNEEDED(client, channel) (":irc.1337.ma 482 " + client +  channel + " :You're not channel operator\r\n")
# define RPL_TOPICWHOTIME(client, channel, nickname, time) (":irc.1337.ma 333 " + client + " " + channel + " " + nickname + " " + time + "\r\n")

# define ERR_USERONCHANNEL(client, nick, channel) (":irc.1337.ma 443 " + client + " " + nick + channel + " :Is already on channel\r\n")

# define RPL_INVITING(client, inviting, invited, channel) (":" + client + " 341 " + inviting + " " + invited + " " + channel + "\r\n")
#define RPL_INVITE(client, user, ipadd, invited, channel) (":" + client + "!~" + user + "@" + ipadd + " INVITE " + invited + " " + channel + "\r\n")