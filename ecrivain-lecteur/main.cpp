/**
 * @file main.cpp
 * @brief Problème lecteurs-écrivains avec sémaphores
 *
 * Ce programme met en œuvre le problème classique
 * des lecteurs-écrivains à l'aide de threads C++
 * et de sémaphores.
 */
#include <iostream>           
#include <thread>             
#include "Semaphore.hpp"

int ressourcePartagee; /** Ressource partagée entre lecteurs et écrivains */
Semaphore sem = Semaphore(5); /** Sémaphore limitant le nombre de lecteurs simultanés */
Semaphore muex = Semaphore(1); /** Sémaphore protégeant l'accès à la variable de lecteurs */
Semaphore ecrit = Semaphore(1); /** Sémaphore garantissant l'exclusion mutuelle pour l'écriture */
int l; /** Nombre de lecteurs actifs */


/**
 * @brief Fonction exécutée par un thread lecteur
 *
 * Le lecteur lit la ressource partagée en respectant
 * les contraintes de synchronisation imposées par
 * les sémaphores.
 *
 * @param numLecteur Identifiant du lecteur
 */
void lecteur(int numLecteur){
  for (int i = 0; i < 4; i++){
    std::cout << "Lecteur n° " << numLecteur << " en cours " << endl;
    this_thread::sleep_for(chrono::milliseconds(rand() % 20000) );
    std::cout << "        Valeur lue = " << ressourcePartagee << "  " << endl;
    muex.P();
    l++;
    if(l==1) ecrit.P();
    muex.V();
    cout<<"Le lecteur lit : "<<ressourcePartagee<<"."<<endl;
    muex.P();
    l--;
    if(l==0) ecrit.V();
    muex.V();
  }
}

/**
 * @brief Fonction exécutée par un thread écrivain
 *
 * L'écrivain modifie la ressource partagée en garantissant
 * l'exclusion mutuelle vis-à-vis des autres écrivains
 * et des lecteurs.
 *
 * @param numEcrivain Identifiant de l'écrivain
 */
void ecrivain(int numEcrivain){
  int x;
  for (int i = 0; i < 4; i++){
    std::cout << "Ecrivain n° " << numEcrivain << " en cours " << endl;
    x = ressourcePartagee;
    this_thread::sleep_for(chrono::milliseconds(rand() % 20000) );
    std::cout << "valeur à incrémenter de la ressourcePartagee = " << x << "  " << endl;
    
    ecrit.P();
    ressourcePartagee = x+1 ;
    ecrit.V();
  }
}

/**
 * @brief Fonction principale
 *
 * Crée et lance les threads lecteurs et écrivains,
 * puis attend leur terminaison.
 *
 * @return 0 si l'exécution se termine correctement
 */
int main() {
  const int nbr = 8;
  std::thread r[nbr];
  std::thread w[nbr];
  int ressourcePartagee = 0;

  for (int i = 1; i < nbr; i++){
    r[i] = std::thread(lecteur, -i);
    w[i] = std::thread(ecrivain, i);
  }
// Join des threads 
  for (int i = 1; i < nbr; i++) {
  r[i].join(); 
  w[i].join();   
  }
  return 0;
}
