/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   auth.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msekhsou <msekhsou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 22:38:33 by msekhsou          #+#    #+#             */
/*   Updated: 2024/08/26 17:33:57 by msekhsou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AUTH_HPP
# define AUTH_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>

#define ERR_PASSWDMISMATCH "Password incorrect"
#define ERR_NEEDMOREPARAMS "Not enough parameters"

class Auth
{
    public:
    
    void    authenticate(const std::string &input);
    void    parse_input(const std::string &input);
};


#endif
