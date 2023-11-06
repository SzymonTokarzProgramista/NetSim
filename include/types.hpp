//
// Created by mateu on 04.01.2023.
//

#ifndef NETSIM_TYPES_HPP
#define NETSIM_TYPES_HPP

#include <functional>

using ElementID = unsigned int; // typ który wyraża identyfikator produtku
using Time = unsigned int; // alias na  czas jako punkt
using TimeOffset = unsigned int; // alias na czas jako odcinek w przestrzeni
using ProbabilityGenerator = std::function<double()> ; //  alias  // do generowania liczby losowej

#endif //NETSIM_TYPES_HPP
