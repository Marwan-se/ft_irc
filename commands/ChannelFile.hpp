/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelFile.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrrhaibi <yrrhaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 10:34:01 by msaidi            #+#    #+#             */
/*   Updated: 2024/09/11 20:28:17 by yrrhaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <cstddef>
#include <cstdlib>
#include <string>
#include <vector>
#include "../server.hpp"
#include "Tools.hpp"


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


        ~Channel();
};






