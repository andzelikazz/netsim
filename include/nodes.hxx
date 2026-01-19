#include <iostream>
#include <optional>
#include <memory>

#include "package.hxx"
#include "types.hxx"

class IPackageReceiver{
  public:
    void receive_package(Package&& p);
    const ElementID get_id() const;
};


class ReceiverPreferences {
public:
  ReceiverPreferences(ProbabilityGenerator pg);
  void add_receiver(IPackageReceiver* r); 
  void remove_receiver(IPackageReceiver* r);
  IPackageReceiver* choose_receiver();
  const preferences_t& get_preferences() const;
  
  int preferences_t; 
};


class PackageSender: ReceiverPreferences{
public:
  PackageSender(PackageSender &&) = default;
  void send_package();
  const std::optional<Package>& get_sending_buffer() const;
  void push_package(Package&&); 

  ReceiverPreferences receiver_preferences; 
};


class Ramp: PackageSender{
public:
  Ramp(ElementID id, TimeOffset di);
  void deliver_goods(Time t);
  const TimeOffset get_delivery_interval() const;
  const ElementID get_id() const;
};


class Worker: PackageSender{
public:
  Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q);
  void do_work(Time t);
  const TimeOffset get_processingn_duration() const;
  const Time get_package_processing_start_time() const;
};


class Storehouse {
public:
  Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d);
};

