/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelFile.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaidi <msaidi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 10:34:01 by msaidi            #+#    #+#             */
/*   Updated: 2024/09/14 11:21:24 by msaidi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include "client.hpp"


class Client;
class Channel
{
    private:
        std::string name;
        std::string topic;
        std::vector<Client> members;
        std::vector<std::string> invited;
        size_t lim;
        std::string key;
        std::time_t topicTime;
        bool t;
        bool i;
        bool k;
        bool l;

    public:

        Channel();
        Channel(std::string name);
        std::string getName();
        std::string getTopic();
        std::vector<Client> &getMembers();
        std::vector<std::string> &getInvited();
        std::string getkey();
        size_t getLimNum();
        bool getKeyRES();
        bool getTopicRES();
        bool getInviteOnly();
        bool getLimit();
        std::time_t getTime();
        Client& getClientMember(std::string nickname);


        void setLimNum(size_t l);
        void setName(std::string name);
        void setTopic(std::string topic);
        void setKey(std::string );
        void addMember(Client &member);
        void removeMember(Client &member);
        void addInvited(std::string Invited);
        void removeInvited(std::string Invited);
        void setTopicRES(bool x);
        void setInviteOnly(bool x);
        void setKeyRES(bool x);
        void setLimit(bool x);
        void setTime(time_t x);


        ~Channel();
};






