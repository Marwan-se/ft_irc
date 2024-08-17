/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelFile.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaidi <msaidi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 10:34:01 by msaidi            #+#    #+#             */
/*   Updated: 2024/08/17 18:01:14 by msaidi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNELFILE_HPP
# define CHANNELFILE_HPP

#include <string>
#include <vector>

class Channel
{
    private:
    std::string name;
    std::string topic;
    std::vector<std::string> members;
    std::vector<std::string> ops;
    
    public:
    Channel(std::string name, std::string topic);
    std::string getName();
    std::string getTopic();
    std::vector<std::string> getMembers();
    std::vector<std::string> getOps();
    void setName(std::string name);
    void setTopic(std::string topic);
    void addMember(std::string member);
    void addOp(std::string op);
    void removeMember(std::string member);
    void removeOp(std::string op);

    ~Channel();
};










#endif

