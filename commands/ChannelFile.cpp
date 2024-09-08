/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelFile.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaidi <msaidi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 10:41:37 by msaidi            #+#    #+#             */
/*   Updated: 2024/09/08 19:32:32 by msaidi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ChannelFile.hpp"
#include <stdexcept>
#include "../server.hpp"

Channel::Channel()
{

}
Channel::Channel(std::string n)
{
    this->name = n;
}

std::string Channel::getName()
{
    return this->name;
}

std::string Channel::getTopic()
{
    return this->topic;
}

std::vector<Client> &Channel::getMembers()
{
    return this->members;
}
std::vector<std::string> &Channel::getInvited()
{
    return this->invited;
}

void Channel::setName(std::string name)
{
    this->name = name;
}

void Channel::setTopic(std::string topic)
{
    this->topic = topic;
}

void Channel::addMember(Client &member)
{
    this->members.push_back(member);
}


void Channel::removeMember(Client &member)
{
    for (std::vector<Client>::iterator it = this->members.begin(); it != this->members.end(); it++)
    {
        if (it->getClient_nick() == member.getClient_nick())
        {
            this->members.erase(it);
            return;
        }
    }
}

void Channel::addInvited(std::string Invited)
{
    this->invited.push_back(Invited);
}


void Channel::removeInvited(std::string Invited)
{
    for (std::vector<std::string>::iterator it = this->invited.begin(); it != this->invited.end(); it++)
    {
        if (*it == Invited)
        {
            this->invited.erase(it);
            return;
        }
    }
}



Channel::~Channel()
{
}
