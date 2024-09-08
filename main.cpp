/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaidi <msaidi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 17:20:28 by msekhsou          #+#    #+#             */
/*   Updated: 2024/09/06 11:44:42 by msaidi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include <iostream>

int main(int ac, char **av)
{
    Server server;
    if (ac != 2)
    {
        std::cerr << "Usage: ./server <port>" << std::endl;
        return (1);
    }
    // get port from input
    int port = std::stoi(av[1]);
    server.Server_connection(port, "aaa");
 
    return (0);
}