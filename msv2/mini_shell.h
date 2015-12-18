#ifndef MINI_SHELL_H
#define MINI_SHELL_H

/*--------------------------------------------------------------------------
 * header pour le mini-shell où sont définies les constantes
 * -----------------------------------------------------------------------*/
#define NB_MAX_JOBS 1      // nb max de lignes que l'on peut exécuter concuremment
#define NB_MAX_COMMANDES 3 // nb max de commandes sur une ligne
#define NB_MAX_MOTS 30     // nb max de mots pour une commande
#define NB_MAX_CAR 4096    // nb max de caractères dans une ligne

/*--------------------------------------------------------------------------
 * headers à inclure afin de pouvoir utiliser certains types
 * -----------------------------------------------------------------------*/
#include <unistd.h>    // pour pid_t
#include <sys/types.h> // pour pid_t
#include <signal.h>    // pour struct sigaction

/*--------------------------------------------------------------------------
 * type structuré décrivant un job
 * -----------------------------------------------------------------------*/
typedef struct job_t
{
  char   nom[NB_MAX_CAR];             // ligne de commandes exécutée par le job
  pid_t  pids[NB_MAX_COMMANDES+1];    // pids des fils exécutant chaque commande
                                      // pids[p]==-2 si le job n'a pas de pème fils
                                      // pids[p]==0 si le pème fils s'est terminé
                                      // le job est en cours d'exécution si pids[0]!=-2
  int    tubes[NB_MAX_COMMANDES-1][2];   // Un tube pour chaque commande
} job_t;

/*--------------------------------------------------------------------------
 * type structuré décrivant un ensemble de jobs
 * -----------------------------------------------------------------------*/
typedef struct job_set_t
{
  job_t  jobs[NB_MAX_JOBS];
  int    job_fg; //numéro du job en avant-plan, qu'il faut donc attendre; =-2 s'il n'y en a pas
                 // ne sera utile qu'à partir de la version avec gestion de SIGCHLD
} job_set_t;

/*--------------------------------------------------------------------------
 * type structuré contenant les fruits de l'analyse d'une ligne de commandes
 * -----------------------------------------------------------------------*/
typedef struct ligne_analysee_t
{
  int    nb_fils;			           // nb de fils à créer pour la ligne reçue
  char*  commandes[NB_MAX_COMMANDES][NB_MAX_MOTS]; // contient les mots des différentes commandes
  char   ligne[NB_MAX_CAR];			   // contient la ligne d'entrée
} ligne_analysee_t;

#endif
