/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msekhsou <msekhsou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 12:10:29 by msekhsou          #+#    #+#             */
/*   Updated: 2024/09/11 05:14:54 by msekhsou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLIES_HPP
# define REPLIES_HPP


/// AUTH 
#define ERR_ALREADYREGISTERED(nickname) (" : 462 " + nickname + " :You may not reregister !" + "\r\n")
#define ERR_NEEDMOREPARAMS(nickname) (" : 461 " + nickname + " :Not enough parameters." + "\r\n")
#define ERR_PASSWDMISMATCH(nickname) (" : 464 " + nickname + " :Password incorrect !" + "\r\n")
#define ERR_NONICKNAMEGIVEN(nickname) (": 431 " + nickname + " :No nickname given" + "\r\n")
#define ERR_ERRONEUSNICKNAME(nickname) (": 432 " + nickname + " :Erroneus nickname" + "\r\n")
#define ERR_NICKNAMEINUSE(nickname) (": 433 " + nickname + " :Nickname is already in use" + "\r\n")
#define ERR_NOTREGISTERED(nickname) (": 451 " + nickname + " :You have not registered!" + "\r\n")


#endif
