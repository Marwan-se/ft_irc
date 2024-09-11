/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msekhsou <msekhsou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 12:10:29 by msekhsou          #+#    #+#             */
/*   Updated: 2024/09/11 20:23:44 by msekhsou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLIES_HPP
# define REPLIES_HPP


/// AUTH 
#define ERR_ALREADYREGISTERED(hostname, nickname, command) (":" + hostname + " 462 " + nickname + " " + command + " :You may not reregister !" + "\r\n")
#define ERR_NEEDMOREPARAMS(hostname, nickname, command) (":" + hostname + " 461 " + nickname + " " + command + " :Not enough parameters." + "\r\n")
#define ERR_PASSWDMISMATCH(hostname, nickname, command) (":" + hostname + " 464 " + nickname + " " + command + " :Password incorrect !" + "\r\n")
#define ERR_NONICKNAMEGIVEN(hostname, nickname, command) (":" + hostname + " 431 " + nickname + " " + command + " :No nickname given" + "\r\n")
#define ERR_ERRONEUSNICKNAME(hostname, nickname, command) (":" + hostname + " 432 " + nickname + " " + command +" :Erroneus nickname" + "\r\n")
#define ERR_NICKNAMEINUSE(hostname, nickname, command) (":" + hostname + " 433 " + nickname + " " + command + " :Nickname is already in use" + "\r\n")
#define ERR_NOTREGISTERED(hostname, nickname, command) (":" + hostname + " 451 " + nickname + " " + command + " :You have not registered!" + "\r\n")


#endif
