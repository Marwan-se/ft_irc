/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msekhsou <msekhsou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 17:20:22 by msekhsou          #+#    #+#             */
/*   Updated: 2024/09/08 13:17:55 by msekhsou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "replies.hpp"
#include "client.hpp"

#include <iostream>
#include <string>
#include <sys/poll.h>
#include <vector>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>
#include <csignal>
#include <map>


class	Server
{
	private:
		int					Socket_fd;
		struct sockaddr_in	Server_addr;
		std::vector<struct pollfd > fdes;
		std::map<int , Client> client_info;
	public:
	
		void	init_Socket(int domain, int type, int protocol, int port);
		void	Server_connection(int port, std::string password);

		struct sockaddr_in	getServer_addr();
		int		getSocket_fd();
		
		static bool signal_received_flag;
		static void SignalHandler(int signum);

		void	receive_data(int fd, std::string password);
		void	close_allfds();

		//ge
};


#endif