#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "objet.h"

#define BUF_SIZE 64
#define NOM_MAX 31
#define VALEUR_MAX 1000000

/* ---------- Utilitaires (ecrits a la main, pas de string.h) ---------- */

static int	ft_strcmp(const char *a, const char *b)
{
	int	i;

	i = 0;
	while (a[i] != '\0' && a[i] == b[i])
		i++;
	return ((unsigned char)a[i] - (unsigned char)b[i]);
}

static int	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

/* Lit une ligne avec fgets, retire le '\n', vide le reste si la ligne
   est trop longue. Retourne 0 en cas de fin de fichier (EOF). */
static int	lire_ligne(char *buf, int taille)
{
	int	i;
	int	c;

	if (fgets(buf, taille, stdin) == NULL)
		return (0);
	i = 0;
	while (buf[i] != '\0' && buf[i] != '\n')
		i++;
	if (buf[i] == '\n')
		buf[i] = '\0';
	else
	{
		c = getchar();
		while (c != '\n' && c != EOF)
			c = getchar();
	}
	return (1);
}

/* Convertit une chaine en entier positif. Retourne 0 si invalide. */
static int	lire_entier(const char *s, int *out)
{
	int	i;
	int	val;

	i = 0;
	val = 0;
	while (s[i] == ' ')
		i++;
	if (s[i] < '0' || s[i] > '9')
		return (0);
	while (s[i] >= '0' && s[i] <= '9')
	{
		val = val * 10 + (s[i] - '0');
		if (val > VALEUR_MAX)
			return (0);
		i++;
	}
	while (s[i] == ' ')
		i++;
	if (s[i] != '\0')
		return (0);
	*out = val;
	return (1);
}

/* Demande un entier compris entre 1 et POIDS_MAX. Retourne 0 si echec. */
static int	demander_entier(const char *question, int *out)
{
	char	buf[BUF_SIZE];

	printf("%s", question);
	if (!lire_ligne(buf, BUF_SIZE))
		return (0);
	if (!lire_entier(buf, out) || *out < 1 || *out > POIDS_MAX)
	{
		printf("Valeur invalide (entier entre 1 et %d attendu).\n",
			POIDS_MAX);
		return (0);
	}
	return (1);
}

/* Demande un nom (1 a 31 caracteres). Retourne 0 si echec. */
static int	demander_nom(char *nom)
{
	char	buf[BUF_SIZE];
	int		i;

	printf("Nom de l'objet : ");
	if (!lire_ligne(buf, BUF_SIZE))
		return (0);
	if (buf[0] == '\0')
	{
		printf("Le nom ne peut pas etre vide.\n");
		return (0);
	}
	if (ft_strlen(buf) > NOM_MAX)
	{
		printf("Nom trop long (%d caracteres maximum).\n", NOM_MAX);
		return (0);
	}
	i = 0;
	while (buf[i] != '\0')
	{
		nom[i] = buf[i];
		i++;
	}
	nom[i] = '\0';
	return (1);
}

/* ---------- Logique de l'inventaire ---------- */

static int	poids_actuel(t_list *inv)
{
	int		total;
	t_objet	*obj;

	total = 0;
	while (inv != NULL)
	{
		obj = (t_objet *)inv->content;
		total += obj->poids * obj->quantite;
		inv = inv->next;
	}
	return (total);
}

static t_objet	*chercher_objet(t_list *inv, const char *nom)
{
	t_objet	*obj;

	while (inv != NULL)
	{
		obj = (t_objet *)inv->content;
		if (ft_strcmp(obj->nom, nom) == 0)
			return (obj);
		inv = inv->next;
	}
	return (NULL);
}

static void	afficher_objet(void *content)
{
	t_objet	*obj;

	obj = (t_objet *)content;
	printf("%-31s | %6d g | x%-5d | %7d g\n",
		obj->nom, obj->poids, obj->quantite, obj->poids * obj->quantite);
}

