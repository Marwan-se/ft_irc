/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tools.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msekhsou <msekhsou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 16:48:40 by yrrhaibi          #+#    #+#             */
/*   Updated: 2024/09/13 09:38:03 by msekhsou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ChannelFile.hpp"
#include "server.hpp"
#include "RPL.hpp"

class Client;

std::string comm_gen(std::string comm);
bool check_membre(std::vector<Client> vec, std::string name);
bool is_invite(std::vector<std::string> &vec, std::string name);
std::string join_members(std::vector<Client> members);