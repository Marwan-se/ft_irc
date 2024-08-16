/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msekhsou <msekhsou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 17:20:28 by msekhsou          #+#    #+#             */
/*   Updated: 2024/08/16 00:57:30 by msekhsou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include <iostream>

int main(int ac, char **av)
{
    Server server;

    signal(SIGINT, server.signal_received);
    if (ac != 2)
    {
        std::cerr << "Usage: ./server <port>" << std::endl;
        return (1);
    }
    // get port from input
    int port = ft_stoi(av[1]);
    try
    {
        while (!Server::signal_received_flag)
        {
            server.Server_connection(port);
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
 
    return (0);
}