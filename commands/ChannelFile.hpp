/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelFile.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaidi <msaidi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 10:34:01 by msaidi            #+#    #+#             */
/*   Updated: 2024/09/11 18:10:38 by msaidi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNELFILE_HPP
# define CHANNELFILE_HPP

#include <cstddef>
#include <string>
#include <vector>

class Client;

class Channel
{
    private:
    std::string name;
    std::string topic;
    std::vector<Client > members;
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
    std::string getKey();
    std::string getTopic();
    std::vector<Client> &getMembers();
    std::vector<std::string> &getInvited();
    bool getTopicRES();
    bool getPass();
    bool getInviteOnly();
    bool getLimit();
    size_t getLimNum();
    Client& getClientMember(std::string nickname);

    void setLimNum(size_t l);
    void setName(std::string name);
    void setTopic(std::string topic);
    void setKey(std::string key);
    void addMember(Client &member);
    void removeMember(Client &member);
    void addInvited(std::string Invited);
    void removeInvited(std::string Invited);
    void setTopicRES(bool x);
    void setPass(bool x);
    void setInviteOnly(bool x);
    void setLimit(bool x);
    ~Channel();
};



// "324 " + client + " " + channel + " +t\r\n"






#endif

