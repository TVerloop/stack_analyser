/*
 * propertypair.hpp
 *
 *  Created on: Nov 2, 2016
 *      Author: Tom
 */

#ifndef PROPERTYPAIR_HPP_
#define PROPERTYPAIR_HPP_

template<class __T1,class __T2,class __T3>
struct property_pair : public std::pair<__T1,__T2>
{
  __T3 property;
};

#endif /* PROPERTYPAIR_HPP_ */
