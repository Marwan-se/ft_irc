/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrrhaibi <yrrhaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 14:17:28 by msaidi            #+#    #+#             */
/*   Updated: 2024/09/05 16:52:22 by yrrhaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
# define MESSAGE_HPP

#include <string>
#include "ChannelFile.hpp"
#include "../server.hpp"



class Message
{
    private:
        std::string command;
        std::string target;
        std::string msg;
    public:
        Message();
        ~Message();
        std::string getCommand();
        std::string getTarget();
        std::string getMsg();
        void setMsg(std::string msg);
        void setTarget(std::string target);
        void setCommand(std::string command);
};

void parsingMsg(char *msg, Message *message, std::vector<Channel> *channels);
void join(std::string str,Message *message, std::vector<Channel> &channels, std::vector<Client> &clients);








#endif