
#ifndef OBJECT_H
#define OBJECT_H

#define POIDS_MAX 10000 /* 10 kg,en grammes */

typedef struct  s_object
{
    char    nom[32];
    int     poids;/*poids unitaire, en grammes*/;
    int     quantite;
}   t_objet;

#endif


