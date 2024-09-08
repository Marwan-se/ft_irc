/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPL.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaidi <msaidi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 10:33:48 by msaidi            #+#    #+#             */
/*   Updated: 2024/09/08 11:12:58 by msaidi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// TOPIC
# define RPL_TOPIC(client, channel, topic) (":irc.1337.ma 332 " + client + " " + channel + " " + topic + "\r\n")
# define RPL_NOTOPIC(client, channel) (":irc.1337.ma 331 " + client + channel + " :No topic is set\r\n")
// # define RPL_TOPICWHOTIME(client, channel, nickname, time) (":irc.1337.ma 333 " + client + " " + channel + " " + nickname + " " + time + "\r\n")
# define ERR_NOTONCHANNEL(client, channel) (":irc.1337.ma 442 " + client + channel + " :not on this channel.\r\n")