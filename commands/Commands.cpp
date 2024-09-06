/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrrhaibi <yrrhaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 17:54:50 by yrrhaibi          #+#    #+#             */
/*   Updated: 2024/09/05 18:33:49 by yrrhaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"
#include <deque>
#include <iterator>
#include <string>
#include <sstream>
#include <iostream>
#include <sys/socket.h>
#include <vector>
#include "ChannelFile.hpp"
#include "../server.hpp"

int count(std::string str)
{
	int i = 0;
	std::string	tmp;
	std::stringstream ss(str);
	while (!ss.eof()) {
		ss >> tmp;
		i++;
	}
	return i;
}

void join(std::string str, Message *message,std::vector<Channel> &channels, std::vector<Client> &clients)
{
	std::string	tmp;
	std::stringstream ss(str);
	if (count(str) < 1)
	{
		//std::string err_msg = ":" + hostame + " 461 " + client_NICK + " JOIN " + ":Not enough parameters";
		//send(socket, err_msg, err_msg.length(), 0);
	}

	//existence of channel
	// for (init-statement; condition; inc-expression) {
	
	// }
}