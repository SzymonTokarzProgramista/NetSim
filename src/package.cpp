//
// Created by mateu on 04.01.2023.
//

#include "package.hpp"

std::set<ElementID> Package::freed_IDs_;
std::set<ElementID> Package::assigned_IDs_;


Package::Package() {
    if (!freed_IDs_.empty()) {
        id = *freed_IDs_.begin();
        assigned_IDs_.insert(id);    /* dodanie nowego id  do zbioru przypsanych */
        freed_IDs_.erase(id);    /* usunięcie przypisanego ze zbioru wolnych */
    }
    else if (!assigned_IDs_.empty()) {  //tutaj brakowało elsa po prostu
        id = *(assigned_IDs_.end()) + 1;        /* inkrementacja */
        assigned_IDs_.insert(id);
    }
    else {
        id = 1;
        assigned_IDs_.insert(id);
    }
}

bool Package::operator==(const Package&& p) {
    return this->id == p.id;
}

Package::Package(ElementID ID) {
    id = ID;
    assigned_IDs_.insert(id);
}
//
Package::~Package() {
    assigned_IDs_.erase(id);   // usuwamm stary
    freed_IDs_.insert(id);   // dodaje do wolnych do uzycia

}
