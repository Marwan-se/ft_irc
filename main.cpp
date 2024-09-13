/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaidi <msaidi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 17:20:28 by msekhsou          #+#    #+#             */
/*   Updated: 2024/09/13 16:23:32 by msaidi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/server.hpp"
#include <cstdlib>
#include <iostream>

bool     check_port(char *num)
{
	int i = 0;
	while (num[i])
	{
		if (!isdigit(num[i]))
			return (false);
		i++;
	}
	return (true);
}

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
    server.Server_connection(port);
 
	return (0);
}
