/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msekhsou <msekhsou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 17:20:22 by msekhsou          #+#    #+#             */
/*   Updated: 2024/07/29 19:24:42 by msekhsou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP


#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>
#include <csignal>

class	Server
{
	private:
		int					Socket_fd;
		struct sockaddr_in	Server_addr;
	public:
		void	init_Socket(int domain, int type, int protocol, int port);
		void	Server_connection(int port);
		struct sockaddr_in	getServer_addr();
		int		getSocket_fd();
		
};

// class	Client
// {
// 	private:
// 		int					Client_fd;
// 		std::string			Client_ip;
	
// };

#endif