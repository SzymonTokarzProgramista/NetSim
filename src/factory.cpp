//
// Created by mateu on 04.01.2023.
//
//#include "../include/factory.hpp"
#include "factory.hpp"
#include <istream>

bool has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors){
    if(node_colors[sender] == NodeColor::VERIFIED){
        return true; // jeśli zweryfikowany zwróc prawde
    }
    node_colors[sender] = NodeColor::VISITED; // oznacz jako odwiedzony
    if(sender -> receiver_preferences_.get_preferences().empty()){
        throw std::logic_error("Sender hasn't got any receivers"); // rzuć wyjątek że jest pusty
    }
    bool sender_has_one_receiver_other_than_himself = false; // flaga czy ma kogokowliek wiecej do obirou niż siebie
    for(auto receiver : sender -> receiver_preferences_.get_preferences()){
        if(receiver.first->get_receiver_type() == ReceiverType::STOREHOUSE){  // jestli typ odbiorcy to magazyn to :
            sender_has_one_receiver_other_than_himself = true;
        }
        else if(receiver.first->get_receiver_type() == ReceiverType::WORKER){  // ttaj wiadomo że odbiorca jest typu  pracowanik
            IPackageReceiver* receiver_ptr = receiver.first;
            auto worker_ptr = dynamic_cast<class Worker*>(receiver_ptr);  // dokonanie konwersji
            auto send_rec_ptr = dynamic_cast<PackageSender*>(worker_ptr);
            if(send_rec_ptr == sender){  // jesli wskaznik wskazuje ten sam adres co sender
                continue;
            }
            sender_has_one_receiver_other_than_himself = true;
            if(node_colors[send_rec_ptr] == NodeColor::UNVISITED){   // jesli send_rec_ptr nie został odwiedzoby
                has_reachable_storehouse(send_rec_ptr,node_colors); // wywołaj rekurencyjnie
            }
        }
    }
    node_colors[sender] = NodeColor::VERIFIED; //  zmien na zweryfikowany;
    if(sender_has_one_receiver_other_than_himself){ // jesli nadawaca ma choc jednego odbiorce
        return true;
    }else{
        throw std::logic_error("Error");//  rzuć wyjątek error
    }
}

bool Factory::is_consistent() {
    std::map<const PackageSender*, NodeColor> color;
    for(auto ptr = cont_w.begin(); ptr!= cont_w.end();ptr++){ // dla wszystkich pracowników
        Worker* worker = &(*ptr);
        auto sender = dynamic_cast<PackageSender*>(worker); // konwersja
        color[sender] = NodeColor::UNVISITED; // oznacz kolor jako nieodwiedzony
    }
    for(auto ptr = cont_r.begin(); ptr!= cont_r.end();ptr++){ // dla wszystkich ramp
        Ramp* ramp = &(*ptr);
        auto sender = dynamic_cast<PackageSender*>(ramp); // konwersja
        color[sender] = NodeColor::UNVISITED;  // ustaw kolor jako nieodwiedzony
    }
    try{ // jesli funkcja rzuci wyjątek trzeba zwrócić fałsz czyli sieć nie jest spójna
        for(auto ptr = cont_r.begin();ptr != cont_r.end();ptr++){ // dla wszystkich ramp
            Ramp* ramp = &(*ptr);
            auto sender = dynamic_cast<PackageSender*>(ramp); // konwersja
            has_reachable_storehouse(sender,color); // wywołaj funkcje do sprawdzania
        }
    }
    catch (const std::logic_error&) {
        return false;
    }
    return true;
}

void Factory::do_deliveries(Time t) {
    for(auto &ramp : cont_r){
        ramp.deliver_goods(t); // dokonywanie dostaw na rampy
    }
}

void Factory::do_package_passing() {  // przekazywwanie półproduktów
    for(auto &ramp : cont_r){
        ramp.send_package();
    }
    for(auto &worker : cont_w){
        worker.send_package();
    }
}

void Factory::do_work(Time t) {
    for (auto& worker: cont_w) {
        worker.do_work(t); // dokonanie przetwarzania półproduktów przez robotników
    }
}