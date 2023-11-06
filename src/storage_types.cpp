//
// Created by mateu on 04.01.2023.
//

#include "storage_types.hpp"

bool PackageQueue::empty() {
    if (s_.size() != 0){
        return false;
    }
    else {
        return true;
    }
    //return Package();
}

Package PackageQueue::pop(){
    if (q_type == PackageQueueType::FIFO) {
        auto p = std::move(s_.front());
        s_.pop_front();
        return p;
    }
    else
    {
        Package p = std::move(s_.back());
        s_.pop_back();
        return p;
    }
}