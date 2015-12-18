/*--------------------------------------------------------------------------
 * headers à inclure afin de pouvoir utiliser divers appels systèmes
 * -----------------------------------------------------------------------*/
#include <stdio.h>   // pour printf and co
#include <stdlib.h>  // pour exit
#include <ctype.h>   // pour avoir isspace and co
#include <string.h>  // pour avoir strcmp and co

/*--------------------------------------------------------------------------
 * headers à inclure pour le minishell
 * -----------------------------------------------------------------------*/
#include "mini_shell.h"
#include "externes.h"
#include "analyse_ligne.h"

/*--------------------------------------------------------------------------
 * Lecture de la ligne de commande
 * -----------------------------------------------------------------------*/
void lit_ligne(ligne_analysee_t *ligne_analysee)
{
   // lecture de la ligne de commande
   // si un fils en arrière-plan est terminé, je dois relancer le fgets
   while (! fgets(ligne_analysee->ligne,sizeof(ligne_analysee->ligne)-1,stdin) )
   {
     // si Ctrl-d est tapé (i.e. EOF), alors on sort du programme
     if (feof(stdin))
     {
       printf("\n");
       exit(0);
     }
     // sinon le NULL provient d'une erreur : fgets interropmu par sigchld
   }
}

/*--------------------------------------------------------------------------
 * fonction d'analyse de la ligne de commande : extrait la commande numéro num_comm
 * -----------------------------------------------------------------------*/
static int decoupe_commande(char **a_debut, ligne_analysee_t *ligne_analysee)
{
   int isfg=1;
   int i;

   // début de la commande numéro num_comm à découper
   char *debut = *a_debut;

   // on met le mot numéro i dans la case numéro i de commandes[num_comm]
   for (i=0; i<NB_MAX_MOTS-1 ;i++) // on laisse la place pour le NULL final
   {
      /* saute les espaces */
      while (*debut && isspace(*debut)) debut++;

      /* fin de ligne ? */
      if (!*debut)
          break;

      /* pipe inattendu ? */
      if (*debut=='|') {
         printf("Erreur : caractère | inattendu\n"); exit(EXIT_FAILURE);
      }

      /* stocke le début du mot numéro i*/
      ligne_analysee->commandes[ligne_analysee->nb_fils][i]=debut;

      /* cherche la fin du mot numéro i*/
      while (*debut && !isspace(*debut) && *debut != '|') debut++;

      /* est-on entre deux arguments? */
      if (*debut && isspace(*debut)) {
          *debut='\0';
          debut++;
         while (*debut && isspace(*debut)) debut++;
      }

      /* est-on entre deux commandes? */
      if (*debut == '|') {
          *debut='\0';
          debut++; i++;
         break;
      }

   }/* fin du parcours des arguments de la commande numéro num_comm*/

   /* renvoie où l'on s'est arrêté*/
   *a_debut = debut;

   /* on n'oublie pas l'élément NULL pour execvp */
   ligne_analysee->commandes[ligne_analysee->nb_fils][i] = NULL;

   /* pour tester si le dernier mot est &*/
   if (i>0 && strcmp("&",ligne_analysee->commandes[ligne_analysee->nb_fils][i-1])==0)
   {
     ligne_analysee->commandes[ligne_analysee->nb_fils][i-1] = NULL;
     isfg=0;
   }

   return isfg;
}

/*--------------------------------------------------------------------------
 * fonction qui extrait chaque commande de la ligne de commande.
 * -----------------------------------------------------------------------*/
int extrait_commandes(ligne_analysee_t *ligne_analysee)
{
   int isfg=1;
   char* debut = ligne_analysee->ligne;

   ligne_analysee->nb_fils=0;
   while (ligne_analysee->nb_fils<NB_MAX_COMMANDES && *debut)
   {
      isfg=decoupe_commande(&debut,ligne_analysee);
      ligne_analysee->nb_fils++;
   }

   return isfg;
}
