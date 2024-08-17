/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaidi <msaidi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 14:17:11 by msaidi            #+#    #+#             */
/*   Updated: 2024/08/17 18:27:32 by msaidi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"
#include <string>
#include <sstream>
#include <iostream>
#include "ChannelFile.hpp"

Message::Message()
{
    this->command = "";
    this->target = "";
    this->msg = "";
}

Message::~Message()
{
}

std::string Message::getCommand()
{
    return this->command;
}

std::string Message::getTarget()
{
    return this->target;
}

std::string Message::getMsg()
{
    return this->msg;
}

void Message::setMsg(std::string msg)
{
    this->msg = msg;
}

void Message::setTarget(std::string target)
{
    this->target = target;
}

void Message::setCommand(std::string command)
{
    this->command = command;
}


void handlingINV(Message *message, std::vector<Channel> *channels)
{
    bool found = false;
    if (message->getCommand() == "INVITE"){
        if (message->getTarget() == "" || message->getMsg() == ""){
            throw std::runtime_error("Error: empty target or message");
        }
        if (message->getMsg()[0] != '#'){
            throw std::runtime_error("Error: invalid channel name");
        }
        for (std::vector<Channel>::iterator it = channels->begin(); it != channels->end(); it++){
            if (it->getName() == message->getMsg()){
                it->addMember(message->getTarget());
                found = true;
                return ;
            }
        }
        if (!found){
            std::cout << "Channel not found" << std::endl;
        }
    }
}

void parsingMsg(char *msg, Message *message, std::vector<Channel> *channels)
{
    std::string str = std::string(msg);
    std::stringstream ss(str);
    if (ss.eof()){
        throw std::runtime_error("Error: empty message");
    }
    std::string sample;
    ss >> sample;
        if (sample == "INVITE"){
            message->setCommand("INVITE");
        }
        else if (sample == "TOPIC"){
            message->setCommand("TOPIC");
        }
        else if (sample == "MODE"){
            message->setCommand("MODE");
        }
        else{
            return ;
        }
    if (ss.eof()){
        throw std::runtime_error("Error: no target");
    }
    sample.clear();
    ss >> sample;
    message->setTarget(sample);
    sample.clear();
    ss >> sample;
    message->setMsg(sample);
    handlingINV(message, channels);
    // handlingTOPIC();
    // handlingMODE();
}
