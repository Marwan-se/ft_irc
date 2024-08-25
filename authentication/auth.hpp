/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   auth.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msekhsou <msekhsou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 22:38:33 by msekhsou          #+#    #+#             */
/*   Updated: 2024/08/25 14:42:38 by msekhsou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AUTH_HPP
# define AUTH_HPP

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#define ERR_PASSWDMISMATCH "Password incorrect"

class Auth
{
    public:
    
    void    authenticate(const std::string &input);
};


#endif