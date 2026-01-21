#ifndef NODES_HPP_
#define NODES_HPP_
NODES_HPP_
#include <iostream>
#include <optional>
#include <memory>
#include <functional>
#include <map>

#include "package.hxx"
#include "types.hxx"

class IPackageReceiver{
  public:
    void receive_package(Package&& p);
    ElementID get_id() const;
};


class ReceiverPreferences {
public:
  ReceiverPreferences(std::function pg);
  void add_receiver(IPackageReceiver* r); 
  void remove_receiver(IPackageReceiver* r);
  IPackageReceiver* choose_receiver();
  const preferences_t& get_preferences() const;
  
  std::map<int, float> preferences_t; 
};


class PackageSender: ReceiverPreferences{
public:
  PackageSender(PackageSender &&) = default;
  void send_package();
  const std::optional<Package>& get_sending_buffer() const {return buffer_;};
  void push_package(Package&& package) {buffer_.emplace(std::move(package));}; 

  ReceiverPreferences receiver_preferences; 
private:
  std::optional<Package> buffer_;
};


class Ramp: PackageSender{
public:
  Ramp(ElementID id, TimeOffset di);
  void deliver_goods(Time t);
  TimeOffset get_delivery_interval() const;
  ElementID get_id() const;
};


class Worker: PackageSender{
public:
  Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q);
  void do_work(Time t);
  TimeOffset get_processingn_duration() const;
  Time get_package_processing_start_time() const;
};


class Storehouse {
public:
  Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d);
};

#endif/*NODES_HPP_*/
