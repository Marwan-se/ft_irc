/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaidi <msaidi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 14:17:28 by msaidi            #+#    #+#             */
/*   Updated: 2024/09/08 15:47:14 by msaidi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
# define MESSAGE_HPP

#include <string>
#include "ChannelFile.hpp"
#include <map>


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

void parsingMsg(char *msg, std::map<std::string, Channel> &channels, Client &client);








#endif