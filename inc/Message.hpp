/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msekhsou <msekhsou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 14:17:28 by msaidi            #+#    #+#             */
/*   Updated: 2024/09/13 10:05:53 by msekhsou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once


#include <string>
#include "RPL.hpp"


class Message
{
    private:
        std::string command;
        std::string target;
        std::string msg;
        std::string comm;

    public:
        Message();
        ~Message();
        std::string getCommand();
        std::string getTarget();
        std::string getMsg();
        std::string getComm();
        void setMsg(std::string msg);
        void setComm(std::string comm);
        void setTarget(std::string target);
        void setCommand(std::string command);
};









