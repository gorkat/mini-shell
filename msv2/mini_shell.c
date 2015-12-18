/*--------------------------------------------------------------------------
 * Programme à compléter pour écrire un mini-shell multi-jobs avec tubes
 * -----------------------------------------------------------------------*/

/*--------------------------------------------------------------------------
 * headers à inclure afin de pouvoir utiliser divers appels systèmes
 * -----------------------------------------------------------------------*/
#include <stdio.h>     // pour printf() and co
#include <stdlib.h>    // pour exit()
#include <errno.h>     // pour errno and co
#include <unistd.h>    // pour fork()
#include <sys/types.h> // pour avoir pid_t
#include <sys/wait.h>  // pour avoir wait() and co
#include <signal.h>    // pour sigaction()
#include <string.h>    // pour strrchr()

/*--------------------------------------------------------------------------
 * header à inclure pour les constantes et types spéciaux
 * -----------------------------------------------------------------------*/
#include "mini_shell.h"

/*--------------------------------------------------------------------------
 * header à inclure afin de pouvoir utiliser le parser de ligne de commande
 * -----------------------------------------------------------------------*/
#include "analyse_ligne.h" // pour avoir extrait_commandes()

/*--------------------------------------------------------------------------
 * header à inclure afin de pouvoir utiliser la gestion des jobs
 * -----------------------------------------------------------------------*/
#include "jobs.h" // pour avoir traite_[kill,stop]() + *job*()

/*--------------------------------------------------------------------------
 * header à inclure afin de pouvoir exécuter des commandes externes
 * -----------------------------------------------------------------------*/
#include "externes.h" // pour avoir executer_commandes()

/*--------------------------------------------------------------------------
 * header à inclure afin de pouvoir utiliser nos propres commandes internes
 * -----------------------------------------------------------------------*/
#include "internes.h" // pour avoir commande_interne()

/*-------------------------------------------------------------------------------
 * Macro pour éviter le warning "unused parameter" dans une version intermédiaire
 * -----------------------------------------------------------------------------*/
#define UNUSED(x) (void)(x)

/*--------------------------------------------------------------------------
 * Variable globale nécessaire pour l'utiliser dans traite_signal()
 * -----------------------------------------------------------------------*/
static job_set_t g_mes_jobs;               // pour la gestion des jobs

// Déclaration d'une fonction utilisée dans traite_signal() mais définie plus bas.
static void affiche_invite(void);

/*--------------------------------------------------------------------------
 * handler qui traite les signaux
 * -----------------------------------------------------------------------*/
static void traite_signal(int signal_recu)
{

   switch (signal_recu) {
         case (SIGINT) :
           printf("\n");
           affiche_invite();
           break;
         case (SIGCHLD) :
           // c'est un signal attendu mais on a rien à afficher !
           break;
  default:
       printf("Signal inattendu\n");
  }
}

/*--------------------------------------------------------------------------
 * fonction d'initialisation de la structure de contrôle des signaux
 * -----------------------------------------------------------------------*/
static void initialiser_gestion_signaux(struct sigaction *sig)
{
   sig->sa_flags=0;
   sigemptyset(&sig->sa_mask);

   sig->sa_handler = traite_signal;
   sigaction(SIGINT,sig,NULL);

   UNUSED(traite_signal);
}

/*--------------------------------------------------------------------------
 * fonction d'affichage du prompt
 * -----------------------------------------------------------------------*/
static void affiche_invite()
{
   // TODO à modifier : insérer le nom du répertoire courant
   char* path = get_current_dir_name();
   char* nom = strrchr(path, '/');
   printf("%s", nom);
   free(path);
   printf("> ");
   fflush(stdout);
}

/*--------------------------------------------------------------------------
 * Analyse de la ligne de commandes et
 * exécution des commandes de façon concurrente
 * -----------------------------------------------------------------------*/
static void execute_ligne(ligne_analysee_t *ligne_analysee, job_set_t *mes_jobs, struct sigaction *sig)
{
   job_t *j;

   // on extrait les commandes présentes dans la ligne de commande
   // et l'on détermine si elle doit être exécutée en avant-plan

   int isfg=extrait_commandes(ligne_analysee);

   // s'il ne s'agit pas d'une commande interne au shell,
   // la ligne est exécutée par un ou des fils
   if (! commande_interne(ligne_analysee,mes_jobs) )
   {
      // trouve l'adresse d'une structure libre pour lui associer le job à exécuter
      j=preparer_nouveau_job(isfg,ligne_analysee->ligne,mes_jobs);

         // TODO : à compléter
         sig->sa_handler=SIG_IGN;
         sigaction(SIGINT,sig,NULL);

         // fait exécuter les commandes de la ligne par des fils
         executer_commandes(j,ligne_analysee, sig);

      // TODO : à compléter
      sig->sa_handler=traite_signal;
      sigaction(SIGINT,sig,NULL);
  }

  // ménage
  *ligne_analysee->ligne='\0';
}

/*--------------------------------------------------------------------------
 * Fonction principale du mini-shell
 * -----------------------------------------------------------------------*/
int main(void)
{
   ligne_analysee_t m_ligne_analysee;  // pour l'analyse d'une ligne de commandes
   struct sigaction m_sig;             // structure sigaction pour gérer les signaux

   // initialise les structures de contrôle des jobs
   initialiser_jobs(&g_mes_jobs);

   // initialise la structure de contrôle des signaux
   initialiser_gestion_signaux(&m_sig);

   while(1)
   {
      affiche_invite();
      lit_ligne(&m_ligne_analysee);
      execute_ligne(&m_ligne_analysee,&g_mes_jobs,&m_sig);
   }
   return EXIT_SUCCESS;
}
