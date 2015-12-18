/*--------------------------------------------------------------------------
 * headers à inclure afin de pouvoir utiliser divers appels systèmes
 * -----------------------------------------------------------------------*/
#include <stdio.h>     // pour printf() and co
#include <stdlib.h>    // pour exit()
#include <errno.h>     // pour errno and co
#include <unistd.h>    // pour pipe
#include <sys/types.h> // pour avoir pid_t
#include <signal.h>    // pour sigaction
#include <string.h>    // pour avoir strcmp and co

#include <sys/wait.h>  // pour avoir wait and co

#include "jobs.h"
#include "externes.h"

/*-------------------------------------------------------------------------------
 * Macro pour éviter le warning "unused parameter" dans une version intermédiaire
 * -----------------------------------------------------------------------------*/
#define UNUSED(x) (void)(x)



/*--------------------------------------------------------------------------
 * crée un fils pour exécuter la commande ligne_analysee->commandes[num_comm]
 * enregistre son pid dans job->pids[num_comm]
 * le fils définit ses handlers à différents signaux grâce à sig
 * -----------------------------------------------------------------------*/
void lire_dans_le_tube (job_t *job, int num_comm){
  close(job->tubes[num_comm-1][1]);
  dup2(job->tubes[num_comm-1][0],1);//lecture
//  int res= execvp(ligne_analysee->commandes[num_comm][0],ligne_analysee->commandes[num_comm]);
//  if (res==-1){perror("Echec execvp 1"); exit(errno);}
//  close(job->tubes[num_comm-1][0]);
}
void ecrire_dans_le_tube (job_t *job, int num_comm, ligne_analysee_t *ligne_analysee){
  printf("######################################################\n");
  close(job->tubes[num_comm-1][0]);
  close(job->tubes[num_comm][0]);
  close(job->tubes[num_comm-1][1]);
  dup2(job->tubes[num_comm-1][0],job->tubes[num_comm][1]);
  //dup2(job->tubes[num_comm][1],0);//ecriture
  int res= execvp(ligne_analysee->commandes[num_comm][0],ligne_analysee->commandes[num_comm]);
  if (res==-1){perror("Echec execvp 3"); exit(errno);}
  close(job->tubes[num_comm][1]);
}
static void execute_commande_dans_un_fils(job_t *job,int num_comm, ligne_analysee_t *ligne_analysee, struct sigaction *sig)
{
  // TODO : supprimer les lignes suivantes et compléter la procédure
   pid_t pid = fork();

  if(pid==-1){
    perror("Echec fork "); exit(errno);
  }
  if(pid==0){ // ok
    if(ligne_analysee->nb_fils == 1) {
      sig->sa_handler=SIG_DFL;
      sigaction(SIGINT,sig,NULL);
      int res = execvp(ligne_analysee->commandes[num_comm][0],ligne_analysee->commandes[num_comm]);
      if (res==-1){perror("Echec execvp 1"); exit(errno);}

    } else {

        if(num_comm==ligne_analysee->nb_fils-1){ // Dernier fils
          close(job->tubes[num_comm-1][1]);
          //lecture sortie du tube + ecriture sur stdout shell
          dup2(job->tubes[num_comm-1][0],0);//lecture
          int res= execvp(ligne_analysee->commandes[num_comm][0],ligne_analysee->commandes[num_comm]);
          if (res==-1){perror("Echec execvp 2"); exit(errno);}
          close(job->tubes[num_comm-1][0]);
        }
        //tub précédent + ecrit tube suivant FILS MILIEU FRATERIE
        else if(num_comm>0 && num_comm<ligne_analysee->nb_fils-1){
          //lire_dans_le_tube(job,num_comm);
          ecrire_dans_le_tube(job,num_comm,ligne_analysee);

        } else { // Premier Fils
          close(job->tubes[num_comm][0]);
          dup2(job->tubes[num_comm][1],1);
          int res = execvp(ligne_analysee->commandes[num_comm][0],ligne_analysee->commandes[num_comm]);
          if (res==-1){perror("Echec execvp 4"); exit(errno);}
          close(job->tubes[num_comm][1]);
        }




    }
  }else{
    job->pids[num_comm] = pid;
  }

  UNUSED(job); UNUSED(num_comm); UNUSED(ligne_analysee); UNUSED(sig);

}
/*--------------------------------------------------------------------------
 * Fait exécuter les commandes de la ligne par des fils
 * -----------------------------------------------------------------------*/
void executer_commandes(job_t *job, ligne_analysee_t *ligne_analysee, struct sigaction *sig)
{

  for(int i = 0; i < ligne_analysee->nb_fils; i++) {
    // recopie de la ligne de commande dans la structure job
    strcpy(job->nom,ligne_analysee->ligne);

    // on lance l'exécution de la commande dans un fils
    execute_commande_dans_un_fils(job,i,ligne_analysee, sig);
  }


  // TODO : à compléter
  for(int j = 0; j < ligne_analysee->nb_fils; j++) {
     waitpid(job->pids[j],NULL,0);
  }



  // on ne se sert plus de la ligne : ménage
  *ligne_analysee->ligne='\0';
}
