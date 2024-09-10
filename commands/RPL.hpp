/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPL.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrrhaibi <yrrhaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 21:56:27 by yrrhaibi          #+#    #+#             */
/*   Updated: 2024/09/10 20:52:08 by yrrhaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//------REPLIES---------//
//********JOIN*********//
# define RPL_TOPIC(hostname,client,channel,topic) (":" + hostname + " 332 " + client + " " + channel + " " + topic + "\r\n")
# define RPL_TOPICWHOTIME(hostname,client,channel,nick,time) (":" + hostname + " 333 " + client + " " + channel + " " + nick + time + "\r\n")
# define RPL_NAMREPLY(hostname,client,channel,members) (":" + hostname + " 353 " + client + " = " + channel + " :" + members + "\r\n")
# define RPL_ENDOFNAMES(hostname,client,channel) (":" + hostname + " 366 " + client + " " + channel + " :End of /NAMES list\r\n")

//------ERRORS---------//

# define ERR_NEEDMOREPARAMS(hostname,client,command) (":" + hostname + " 461 " + client + " " + command + " :Not enough parameters\r\n")

//********JOIN*********//

# define ERR_NOSUCHCHANNEL(hostname,client,channel) (":" + hostname + " 403 " + client + " " + channel + " :No such channel\r\n")
# define ERR_CHANNELISFULL(hostname,client,channel) (":" + hostname + " 471 " + client + " " + channel + " :Cannot join channel (+l)\r\n")
# define ERR_BADCHANNELKEY(hostname,client,channel) (":" + hostname + " 475 " + client + " " + channel + " :Cannot join channel (+k)\r\n")
# define ERR_INVITEONLYCHAN(hostname,client,channel) (":" + hostname + " 473 " + client + " " + channel + " :Cannot join channel (+i)\r\n")

//********KICK*********//

# define ERR_NOSUCHNICK(hostname,client,nick) (":" + hostname + " 401 " + client + " " + nick + " :No such nick\r\n")
# define ERR_NOTONCHANNEL(hostname,client,channel) (":" + hostname + " 442 " + client + " " + channel + " :You're not on that channel\r\n")
# define ERR_CHANOPRIVSNEEDED(hostname,client,channel) (":" + hostname + " 482 " + client + " " + channel + " :You're not channel operator\r\n")
# define ERR_USERNOTINCHANNEL(hostname,client,nick,channel) (":" + hostname + " 441 " + client + " " + nick+ " " + channel + " :They aren't on that channel\r\n")

//********PRIVMSG*********//

# define ERR_CANNOTSENDTOCHAN(hostname,client,channel) (":" + hostname + " 404 " + client + " " + channel + " :Cannot send to channel\r\n")