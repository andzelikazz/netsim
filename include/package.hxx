#ifndef PACKAGE_HPP
#define PACKAGE_HPP

#include "types.hxx"
#include <set>

class Package {
public:
	Package();
	Package(ElementID ID_) : ID(ID_) { assigned_IDs.insert(ID_); };
	Package(Package&& package) : ID(package.ID) {
		assigned_IDs.erase(package.ID);
		freed_IDs.insert(package.ID);
		package.ID = ElementID{};
	};
	Package& operator = (Package&& other) noexcept;
	ElementID get_id() const { return ID;};
	~Package(); 
private:
	ElementID ID;
	static std::set<ElementID> assigned_IDs;
	static std::set<ElementID> freed_IDs;
};
#endif 