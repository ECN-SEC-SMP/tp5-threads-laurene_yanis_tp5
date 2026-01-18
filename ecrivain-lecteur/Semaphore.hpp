#ifndef SEMAPHORE_HPP
#define SEMAPHORE_HPP

#include <iostream>
#include <mutex>
#include <condition_variable>

using namespace std; 


/**
 * @file Semaphore.hpp
 * @brief Implémentation d'un sémaphore pour la synchronisation de threads
 *
 * Cette classe implémente un sémaphore classique basé sur
 * un mutex et une variable de condition C++.
 * Elle permet de contrôler l'accès concurrent à une ressource partagée.
 */

/**
 * @class Semaphore
 * @brief Sémaphore de synchronisation pour threads
 *
 * Le sémaphore est initialisé avec une valeur entière.
 * Les opérations P() et V() permettent respectivement
 * de bloquer ou libérer des threads.
 */

class Semaphore {
  public:
    /**
     * @brief Constructeur du sémaphore
     *
     * Initialise le sémaphore avec une valeur initiale.
     *
     * @param valeur_initiale Valeur initiale du compteur du sémaphore
     */
    Semaphore (int valeur_initiale)
    : val(valeur_initiale) 
    {
            cout << "valeur initiale du sémaphore = " << val << endl;

    }
    
    /**
     * @brief Opération V (signal)
     *
     * Incrémente le compteur du sémaphore et réveille
     * un thread bloqué, le cas échéant.
     */
    inline void V (  ) { // int tid
      std::unique_lock<std::mutex> lock(mtx);
      val++;
      //cout << "processus " << tid <<  " V" << endl;
      //déblocage d'un processus bloqué (si il en existe)
      cv.notify_one();
    }
    

    /**
     * @brief Opération P (wait)
     *
     * Décrémente le compteur du sémaphore.
     * Si la valeur est nulle ou négative, le thread appelant est bloqué
     * jusqu'à ce qu'un autre thread appelle V().
     */
    inline void P (  ) { // int tid
      std::unique_lock<std::mutex> lock(mtx);
      while(val == 0) {
        //cout << "processus " << tid << " P" << endl;
        //attente jusqu'à invocation d'un V() -> blocage du processus appelant
        cv.wait(lock);
        //cout << "processus " << tid << " reprend" << endl;
      }
      val--;
    }
  private:
    std::mutex mtx; /**< Mutex protégeant l'accès au compteur */
    std::condition_variable cv; /**< Variable de condition pour l'attente */
    int val=0; /**< Compteur du sémaphore */
};

#endif //SEMAPHORE_HPP

