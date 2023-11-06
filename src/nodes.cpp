//
// Created by mateu on 04.01.2023.
//

#include "nodes.hpp"



extern std::random_device rd;
extern std::mt19937 rng;

Storehouse::Storehouse(ElementID id, std::unique_ptr<PackageQueue> d) {  // implementacja prostego konstruktora
    id_ = id;
    d_ = std::move(d);
}

void Storehouse::receive_package(Package&& pac) {
    d_->push(std::move(pac));  // wstawienie na szczyt stosu
}

void ReceiverPreferences::add_receiver(IPackageReceiver* r) {
    preferences_.emplace(r,1); // dodaje odbiorce do konteera
    for(auto i = preferences_.begin();i != preferences_.end();i++){
        i->second = 1/(double)(preferences_.size()); // skalowanie prawdowpodobienstaa wylosowania odbiorcy
    }
}

IPackageReceiver *ReceiverPreferences::choose_receiver() {
    double dys =  pg_();
    double suma = 0 ;
    for(const auto& i:preferences_){ //przeszukuje poprzez wylosowane prawdopodobienstwo szukam odbiorcy
        suma += i.second;
        if(dys <=suma) {
            return i.first;
        }
    }
    throw std::logic_error("Brak odbiorcy"); // jesli sie nie uda daje błąd
}

void ReceiverPreferences::remove_receiver(IPackageReceiver* r) {
    preferences_.erase(r); // usuniecie
    if(preferences_.size() !=0 ){ // jesli coś tam wgl jest
        for(auto i = preferences_.begin();i != preferences_.end();i++) {
            i->second = 1 / (double)(preferences_.size()); // skalowanie prawdowpodobienstaa wylosowania odbiorcy
        }
    }
}

void PackageSender::send_package() {
    if(buffer_){
        receiver_preferences_.choose_receiver()->receive_package(buffer_->get_id()); // przekazanie zawartości bufora
        buffer_.reset();
    }
}

Ramp::Ramp(ElementID id, TimeOffset di) {  // implementacja konstruktora Ramp
    id_ = id;
    di_ = di;
}
void Ramp::deliver_goods(Time t){
    if(start_time_ == UINTMAX_MAX) start_time_ = t; // ustawienie czasu
    if((t-start_time_)%di_ == 0){  // aby wywołać w kazdej turze symulacji
        if(!buffer_){
            Package pac = Package();  // dostarczenie produktu
            push_package(std::move(pac));
        }
    }
}

Worker::Worker(ElementID id, TimeOffset pd, std::unique_ptr<PackageQueue> q) {  // konstruktor Worker implementacja
    id_ = id;
    pd_ = pd;
    q_ = std::move(q);
}

void Worker::receive_package(Package&& pac) {
    PackageSender::push_package(std::move(pac));
    //q_->push(std::move(pac));  //  przekazanie paczki robotnikowi
}

void Worker::do_work(Time t) {  // metoda do wykonywania pracy przez robotnika
    if(!q_->empty() and buffer_processing_){
        buffer_processing_ = q_->pop();
        package_processing_start_time_ = t;
    }
    if( (t-package_processing_start_time_ + 1) >= pd_ and bool(buffer_processing_)){
        push_package(std::move(buffer_processing_.value()));
        buffer_processing_.reset();
        package_processing_start_time_ = 0;
    }
}