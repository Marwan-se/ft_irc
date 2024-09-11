/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPL.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrrhaibi <yrrhaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 21:56:27 by yrrhaibi          #+#    #+#             */
/*   Updated: 2024/09/11 19:59:26 by yrrhaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//------REPLIES---------//

//********TOPIC*********//

# define RPL_TOPIC(hostname,client,channel,topic) (":" + hostname + " 332 " + client + " " + channel + " " + topic + "\r\n")
# define RPL_TOPICWHOTIME(hostname,client,channel,nick,time) (":" + hostname + " 333 " + client + " " + channel + " " + nick + time + "\r\n")
# define RPL_NOTOPIC(client, channel) (":irc.1337.ma 331 " + client + channel + " :No topic is set\r\n")


//********JOIN*********//

# define RPL_NAMREPLY(hostname,client,channel,members) (":" + hostname + " 353 " + client + " = " + channel + " :" + members + "\r\n")
# define RPL_ENDOFNAMES(hostname,client,channel) (":" + hostname + " 366 " + client + " " + channel + " :End of /NAMES list\r\n")

//********INVITE*********//
# define RPL_INVITING(client, inviting, invited, channel) (":" + client + " 341 " + inviting + " " + invited + " " + channel + "\r\n")
# define RPL_INVITE(client, user, ipadd, invited, channel) (":" + client + "!~" + user + "@" + ipadd + " INVITE " + invited + " " + channel + "\r\n") 

//------ERRORS---------//

# define ERR_NEEDMOREPARAMS(hostname,client,command) (":" + hostname + " 461 " + client + " " + command + " :Not enough parameters\r\n")
# define ERR_UNKNOWNCOMMAND(hostname,client,command) (":" + hostname + " 421 " + client + " " + command + " :Unknown command\r\n")

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

//********INVITE*********//

# define ERR_USERONCHANNEL(client, nick, channel) (":irc.1337.ma 443 " + client + " " + nick + channel + " :Is already on channel\r\n")

//********MODE*********//

#define ERR_UNKNOWNMODE(client, modechar) (":irc.1337.ma 472 " + client +  modechar + " :is unknown mode char to me\r\n")
#define RPL_CHANNELMODEIS(client, user, ipadd, channel, mod, param) (":" + client + "!~" + user + "@" + ipadd + " MODE " + channel + " " + mod + " :" + param + "\r\n") 



