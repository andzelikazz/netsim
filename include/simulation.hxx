#ifndef AAANETSIM_SIMULATION_HPP
#define AAANETSIM_SIMULATION_HPP

#include "factory.hxx"
#include "types.hxx"
#include <functional>
#include <set>

void simulate(Factory &f, TimeOffset duration,
              std::function<void(Factory &, Time)> rf);

class SpecificTurnsReportNotifier {
public:
  SpecificTurnsReportNotifier(std::set<Time> turns) : turns_(turns) {};
  bool should_generate_report(Time t) const {
    return turns_.find(t) != turns_.cend();
  };

private:
  std::set<Time> turns_;
};

class IntervalReportNotifier {
public:
  IntervalReportNotifier(TimeOffset interval) : interval_(interval) {};
  bool should_generate_report(Time t) const {
    return t == 1 || t % interval_ == 1;
  };

private:
  TimeOffset interval_;
};

#endif
