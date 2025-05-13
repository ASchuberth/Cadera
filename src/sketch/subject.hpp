#pragma once

#include "observer.hpp"

namespace CADERA_APP_NAMESPACE {

class Subject {

public:
  Subject();
  ~Subject();
  
  void addRender(Observer* observer);
  void removeRender(Observer* observer);
  virtual void notify();

protected:
  
std::vector<Observer*> observers;

private:

  


};
}