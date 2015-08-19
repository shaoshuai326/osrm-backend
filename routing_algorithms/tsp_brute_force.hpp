/*

Copyright (c) 2015, Project OSRM contributors
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list
of conditions and the following disclaimer.
Redistributions in binary form must reproduce the above copyright notice, this
list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#ifndef TSP_BRUTE_FORCE_HPP
#define TSP_BRUTE_FORCE_HPP


#include "../data_structures/search_engine.hpp"
#include "../util/string_util.hpp"
#include "../util/dist_table_wrapper.hpp"

#include <osrm/json_container.hpp>

#include <cstdlib>

#include <algorithm>
#include <string>
#include <vector>
#include <limits>
#include <iostream>
#include "../util/simple_logger.hpp"




namespace osrm
{
namespace tsp
{

template <typename number>
int ReturnDistance(const DistTableWrapper<EdgeWeight> & dist_table,
                   const std::vector<number> & location_order,
                   const EdgeWeight min_route_dist,
                   const std::size_t  number_of_locations) {
    EdgeWeight route_dist = 0;
    int i = 0;
    while (i < location_order.size()) {
        route_dist += dist_table(location_order[i], location_order[(i+1) % number_of_locations]);
        ++i;
    }
    return route_dist;
}

std::vector<NodeID> BruteForceTSP(std::vector<NodeID> & component,
                                  const std::size_t number_of_locations,
                                  const DistTableWrapper<EdgeWeight> & dist_table) {

    std::vector<NodeID> route;
    route.reserve(number_of_locations);


    EdgeWeight min_route_dist = INVALID_EDGE_WEIGHT;

    // check length of all possible permutation of the component ids
    do {
        const auto new_distance = ReturnDistance(dist_table, component, min_route_dist, number_of_locations);
        if (new_distance <= min_route_dist) {
            min_route_dist = new_distance;
            route = component;
        }
    } while(std::next_permutation(std::begin(component), std::end(component)));

    return route;
}

} //end namespace osrm
} //end namespace tsp
#endif // TSP_BRUTE_FORCE_HPP