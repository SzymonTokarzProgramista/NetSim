//
// Created by mateu on 04.01.2023.
//

#ifndef NETSIM_NODES_HPP

#include "package.hpp"
#include "types.hpp"
#include "storage_types.hpp"
#include <optional>
#include <iostream>
#include <memory>
#include <map>
#include <utility>
#include "helpers.hpp"
#include "global_functions_mock.hpp"

enum ReceiverType {   //definije sobie dwa typy
    WORKER,
    STOREHOUSE
};

using const_iterator = IPackageStockpile::const_iterator;

class IPackageReceiver {
public:
    virtual void receive_package(Package&& pac) = 0;
    virtual ElementID get_id() const = 0;
    //virtual ReceiverType get_receiver_type() const = 0;   // konieczne do zakomentowania na tym etapie projektu
    virtual IPackageStockpile::const_iterator cbegin() const = 0;   // metody delegujące
    virtual IPackageStockpile::const_iterator begin() const = 0;
    virtual IPackageStockpile::const_iterator cend() const = 0;
    virtual IPackageStockpile::const_iterator end() const = 0;

    virtual ~IPackageReceiver() = default;  // destruktor o implementacji domyślnej
};

class Storehouse : public IPackageReceiver {
public:
    Storehouse(ElementID id,std::unique_ptr<PackageQueue> d = std::make_unique<PackageQueue>(PackageQueue(FIFO)));  //inteligentny wskaźnik nie ma znaczenia czy bedzie tu FIFO czy LIFO
    void receive_package(Package&& pac) override;
    ElementID get_id() const override {return id_;} // nadpisanie metody zwracającej ID
    //ReceiverType get_receiver_type() const override {return STOREHOUSE;} // nadpisanie metody zwracającej typ  // konieczne do zakomentowania do testów Node
    IPackageStockpile::const_iterator cbegin() const override {return d_->cbegin();}  ; // metody delegujące
    IPackageStockpile::const_iterator begin() const override {return d_->begin();};
    IPackageStockpile::const_iterator cend() const override {return d_->cend();};
    IPackageStockpile::const_iterator end() const override {return d_->end();};

private:
    ElementID id_;  // id elementu
    std::unique_ptr<PackageQueue> d_; // inteligenty wskaźnik
};

class ReceiverPreferences{
public:
    using preferences_t = std::map<IPackageReceiver*, double>; // z instrukcji
    using const_iterator = preferences_t::const_iterator;

    explicit ReceiverPreferences(ProbabilityGenerator pg = probability_generator) : pg_(std::move(pg)) {};

    void add_receiver (IPackageReceiver* r);   // dodanie odbiorcy
    void remove_receiver (IPackageReceiver* r);  // usniecie odbiorcy
    IPackageReceiver *choose_receiver();        //wybranie odbiorcy
    preferences_t& get_preferences() {return preferences_;} ; // metoda zwracająca połączenia
    preferences_t& get_preferences() const {return const_cast<preferences_t&>(preferences_);};
    bool empty () const {return preferences_.empty();};
    const_iterator cbegin() const {return preferences_.cbegin();} ; // metody delegujące
    const_iterator begin() const {return preferences_.begin();};
    const_iterator cend() const { return preferences_.cend();};
    const_iterator end() const { return preferences_.end();};

private:
    ProbabilityGenerator pg_;
    preferences_t preferences_;

};

class PackageSender : public ReceiverPreferences{
public:
    ReceiverPreferences receiver_preferences_ ;
    //PackageSender(PackageSender&&) = default ; // konstuktor przenoszący o domyślnej implementacji
    void send_package(); // przekazanie półproduktu
    std::optional<Package> &get_sending_buffer() {return buffer_;};
    const std::optional<Package> &get_sending_buffer() const {return buffer_;};
protected:
    void push_package(Package&& pac) { if (!buffer_) {buffer_.emplace(pac);}};  // wstawaianie produktu do przekazania dalej
    std::optional<Package> buffer_ = std::nullopt ;   // rodzaj jakby paczkomatu
};

class Ramp : public PackageSender {
public:
    Ramp (ElementID id, TimeOffset di);
    void deliver_goods(Time t); // metoda służąca do dostarczenia produktu
    TimeOffset get_delivery_interval() const {return di_;};   // zworc biezacy czas symulacji
    ElementID get_id() const {return id_;};  // zwroc id
private:
    ElementID id_;  // id elementu
    TimeOffset di_; // czas symulacji
    unsigned long long start_time_ = UINTMAX_MAX; // czas na start
};

class Worker : public IPackageReceiver, public PackageSender {
public:
    Worker(ElementID id,TimeOffset pd, std::unique_ptr<PackageQueue> q); // kosntruktor worker
    ElementID get_id() const override {return id_;};  // daj id
    //ReceiverType get_receiver_type() const override {return WORKER;};  // jaki typ // zakomentowane dla testów Node
    void do_work(Time t); // metoda służąca do wykonywania pracy
    void receive_package(Package&& pac) override ; // nadpisanie metody
    TimeOffset get_procesing_duration() const {return pd_;}; // zwrócenie czasu symulacji
    Time get_package_processing_start_time() const {return package_processing_start_time_;};  // zwrócenie czasu startu
    const std::optional<Package>& get_package_processing_buffer() const {return buffer_processing_;}; // zwrócenie buffora
    IPackageQueue* get_queue() const {return q_.get();}; // zwrócenie wskaźnika

    IPackageStockpile::const_iterator cbegin() const override {return q_->cbegin();}  ; // metody delegujące
    IPackageStockpile::const_iterator begin() const override {return q_->begin();};
    IPackageStockpile::const_iterator cend() const override {return q_->cend();};
    IPackageStockpile::const_iterator end() const override {return q_->end();};
private:
    ElementID id_;  // id elementu
    TimeOffset pd_; // czas symulacji
    std::unique_ptr<PackageQueue> q_; // inteligenty wskaźnik
    Time package_processing_start_time_ ; // rozpoczęcie przetwarzania produktu
    std::optional<Package> buffer_processing_ ;
};

#define NETSIM_NODES_HPP

#endif //NETSIM_NODES_HPP
