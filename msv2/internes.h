#ifndef MES_COMMANDES_H
#define MES_COMMANDES_H

/*--------------------------------------------------------------------------
 * Exécute la commande selon une commande interne si elle existe
 * Utilisée dans mini_shell.c
 * -----------------------------------------------------------------------*/
int commande_interne(ligne_analysee_t *ligne_analysee, job_set_t *mes_jobs);

#endif
