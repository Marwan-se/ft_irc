/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   auth.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msekhsou <msekhsou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 22:44:35 by msekhsou          #+#    #+#             */
/*   Updated: 2024/08/26 17:55:43 by msekhsou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "auth.hpp"

void Auth::parse_input(const std::string &input)
{
    std::stringstream ss(input);
    
    std::string command;
    std::string params;

    ss >> command >> params;
    
    //print ss content
    std::cout << "ss content: " << ss.str() << std::endl;
    
    
    
    
}

void Auth::authenticate(const std::string &input)
{
    
}
