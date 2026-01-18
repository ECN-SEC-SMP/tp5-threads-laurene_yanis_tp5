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
  bool trainPresent = false;
  int numPrecedent = 0;
  int nbtrainEngage = 0;
  bool autorisation1 = false;
  bool autorisation2 = false;
  int sensA = 0;
  int sensB = 0;
  int nbtchoutchouA = 0;
  int nbtchoutchouB = 0;

  public:
    Controleur (int valeur_initiale) : 
    val(valeur_initiale)
    {
    }
    
    bool controlinEnB(int numero) {
      if(sensB < 2 && nbtchoutchouB < 3){
        autorisation2 = true;
      }
      if(trainPresent == false || (trainPresent == true && numPrecedent<0 && autorisation1 == true && autorisation2 == true)) {
        trainPresent = true;
        nbtrainEngage++;
        sensB++;
        nbtchoutchouB++;
        sensA=0;
        nbtchoutchouA=0;
        if(nbtrainEngage == 2){
          autorisation1 = false;
        }
        numPrecedent = numero;
        return true;
      } 
      else {
        return false;
      }
    }  

    bool controlinEnA(int numero) {
      if(sensA < 2 && nbtchoutchouA < 3){
        autorisation2 = true;
      }
      if(trainPresent == false || (trainPresent == true && numPrecedent>0 && autorisation1 == true && autorisation2 == true)) {
        nbtrainEngage++;
        sensA++;
        nbtchoutchouA++;
        sensB=0;
        nbtchoutchouB=0;
        trainPresent = true;
        if(nbtrainEngage == 2){
          autorisation1 = false;
        }
        numPrecedent = numero;
        return true;
      } 
      else {
        return false;
      }
    }  

    bool controloutEnB(int numero){
      nbtrainEngage--;
      if(nbtrainEngage == 0){
        trainPresent = false; // pas de train
        autorisation1 = true;
      }
      return trainPresent;
    }

    bool controloutEnA(int numero){ 
      nbtrainEngage--;
      if(nbtrainEngage == 0){
        trainPresent = false; // pas de train
        autorisation1 = true;
      } 
      return trainPresent;
    }

  private:
    int val;
};

#endif //CONTROLEUR_HPP
