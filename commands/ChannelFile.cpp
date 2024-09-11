/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelFile.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrrhaibi <yrrhaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 10:41:37 by msaidi            #+#    #+#             */
/*   Updated: 2024/09/11 11:10:50 by yrrhaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ChannelFile.hpp"
#include <stdexcept>


Channel::Channel()
{
    this->l = false;
    this->k = false;
    this->i = false;
    this->t = false;
}

Channel::Channel(std::string n)
{
    this->l = false;
    this->k = false;
    this->i = false;
    this->t = false;
    this->name = n;
}

int Channel::getlim()
{
    return this->lim;
}

std::string Channel::getkey()
{
    return this->key;
}

void Channel::setkey(std::string p)
{
    this->key = p;
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

void Channel::setlim(int l)
{
    this->lim = l;
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
    (void)member;
    for (std::vector<Client>::iterator it = this->members.begin(); it != this->members.end(); it++)
    {
        if (it->getClient_nick() == member.getClient_nick())
        {
            this->members.erase(it);
            return;
        }
    }
}

Channel::~Channel()
{
}
