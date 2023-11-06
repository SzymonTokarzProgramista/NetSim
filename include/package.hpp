//
// Created by mateu on 04.01.2023.
//

#ifndef NETSIM_PACKAGE_HPP
#define NETSIM_PACKAGE_HPP

#include "types.hpp"
#include <set>
/*
class Package{
public:
    Package();
    Package(ElementID ID);
    Package(Package&&) noexcept;
    Package(Package&) = delete;
    bool operator == (const Package&& p);
    ElementID get_id() const { return id; }

    ~Package();

private:
    static std::set<ElementID> freed_IDs_;
    static std::set<ElementID> assigned_IDs_;
    ElementID id = -1;
}; */

class Package{
public:
    Package();
    Package(ElementID ID);

    bool operator==(const Package&& p);
    ElementID get_id() const { return id; }

    ~Package();

protected:
    static std::set<ElementID> freed_IDs_;
    static std::set<ElementID> assigned_IDs_;
    ElementID id;
};


#endif //NET_SIM_PACKAGE_HPP
