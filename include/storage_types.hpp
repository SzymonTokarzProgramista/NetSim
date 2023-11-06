//
// Created by mateu on 04.01.2023.
//

#ifndef NETSIM_STORAGE_TYPES_HPP
#define NETSIM_STORAGE_TYPES_HPP

#include "package.hpp"
#include<list>

enum PackageQueueType {FIFO, LIFO};
class IPackageStockpile {
public:
//    IPackageStockpile(ElementID s) : s_(s) {} //wywalam ten konstruktor bo daje jakies bledy

    using const_iterator = std::list<Package>::const_iterator;
    virtual void push(Package&& t) = 0;
    virtual std::size_t size() = 0;
    virtual bool empty() = 0;
    virtual IPackageStockpile::const_iterator cbegin() const = 0;
    virtual IPackageStockpile::const_iterator begin() const = 0;
    virtual IPackageStockpile::const_iterator cend() const = 0;
    virtual IPackageStockpile::const_iterator end() const = 0;

    virtual ~IPackageStockpile() = default;

//protected:
//    std::list<Package> s_;  // skoro konstrukotr wywalony to to też wyjebałem
};


class IPackageQueue : public IPackageStockpile
{
protected:
    PackageQueueType queue_type;

public:
    virtual Package pop() = 0;
    virtual PackageQueueType get_queue_type() = 0;

};

class PackageQueue : public IPackageQueue
{
public:
    bool empty() override;
    PackageQueue(PackageQueueType queue_type): q_type(queue_type) {};
    Package pop() override;
    void push(Package&& t) override {s_.push_back(t);};
    std::size_t size() override {return s_.size();}
    PackageQueueType get_queue_type() override {return q_type;};
    IPackageStockpile::const_iterator cbegin() const override {return s_.cbegin();}
    IPackageStockpile::const_iterator begin() const override {return s_.begin();}
    IPackageStockpile::const_iterator cend() const override { return s_.cend();}
    IPackageStockpile::const_iterator end() const override { return s_.end();}
private:
    PackageQueueType q_type;
    std::list<Package> s_;
};



#endif //NET_SIM_STORAGE_TYPES_HPP