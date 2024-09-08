/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelFile.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaidi <msaidi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 10:34:01 by msaidi            #+#    #+#             */
/*   Updated: 2024/09/08 11:30:09 by msaidi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNELFILE_HPP
# define CHANNELFILE_HPP

#include <string>
#include <vector>

class Client;

class Channel
{
    private:
    std::string name;
    std::string topic;
    std::vector<Client > members;
    std::vector<Client> invited;
    // int lim;
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

    void setName(std::string name);
    void setTopic(std::string topic);
    void addMember(Client &member);
    void removeMember(Client &member);

    ~Channel();
};










#endif

