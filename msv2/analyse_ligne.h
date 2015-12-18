#ifndef ANALYSE_LIGNE_H
#define ANALYSE_LIGNE_H

/*--------------------------------------------------------------------------
 * Lecture de la ligne de commande
 * Utilisée dans mini_shell.c
 * -----------------------------------------------------------------------*/
void lit_ligne(ligne_analysee_t *ligne_analysee);

/*--------------------------------------------------------------------------
 * Fonction qui extrait chaque commande
 * de la ligne de commande. Renvoie le nombre de commandes lancées.
 * Utilisée dans mini_shell.c
 * -----------------------------------------------------------------------*/
int extrait_commandes(ligne_analysee_t *ligne_analysee);
#endif
