#ifndef CONTROLEUR_HPP
#define CONTROLEUR_HPP

#include <thread>             
#include <iostream>
#include <mutex>
#include <condition_variable>

using namespace std; 


/**
 * @file Controleur.hpp
 * @brief Contrôleur de synchronisation pour la circulation de trains
 *
 * Cette classe gère l'accès concurrent de trains à une section critique
 * en imposant des règles de priorité, de sens de circulation et de limitation
 * du nombre de trains engagés.
 */

/**
 * @class Controleur
 * @brief Contrôleur d'accès pour trains circulant en sens A et B
 *
 * Le contrôleur autorise ou refuse l'entrée des trains dans la section critique
 * selon :
 * - le sens de circulation précédent,
 * - le nombre de trains déjà engagés,
 * - des règles de limitation par sens.
 */
class Controleur {
  private:
  mutex acces; /** Mutex protégeant l'état interne du contrôleur */
  bool trainPresent = false; /** Indique si au moins un train est présent dans la section critique */
  /**
     * Numéro du dernier train autorisé.
     * Le signe du numéro est utilisé pour déterminer le sens précédent.
     */
  int numPrecedent = 0;
  int nbtrainEngage = 0; /** Nombre de trains actuellement engagés dans la section critique */
  bool autorisation1 = false; /** Autorisation principale de passage */
  bool autorisation2 = false; /** Autorisation secondaire liée aux conditions de sens */
  int sensA = 0; /** Nombre de trains consécutifs passés en sens A */
  int sensB = 0; /** Nombre de trains consécutifs passés en sens B */
  int nbtchoutchouA = 0; /** Nombre total de trains autorisés en sens A */
  int nbtchoutchouB = 0; /** Nombre total de trains autorisés en sens B */

  public:
  /**
     * @brief Constructeur du contrôleur
     *
     * Initialise le contrôleur avec une valeur entière interne.
     *
     * @param valeur_initiale Valeur initiale stockée dans le contrôleur
     */
    Controleur (int valeur_initiale) : 
    val(valeur_initiale)
    {
    }
    
    /**
     * @brief Demande d'entrée d'un train en sens B
     *
     * Autorise ou refuse l'entrée d'un train circulant en sens B
     * en fonction de l'état interne du contrôleur, du sens précédent
     * et des règles de limitation.
     *
     * @param numero Numéro identifiant le train
     * @return true si l'accès est autorisé, false sinon
     */
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

    /**
     * @brief Demande d'entrée d'un train en sens A
     *
     * Autorise ou refuse l'entrée d'un train circulant en sens A
     * selon les mêmes règles que pour le sens B, adaptées au sens A.
     *
     * @param numero Numéro identifiant le train
     * @return true si l'accès est autorisé, false sinon
     */
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

    /**
     * @brief Sortie d'un train circulant en sens B
     *
     * Met à jour l'état interne lors de la sortie d'un train.
     * Si aucun train ne reste engagé, la section critique est libérée.
     *
     * @param numero Numéro du train sortant
     * @return true si un train est encore présent, false sinon
     */
    bool controloutEnB(int numero){
      nbtrainEngage--;
      if(nbtrainEngage == 0){
        trainPresent = false; // pas de train
        autorisation1 = true;
      }
      return trainPresent;
    }

    /**
     * @brief Sortie d'un train circulant en sens A
     *
     * Met à jour l'état interne lors de la sortie d'un train.
     *
     * @param numero Numéro du train sortant
     * @return true si un train est encore présent, false sinon
     */
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
