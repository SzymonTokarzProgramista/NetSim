//
// Created by mateu on 04.01.2023.
//

#ifndef NETSIM_FACTORY_HPP
#define NETSIM_FACTORY_HPP
#include "storage_types.hpp"
#include "nodes.hpp"
#include <list>


enum class NodeColor { UNVISITED, VISITED, VERIFIED };
bool has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors); // z instrukcji do zajeć

template <class Node>
class NodeCollection{
public:
    using container_t = typename std::list<Node>;  // aliasy na typy iteratorów
    using iterator = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;

    iterator begin() {return container.begin();};  // dostęp do metod begin i end
    iterator end() {return container.end();};
    const_iterator cbegin() const {return container.cbegin();};
    const_iterator cend() const {return container.cend();};

    void add(Node&& node) {container.push_back(std::move(node));};  // zawłaszczenie przekazanego obiektu do węzła
    void remove_by_id(ElementID id){
        container.remove_if([id](const Node& el) {return el.get_id() == id;}); // remove if musi zawierac predykant, usniecie elementu o danym id
    };
    NodeCollection<Node>::iterator find_by_id(ElementID id){  // szykanie po id
        return std::find_if(container.begin(),container.end(),[id](const Node& el) {return el.get_id() == id;}); // przeszukaj od poczatku do konca i znajdz takie ktore rowna sie id
    };
    NodeCollection<Node>::const_iterator find_by_id(ElementID id) const { // szukanie po id
        return std::find_if(container.begin(),container.end(),[id](const Node& el) {return el.get_id() == id;}); // przeszukaj od poczatku do konca i znajdz takie ktore rowna sie id
    };

private:
    container_t container;
};


class Factory {
public:
    void add_ramp(Ramp&& r) {cont_r.add(std::move(r));}; // wrzucenie rampy
    void remove_ramp(ElementID id) {cont_r.remove_by_id(id);}; // wyrzucenie rampy o danym id
    NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id) {return cont_r.find_by_id(id);};   // metoda do zanjodwania po id
    NodeCollection<Ramp>::const_iterator find_ramp_by_id(ElementID id) const {return cont_r.find_by_id(id);};
    NodeCollection<Ramp>::const_iterator ramp_cbegin() const {return cont_r.cbegin();}; // metody dostępu
    NodeCollection<Ramp>::const_iterator ramp_cend() const {return cont_r.cend();};

    void add_worker(Worker&& w) {cont_w.add(std::move(w));}; // dodanie pracownika
    void remove_worker(ElementID id){
        Worker* node = &(*cont_w.find_by_id(id));
        std::for_each(cont_r.begin(),cont_r.end(),[&node](Ramp& ramp){
            ramp.receiver_preferences_.remove_receiver(node); // znajdz i usuń
        });
        std::for_each(cont_w.begin(),cont_w.end(),[&node](Worker& worker){
            worker.receiver_preferences_.remove_receiver(node); // rwonież znajdz i usuń
        });
        cont_w.remove_by_id(id);// no i usun pracownika
    }
    NodeCollection<Worker>::iterator find_worker_by_id(ElementID id) {return cont_w.find_by_id(id);}; //metody znajodwania po id
    NodeCollection<Worker>::const_iterator find_worker_by_id(ElementID id) const {return cont_w.find_by_id(id);};
    NodeCollection<Worker>::const_iterator worker_cbegin() const {return cont_w.cbegin();}; // metody dostępu
    NodeCollection<Worker>::const_iterator worker_cend() const {return cont_w.cend();};

    void add_storehouse(Storehouse&& s) {cont_s.add(std::move(s));}; // dodanie magazynu
    void remove_storehouse(ElementID id){
        Storehouse* node = &(*cont_s.find_by_id(id));
        std::for_each(cont_r.begin(),cont_r.end(),[&node](Ramp& ramp){
            ramp.receiver_preferences_.remove_receiver(node); // znajdz i usuń
        });
        std::for_each(cont_w.begin(),cont_w.end(),[&node](Worker& worker){
            worker.receiver_preferences_.remove_receiver(node); // rwonież znajdz i usuń
        });
        cont_s.remove_by_id(id); // no i usuń magazyn
    }
    NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID id) {return cont_s.find_by_id(id);}; // Metody znajdowania po id
    NodeCollection<Storehouse>::const_iterator find_storehouse_by_id(ElementID id) const {return cont_s.find_by_id(id);};
    NodeCollection<Storehouse>::const_iterator storehouse_cbegin() const {return cont_s.cbegin();}; // metody dostępu
    NodeCollection<Storehouse>::const_iterator storehouse_cend() const{return cont_s.cend();};

    bool is_consistent(); // sprawdzanie sieci
    void do_deliveries(Time); // dokonywanie ewentuualnych dostaw
    void do_package_passing(); //dokonywanie ewentualnego przekazywania półproduktów
    void do_work(Time); //dokonywanie ewentualnego przetwarzania półproduktów przez robotników

private:
    template<class Node>
            void remove_reveiver(NodeCollection<Node>& collection, ElementID id);
    NodeCollection<Ramp> cont_r; // kontener ramp
    NodeCollection<Worker> cont_w; // kontener worker
    NodeCollection<Storehouse> cont_s; //kontener magazyn
};

#endif //NETSIM_FACTORY_HPP
