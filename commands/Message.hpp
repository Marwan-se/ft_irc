/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaidi <msaidi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 14:17:28 by msaidi            #+#    #+#             */
/*   Updated: 2024/09/11 18:31:54 by msaidi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
# define MESSAGE_HPP

#include <string>


class Channel;

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
        void setTarget(std::string target);
        void setCommand(std::string command);
        void setComm(std::string comm);
};








#endif