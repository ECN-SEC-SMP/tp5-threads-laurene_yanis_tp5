#ifndef CONTROLEUR_HPP
#define CONTROLEUR_HPP

#include <thread>             
#include <iostream>
#include <mutex>
#include <condition_variable>

using namespace std; 


class Controleur {
  private:
  mutex acces;
  bool trainPresent;

  public:
    Controleur (int valeur_initiale) : 
    val(valeur_initiale)
    {
    }
    
    bool controlinEnB(int numero) { 
        if(trainPresent == false) {
          trainPresent = true;
          return true;
        } 
        else {
          return false;
        }
    }  

    bool controlinEnA(int numero) { 
        if(trainPresent == false) {
          trainPresent = true;
          return true;
        } 
        else {
          return false;
        }
    }  

    bool controloutEnB(int numero){
      trainPresent = false; // pas de train
      return trainPresent;
    }

    bool controloutEnA(int numero){ 
      trainPresent = false; // pas de train
      return trainPresent;
    }

  private:
    int val;
};

#endif //CONTROLEUR_HPP
