/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelFile.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaidi <msaidi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 10:41:37 by msaidi            #+#    #+#             */
/*   Updated: 2024/09/14 11:55:41 by msaidi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ChannelFile.hpp"
#include <cstddef>


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

size_t Channel::getLimNum()
{
    return this->lim;
}

std::string Channel::getkey()
{
    return this->key;
}

void Channel::setKey(std::string p)
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

void Channel::setLimNum(size_t l)
{
    this->lim = l;
}


void Channel::setLimit(bool l)
{
    this->l = l;
}

bool Channel::getTopicRES()
{
    return t;
}
bool Channel::getKeyRES()
{
    return k;
}
bool Channel::getInviteOnly()
{
    return i;
}
bool Channel::getLimit()
{
    return l;
}

void Channel::setTopicRES(bool x)
{
    this->t = x;
}
void Channel::setInviteOnly(bool x)
{
    this->i = x;
}
void Channel::setKeyRES(bool x)
{
    this->k = x;
}

std::time_t Channel::getTime()
{
    return this->topicTime;
}

void Channel::setTime(time_t x)
{
    this->topicTime = x;
}

void Channel::setTopic(std::string topic)
{
    this->topic = topic;
}

void Channel::addMember(Client &member)
{
    this->members.push_back(member);
}

std::string Channel::getTopicSetter()
{
    return topicSetter;
}

void Channel::setTopicSetter(std::string s)
{
    this->topicSetter = s;
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

Client& Channel::getClientMember(std::string nickname)
{
    for (std::vector<Client>::iterator it = this->members.begin();it != this->members.end(); it++)
    {
        if (it->getClient_nick() == nickname)
            return *it;
    }
    return *this->members.end();
}