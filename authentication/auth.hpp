/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   auth.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msekhsou <msekhsou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 22:38:33 by msekhsou          #+#    #+#             */
/*   Updated: 2024/08/23 22:43:45 by msekhsou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AUTH_HPP
# define AUTH_HPP

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

class Auth
{
    private:
        std::string password;
    public:
        static bool is_authenticated;
};


#endif