static t_objet	*creer_objet(const char *nom, int poids, int quantite)
{
	t_objet	*obj;
	int		i;

	obj = malloc(sizeof(t_objet));
	if (obj == NULL)
		return (NULL);
	i = 0;
	while (nom[i] != '\0')
	{
		obj->nom[i] = nom[i];
		i++;
	}
	obj->nom[i] = '\0';
	obj->poids = poids;
	obj->quantite = quantite;
	return (obj);
}

static void	ajouter_objet(t_list **inv, const char *nom, int poids,
				int quantite)
{
	t_objet	*obj;
	t_list	*node;

	obj = creer_objet(nom, poids, quantite);
	if (obj == NULL)
	{
		printf("Erreur d'allocation memoire.\n");
		return ;
	}
	node = ft_lstnew(obj);
	if (node == NULL)
	{
		free(obj);
		printf("Erreur d'allocation memoire.\n");
		return ;
	}
	ft_lstadd_back(inv, node);
	printf("'%s' ajoute a l'inventaire.\n", nom);
}

static void	ramasser(t_list **inv)
{
	char	nom[NOM_MAX + 1];
	int		poids;
	int		quantite;
	int		depassement;
	t_objet	*obj;

	if (!demander_nom(nom))
		return ;
	if (!demander_entier("Poids unitaire (en grammes) : ", &poids))
		return ;
	if (!demander_entier("Quantite : ", &quantite))
		return ;
	/* Verification du poids AVANT toute allocation */
	depassement = poids_actuel(*inv) + poids * quantite - POIDS_MAX;
	if (depassement > 0)
	{
		printf("Refuse : la limite de %d g serait depassee de %d g.\n",
			POIDS_MAX, depassement);
		return ;
	}
	obj = chercher_objet(*inv, nom);
	if (obj == NULL)
		ajouter_objet(inv, nom, poids, quantite);
	else if (obj->poids != poids)
		printf("'%s' existe deja avec un autre poids (%d g).\n",
			nom, obj->poids);
	else
	{
		obj->quantite += quantite;
		printf("Quantite de '%s' augmentee (total : %d).\n",
			nom, obj->quantite);
	}
}

static void	jeter(t_list **inv)
{
	char	nom[NOM_MAX + 1];
	t_list	**cur;
	t_list	*tmp;

	if (!demander_nom(nom))
		return ;
	cur = inv;
	while (*cur != NULL)
	{
		if (ft_strcmp(((t_objet *)(*cur)->content)->nom, nom) == 0)
		{
			tmp = *cur;
			*cur = tmp->next;
			ft_lstdelone(tmp, free);
			printf("'%s' a ete jete.\n", nom);
			return ;
		}
		cur = &(*cur)->next;
	}
	printf("Aucun objet nomme '%s' dans l'inventaire.\n", nom);
}

static void	lister(t_list *inv)
{
	if (inv == NULL)
	{
		printf("Inventaire vide.\n");
		return ;
	}
	printf("%-31s | %8s | %-6s | %9s\n",
		"Nom", "Poids u.", "Qte", "Total");
	ft_lstiter(inv, afficher_objet);
	printf("Poids total porte : %d g / %d g\n", poids_actuel(inv), POIDS_MAX);
}

static void	afficher_menu(void)
{
	printf("\n=== INVENTAIRE RPG ===\n");
	printf("1. Ramasser un objet\n");
	printf("2. Jeter un objet\n");
	printf("3. Lister l'inventaire\n");
	printf("4. Quitter\n");
	printf("Choix : ");
}

int	main(void)
{
	t_list	*inv;
	char	buf[BUF_SIZE];
	int		choix;

	inv = NULL;
	choix = 0;
	while (choix != 4)
	{
		afficher_menu();
		if (!lire_ligne(buf, BUF_SIZE))
			break ;
		if (!lire_entier(buf, &choix))
			choix = 0;
		if (choix == 1)
			ramasser(&inv);
		else if (choix == 2)
			jeter(&inv);
		else if (choix == 3)
			lister(inv);
		else if (choix != 4)
			printf("Choix invalide.\n");
	}
	ft_lstclear(&inv, free);
	return (0);
}
