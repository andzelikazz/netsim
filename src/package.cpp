#include "package.hxx"

std::set<ElementID> Package::assigned_IDs;
std::set<ElementID> Package::freed_IDs;


Package::Package() {
    if (!freed_IDs.empty()) {
        ID = *freed_IDs.begin();
        freed_IDs.erase(freed_IDs.begin());
    }
    else if (assigned_IDs.empty()) {
        ID = ElementID{1}; 
    }
    else {
        ID = *assigned_IDs.rbegin() + 1;
    }

    assigned_IDs.insert(ID);
}
    
Package::~Package() {
    if (ID != ElementID{}) {
        freed_IDs.insert(ID);
        assigned_IDs.erase(ID);
    }
}

Package& Package::operator=(Package&& other) noexcept {
    if (this == &other)
        return *this;

    assigned_IDs.erase(this->ID);
    freed_IDs.insert(this->ID);

    this->ID = other.ID;
    assigned_IDs.insert(this->ID);

    other.ID = ElementID{};

    return *this;
}