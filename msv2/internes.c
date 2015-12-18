/*--------------------------------------------------------------------------
 * headers à inclure afin de pouvoir utiliser divers appels systèmes
 * -----------------------------------------------------------------------*/
#include <stdio.h>   // pour printf and co
#include <stdlib.h>  // pour getenv et exit
#include <string.h>  // pour avoir strcmp and co
#include <signal.h>  // pour SIG*
#include <errno.h>   // pour errno and co

/*--------------------------------------------------------------------------
 * headers à inclure pour les constantes et types spéciaux
 * -----------------------------------------------------------------------*/
#include "mini_shell.h"

/*--------------------------------------------------------------------------
 * headers à inclure pour la gestion des jobs
 * -----------------------------------------------------------------------*/
#include "jobs.h"
#include "internes.h"

/*-------------------------------------------------------------------------------
 * Macro pour éviter le warning "unused parameter" dans une version intermédiaire
 * -----------------------------------------------------------------------------*/
//#define /*UNUSED(x) (void)(x)*/

/*--------------------------------------------------------------------------
 * change directory
 * -----------------------------------------------------------------------*/
static int mon_cd(char *rep)
{
  // TODO : supprimer la ligne et compléter la procédure
    if (rep==NULL){
      return(chdir(getenv("HOME")));
    }else{
      return(chdir(rep));
    }
}

/*--------------------------------------------------------------------------
 * affiche la ligne de commande
 * -----------------------------------------------------------------------*/
static int mon_echo(char **mots)
{
    for (int m=1; m<NB_MAX_MOTS && mots[m]; m++)
      printf("%s",mots[m]);
    printf("\n");
    return 1;
}

/*--------------------------------------------------------------------------
 * kill un job : exemple d'utilisation de action_job().
 * ne sera réellement utile que dans la version multi-jobs.
 * -----------------------------------------------------------------------*/
static int mon_kill(char *numero, job_set_t *mes_jobs)
{
  int j_k=-2;

    // si commande sans argument : erreur
    if (! numero)
    {
      printf("Usage kill <numero de job à terminer>\n");
      return 1;
    }

    // interprétation de l'argument comme un numéro de job
    j_k=atoi(numero);

    if (j_k>=NB_MAX_JOBS || mes_jobs->jobs[j_k].pids[0]==-2 )
      printf ("Le job %i n'existe pas\n",j_k);
    else
      action_job(j_k,mes_jobs->jobs[j_k],SIGKILL,NULL);

    return 1;
}


/*--------------------------------------------------------------------------
 * exécute la commande selon une commande interne si elle existe
 * -----------------------------------------------------------------------*/
int commande_interne(ligne_analysee_t *ligne_analysee, job_set_t *mes_jobs)
{
  if (! ligne_analysee->commandes[0][0]) // commande vide
  {
    return 1;
  }
  else
  if (! strcmp(ligne_analysee->commandes[0][0],"cd") )
  {
    return mon_cd(ligne_analysee->commandes[0][1]);
  }
  else
  if (! strcmp(ligne_analysee->commandes[0][0],"mon_echo") )
  {
    return mon_echo(ligne_analysee->commandes[0]);
  }
  else
  if (! strcmp(ligne_analysee->commandes[0][0],"kill-job") )
  {
    return mon_kill(ligne_analysee->commandes[0][1], mes_jobs);
  }
  else
    return 0;
}
