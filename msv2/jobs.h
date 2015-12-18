#ifndef JOBS_H
#define JOBS_H

/*--------------------------------------------------------------------------
 * header à inclure pour les constantes et types spéciaux
 * -----------------------------------------------------------------------*/
#include "mini_shell.h"

/*--------------------------------------------------------------------------
 * Fonction qui initialise les structures de contrôle des jobs
 * Utilisée dans mini_shell.c
 * -----------------------------------------------------------------------*/
void initialiser_jobs(job_set_t *mes_jobs);


/*--------------------------------------------------------------------------
 * Action sur un job qui est supposé exister
 * Utilisée dans internes.c
 * -----------------------------------------------------------------------*/
void action_job(int j, job_t job,int sig,const char*text);


/*--------------------------------------------------------------------------
 * Prépare un nouveau job
 * Utilisée dans mini_shell.c
 * -----------------------------------------------------------------------*/
job_t *preparer_nouveau_job(int isfg, char *ligne, job_set_t *mes_jobs);

#endif
