/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelFile.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrrhaibi <yrrhaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 10:34:01 by msaidi            #+#    #+#             */
/*   Updated: 2024/09/08 19:24:27 by yrrhaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <cstdlib>
#include <string>
#include <vector>
#include "../server.hpp"

class Client;
class Channel
{
    private:
        std::string name;
        std::string topic;
        std::vector<Client> members;
        std::vector<std::string> invited;
        int lim;
        std::string key;

    public:
        bool t;
        bool i;
        bool k;
        bool l;

        Channel();
        Channel(std::string name);
        std::string getName();
        std::string getTopic();
        std::vector<Client> &getMembers();
        std::vector<std::string> &getInvited();
        int getlim();
        std::string getkey();

        void setName(std::string name);
        void setTopic(std::string topic);
        void addMember(Client &member);
        void removeMember(Client &member);
        void setkey(std::string);
        void setlim(int);


        ~Channel();
};






