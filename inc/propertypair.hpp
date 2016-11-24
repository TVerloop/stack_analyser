/**
 * \file propertypair.hpp
 */

#ifndef PROPERTYPAIR_HPP_
#define PROPERTYPAIR_HPP_

/** defines the type of a edge that connects the cgraph_nodes */
enum class cgraph_edge_type
{
	call, /**< edge type is a call */
	inlined_call,  /**< edge type is an inlined call */
	definition_link,  /**< edge type is a definition */
	clone_link  /**< edge type is a clone */
};

//std includes
#include <utility> //std::pair<T1,T2>


/**
 * \class property_pair
 *
 *
 * \brief used to define edges if the call graph
 *
 *
 * \author Tom Verloop
 *
 * \version 1.0
 *
 *
 * Contact: Tom_Verloop@outlook.com
 *
 * Created on: Nov 2, 2016
 *
 */
template<class __T1, class __T2, class __T3>
struct property_pair: public std::pair<__T1, __T2> {
__T3 property; /**< contains the edge type */
};
#endif /* PROPERTYPAIR_HPP_ */
