/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelFile.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaidi <msaidi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 10:41:37 by msaidi            #+#    #+#             */
/*   Updated: 2024/08/17 17:38:41 by msaidi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ChannelFile.hpp"
#include <stdexcept>


Channel::Channel(std::string name, std::string topic)
{
    if (name == "")
        throw std::invalid_argument("Error: Channel name cannot be empty");
    if (name[0] != '#')
        throw std::invalid_argument("Error: Channel name must start with #");
    this->name = name;
    if ( topic[0] == '#')
        throw std::invalid_argument("Error: Channel topic cannot start with #");
    this->topic = topic;
}

std::string Channel::getName()
{
    return this->name;
}

std::string Channel::getTopic()
{
    return this->topic;
}

std::vector<std::string> Channel::getMembers()
{
    return this->members;
}

std::vector<std::string> Channel::getOps()
{
    return this->ops;
}

void Channel::setName(std::string name)
{
    this->name = name;
}

void Channel::setTopic(std::string topic)
{
    this->topic = topic;
}

void Channel::addMember(std::string member)
{
    this->members.push_back(member);
}

void Channel::addOp(std::string op)
{
    this->ops.push_back(op);
}

void Channel::removeMember(std::string member)
{
    for (std::vector<std::string>::iterator it = this->members.begin(); it != this->members.end(); it++)
    {
        if (*it == member)
        {
            this->members.erase(it);
            return;
        }
    }
}

void Channel::removeOp(std::string op)
{
    for (std::vector<std::string>::iterator it = this->ops.begin(); it != this->ops.end(); it++)
    {
        if (*it == op)
        {
            this->ops.erase(it);
            return;
        }
    }
}

Channel::~Channel()
{
}
