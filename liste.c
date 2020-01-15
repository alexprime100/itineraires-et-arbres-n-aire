#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "liste.h"

ville* creer_ville(char * nom_ville, char* nom_voisin, double distance, double temps)
{
	ville* vill = NULL;
	vill = malloc(sizeof(ville));
	vill->nom = malloc(17);
	vill->nom = nom_ville;
	vill->voisin = malloc(17);
	vill->voisin = nom_voisin;
	vill->distance = distance;
	vill->duree = temps;
	vill->suivant = NULL;
	return vill;
}

liste_ville* creer_liste_ville(char* nom_ville)
{
    liste_ville* liste = NULL;
    liste = malloc(sizeof(liste_ville));
    liste->nom = malloc(17);
    liste->nom = nom_ville;
    liste->next = NULL;

    return liste;
}

double puissance(double nombre, int exposant)
{
    double resultat = 1;
    for (int i = 1; i <= exposant; i++)
    {
        resultat = resultat * nombre;
    }
    return resultat;
}

double conversion_char_double(char* nombre)
{
	//[0;9] <=> [48;57]
	int size = strlen(nombre);
	double distance = 0;
	for (int i = 0; i < size; i++)
	{
		distance += ((int)nombre[i] - 48) * puissance(10, size - 1 - i);
	}
	return distance;
}

double conversion_char_heure(char* c_heure)
{
	//[0;9] <=> [48;57]
	int* tab = malloc(sizeof(int) * 4);
	int index = 0;
	for (int i = 0; i < 5; i++)
	{
		if ((int)c_heure[i] >= 48 && (int)c_heure[i] <= 57)   //le caractere ':' n'est pas un chiffre, il faut l'exclure
		{
			tab[index] = (int)c_heure[i] - 48;
			index++;
		}
	}
	//c_heure a ete converti en un tableau de int sans le caractere ':'
	double heure = 0;
	double minute = 0;
	heure += tab[0] * 10 + tab[1];
	minute += tab[2] * 10 + tab[3];
	minute /= 60;
	heure += minute;
	return heure;
}

bool comparaison_chaine(char* chaine1, char* chaine2)
{
	int size1;
	int size2;
	if (chaine1 == NULL)
    {
        size1 = 0;
    }
    else
    {
        size1 = strlen(chaine1);
    }
    if (chaine2 == NULL)
    {
        size2 = 0;
    }
    else
    {
        size2 = strlen(chaine2);
    }
	if (size1 == size2)
	{
		for (int i = 0; i < size1; i++)
		{
			if (chaine1[i] != chaine2[i])
			{
				return false;
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}

bool comparaison2(char* chaine, liste_ville* liste)
{
    liste_ville* liste_temp = liste;
    while (liste_temp != NULL)
    {
        if (comparaison_chaine(chaine, liste_temp->nom))
        {
            return true;
        }
        else
        {
            liste_temp = liste_temp->next;
        }

        suppression_liste(liste_temp);

    }
    return false;
}

ville* insertion_tete(char * nom_ville, char* nom_voisin, double distance, double temps, ville* reseau)
{
	ville* vill = creer_ville(nom_ville, nom_voisin, distance, temps);
	if (vill != NULL)
	{
		vill->suivant = reseau;
		return vill;
	}
	else
	{
		printf("echec creation ville");
		return NULL;
	}
}

ville* insertion_queue(char * nom_ville, char* nom_voisin, double distance, double temps, ville* reseau)
{
    ville* vill = creer_ville(nom_ville, nom_voisin, distance, temps);
	if (reseau != NULL)
    {
        ville* reseau_temp = reseau;
        while (reseau_temp->suivant != NULL)
        {
            reseau_temp = reseau_temp->suivant;
        }
        reseau_temp->suivant = vill;
        return reseau;
    }
    else
    {
        return vill;
    }
}

liste_ville* insertion_liste_ville(char* nom, liste_ville* ancetres)
{
    liste_ville* liste = creer_liste_ville(nom);
    if (liste != NULL)
    {
        liste->next = ancetres;
        return liste;
    }
    else
    {
        return NULL;
    }
}

liste_ville* suppression_liste(liste_ville* liste)
{
    if (liste == NULL || liste->next == NULL)
    {
        return NULL;
    }
    else
    {
        return liste->next;
    }
}

void affichage(ville* reseau)
{
	ville* reseau_temp = reseau;
	while (reseau_temp != NULL)
	{
		printf("%s <-> %s : %g km en %.2f h\n", reseau_temp->nom, reseau_temp->voisin, reseau_temp->distance, reseau_temp->duree);
		reseau_temp = reseau_temp->suivant;
	}
}

ville* suppression_voisin(ville* reseau)
{
	if (reseau == NULL || reseau->suivant == NULL)
	{
		return NULL;
	}
	return reseau->suivant;
}

void suppression_totale(ville* reseau)
{
	while (reseau != NULL)
	{
		reseau = suppression_voisin(reseau);
	}
}

ville** remplissage_tableau_ville(ville** tab_ville, const int nb_ville)
{
	for (int i = 0; i < nb_ville; i++)
	{
		tab_ville[i] = NULL;
	}
	FILE* fichier = NULL;
	int caractereActuel = 0;

	fichier = fopen("connexions.csv", "r");
	int index_ville = 0;

	if (fichier != NULL)
	{
		do
		{
			caractereActuel = fgetc(fichier);
		} while (caractereActuel != EOF && caractereActuel != '\n');    //on attend le 1er retour a la ligne afin d'exclure la ligne 1 du fichier

		//on parcourt chaque ligne du fichier
		bool continuer = true;
		caractereActuel = fgetc(fichier);
		while (continuer)
		{

			char* nom_ville = malloc(17);

			nom_ville[0] = caractereActuel;
			int index_nom = 1;
			while (caractereActuel != ',')   //on determine le nom de la ville
			{
			    caractereActuel = fgetc(fichier);
                if (caractereActuel != ',')
                {
                    nom_ville[index_nom] = caractereActuel;
                    index_nom++;
                }

			}
			nom_ville[index_nom] = '\0';


			char* nom_voisin = malloc(17);
			int index_voisin = 0;

			caractereActuel = fgetc(fichier);
			while (caractereActuel != ',')   // on determine le nom de la ville voisine
			{
				nom_voisin[index_voisin] = caractereActuel;
				caractereActuel = fgetc(fichier);
				index_voisin++;

			}
			nom_voisin[index_voisin] = '\0';

			char* trajet = malloc(4);    //les distances possedent toutes au plus 3 chiffres
			caractereActuel = fgetc(fichier);
			int index_trajet = 0;
			while (caractereActuel != ',')
			{
				trajet[index_trajet] = caractereActuel;
				caractereActuel = fgetc(fichier);
				index_trajet++;
			}
			trajet[index_trajet] = '\0';
			double distance = conversion_char_double(trajet);

			char* temps = malloc(5);
			caractereActuel = fgetc(fichier);
			int index_temps = 0;
			while (caractereActuel != '\n')
			{
				temps[index_temps] = caractereActuel;
				caractereActuel = fgetc(fichier);
				index_temps++;
			}
			temps[index_temps] = '\0';
			double duree = conversion_char_heure(temps);



			if (tab_ville[index_ville] == NULL)
			{
				tab_ville[index_ville] = insertion_queue(nom_ville, nom_voisin, distance, duree, tab_ville[index_ville]);
			}
			else
			{
				bool identite = comparaison_chaine(tab_ville[index_ville]->nom, nom_ville);

				if (identite == false)
				{
					index_ville++;
					tab_ville[index_ville] = insertion_queue(nom_ville, nom_voisin, distance, duree, tab_ville[index_ville]);
				}
				else
				{
					tab_ville[index_ville] = insertion_queue(nom_ville, nom_voisin, distance, duree, tab_ville[index_ville]);
				}
			}
            caractereActuel = fgetc(fichier);
            if (caractereActuel == '|')
            {
                continuer = false;
            }
		}
	}
	else
	{
		printf("le fichier CSV n'existe pas , assurez vous qu'il soit dans le meme dossier que le code source");
	}
	return tab_ville;
}

trajet* acceder_trajet(trajet* chemin, int k)
{
    trajet* chemin_temp = chemin;
    int i = 0;
    while (chemin_temp != NULL && i < k)
    {
        i++;
        chemin_temp = chemin_temp->next;
    }
    if (i == k)
    {
        return chemin_temp;
    }
    else
    {
        return NULL;
    }
}

noeud* creer_noeud(ville* vill)
{
    noeud* nod = malloc(sizeof(noeud));
    nod->reseau = vill;
    nod->trajet = 0;
    nod->temps = 0;
    nod->ancetres = creer_liste_ville(nod->reseau->nom);
    nod->pere = NULL;
    nod->fils1 = NULL;
    nod->fils2 = NULL;
    nod->fils3 = NULL;
    nod->fils4 = NULL;
    nod->fils5 = NULL;

    return nod;
}

noeud* association_fils(noeud* parent, ville** tab_ville)
{
    ville** enfant = malloc(sizeof(ville*) * 5);
    ville* ville_temp = parent->reseau;

    for (int i = 0; i < 5; i++)
    {
        enfant[i] = NULL;
    }
    if (parent != NULL)
    {
        if (parent->fils1 == NULL)
        {
            int index_fils = 0;
            while (ville_temp != NULL)
            {

                bool identite = comparaison2(ville_temp->voisin,parent->ancetres);
                if (identite == false)    //si la ville selectionnee est difference de toutes les villes ascendantes
                {
                    int index_ville = 0;
                    bool identite2 = comparaison_chaine(ville_temp->voisin, tab_ville[index_ville]->nom);   //on cherche l'index dans tab_ville de la ville selectionnee dans

                    while (identite2 == false)    //s'il ne s'agit pas du bon index, on l'incremente a nouveau
                    {
                        index_ville++;
                        identite2 = comparaison_chaine(ville_temp->voisin, tab_ville[index_ville]->nom);
                    }
                    enfant[index_fils] = tab_ville[index_ville];
                }

                index_fils++;   //on passe a la ville voisine suivante
                ville_temp = ville_temp->suivant;
            }

            //allocation memoire de chaque fils
            parent->fils1 = malloc(sizeof(noeud));
            parent->fils2 = malloc(sizeof(noeud));
            parent->fils3 = malloc(sizeof(noeud));
            parent->fils4 = malloc(sizeof(noeud));
            parent->fils5 = malloc(sizeof(noeud));

            //affectation des villes pour chaque fils
            parent->fils1->reseau = enfant[0];
            parent->fils2->reseau = enfant[1];
            parent->fils3->reseau = enfant[2];
            parent->fils4->reseau = enfant[3];
            parent->fils5->reseau = enfant[4];

            //pour chaque fils non NULL, on lui affecte ses villes ascendantes (ancetres), son nombre d'ancetres, sa distance et sa duree a la ville de depart (trajet et temps)
            if (parent->fils1->reseau != NULL)
            {
                parent->fils1->ancetres = parent->ancetres;
                parent->fils1->ancetres = insertion_liste_ville(parent->fils1->reseau->nom, parent->fils1->ancetres);
                parent->fils1->pere = parent;
                parent->fils1->trajet = parent->fils1->reseau->distance + parent->trajet;
                parent->fils1->temps = parent->fils1->reseau->duree + parent->temps;
            }

            if (parent->fils2->reseau != NULL)
            {
                parent->fils2->ancetres = parent->ancetres;
                parent->fils2->ancetres = insertion_liste_ville(parent->fils2->reseau->nom, parent->fils2->ancetres);
                parent->fils2->pere = parent;
                parent->fils2->trajet = parent->fils2->reseau->distance + parent->trajet;
                parent->fils2->temps = parent->fils2->reseau->duree + parent->temps;
            }

            if (parent->fils3->reseau != NULL)
            {
                parent->fils3->ancetres = parent->ancetres;
                parent->fils3->ancetres = insertion_liste_ville(parent->fils3->reseau->nom, parent->fils3->ancetres);
                parent->fils3->pere = parent;
                parent->fils3->trajet = parent->fils3->reseau->distance + parent->trajet;
                parent->fils3->temps = parent->fils3->reseau->duree + parent->temps;
            }

            if (parent->fils4->reseau != NULL)
            {
                parent->fils4->ancetres = parent->ancetres;
                parent->fils4->ancetres = insertion_liste_ville(parent->fils4->reseau->nom, parent->fils4->ancetres);
                parent->fils4->pere = parent;
                parent->fils4->trajet = parent->fils4->reseau->distance + parent->trajet;
                parent->fils4->temps = parent->fils4->reseau->duree + parent->temps;
            }

            if (parent->fils5->reseau != NULL)
            {
                parent->fils5->ancetres = parent->ancetres;
                parent->fils5->ancetres = insertion_liste_ville(parent->fils5->reseau->nom, parent->fils5->ancetres);
                parent->fils5->pere = parent;
                parent->fils5->trajet = parent->fils5->reseau->distance + parent->trajet;
                parent->fils5->temps = parent->fils5->reseau->duree + parent->temps;
            }
        }
    }
    return parent;
}

void decalage(int offset)
{
    for (int i = 0; i < offset; i++)
    {
        printf("  ");
    }
    printf("|-");
}

void _affichage_reseau(noeud* racine, int offset)
{

    if (racine->reseau != NULL)
    {
        if (offset > 0)
        {
            decalage(offset);
        }
        printf("%s\n", racine->reseau->nom);
    }

    if (racine != NULL)
    {
        if (racine->fils1 != NULL)
        {
            _affichage_reseau(racine->fils1, offset + 1);
        }

        if (racine->fils2 != NULL)
        {
            _affichage_reseau(racine->fils2, offset + 1);
        }

        if (racine->fils3 != NULL)
        {
            _affichage_reseau(racine->fils3, offset + 1);
        }

        if (racine->fils4 != NULL)
        {
            _affichage_reseau(racine->fils4, offset + 1);
        }

        if (racine->fils5 != NULL)
        {
            _affichage_reseau(racine->fils5, offset + 1);
        }
    }
    else
    {
        decalage(offset+1);
        printf("NULL\n");
    }
}

void affichage_reseau(noeud* racine)
{
    _affichage_reseau(racine, 0);
}

void affichage_noeud(noeud* racine, int offset)
{
    if (offset > 0)
    {
        decalage(offset);
    }
    printf("%s\n",racine->reseau->nom);
    if (racine != NULL)
    {
        if (racine->fils1->reseau != NULL)
        {
            decalage(offset+1);
            printf("%s\n", racine->fils1->reseau->nom);
        }
        if (racine->fils2->reseau != NULL)
        {
            decalage(offset+1);
            printf("%s\n", racine->fils2->reseau->nom);
        }
        if (racine->fils3->reseau != NULL)
        {
            decalage(offset+1);
            printf("%s\n", racine->fils3->reseau->nom);
        }
        if (racine->fils4->reseau != NULL)
        {
            decalage(offset+1);
            printf("%s\n", racine->fils4->reseau->nom);
        }
        if (racine->fils5->reseau != NULL)
        {
            decalage(offset+1);
            printf("%s\n", racine->fils5->reseau->nom);
        }
    }
}

noeud* _remplissage_arbre(noeud* racine, ville** tab_ville, ville* arrivee, int hauteur, int compteur)
{
    racine = association_fils(racine, tab_ville);
    if (racine->fils1 != NULL)
    {
        if (racine->fils1->reseau != NULL)
        {
            if (comparaison_chaine(racine->fils1->reseau->nom, arrivee->nom) == false &&  compteur < hauteur )
            {
                racine->fils1 = _remplissage_arbre(racine->fils1, tab_ville, arrivee, hauteur, compteur + 1);
            }
        }
    }
    if (racine->fils2 != NULL)
    {
        if (racine->fils2->reseau != NULL)
        {
            if (comparaison_chaine(racine->fils2->reseau->nom, arrivee->nom) == false &&  compteur < hauteur)
            {
                racine->fils2 = _remplissage_arbre(racine->fils2, tab_ville, arrivee, hauteur, compteur + 1);
            }
        }
    }
    if (racine->fils3 != NULL)
    {
        if (racine->fils3->reseau != NULL)
        {
            if (comparaison_chaine(racine->fils3->reseau->nom, arrivee->nom) == false &&  compteur < hauteur)
            {
                racine->fils3 = _remplissage_arbre(racine->fils3, tab_ville, arrivee, hauteur, compteur + 1);
            }
        }
    }
    if (racine->fils4 != NULL)
    {
        if (racine->fils4->reseau != NULL)
        {
            if (comparaison_chaine(racine->fils4->reseau->nom, arrivee->nom) == false &&  compteur < hauteur)
            {
                racine->fils4 = _remplissage_arbre(racine->fils4, tab_ville, arrivee, hauteur, compteur + 1);
            }
        }
    }
    if (racine->fils5 != NULL)
    {
        if (racine->fils5->reseau != NULL)
        {
            if (comparaison_chaine(racine->fils5->reseau->nom, arrivee->nom) == false &&  compteur < hauteur)
            {
                racine->fils5 = _remplissage_arbre(racine->fils5, tab_ville, arrivee, hauteur, compteur + 1);
            }
        }
    }


    return racine;
}

noeud* remplissage_arbre(noeud* racine, ville** tab_ville, ville* arrivee, int hauteur)
{
    return _remplissage_arbre(racine, tab_ville, arrivee,  hauteur, 1);
}

noeud* suppr(noeud* arbre)
{
    arbre = NULL;
    return arbre;
}

int _nombre_trajets(noeud* arbre, ville* arrivee, int compteur)
{
    if (arbre != NULL)
    {
        if (arbre->reseau != NULL)
        {
            compteur = _nombre_trajets(arbre->fils1, arrivee, compteur);
            compteur = _nombre_trajets(arbre->fils2, arrivee, compteur);
            compteur = _nombre_trajets(arbre->fils3, arrivee, compteur);
            compteur = _nombre_trajets(arbre->fils4, arrivee, compteur);
            compteur = _nombre_trajets(arbre->fils5, arrivee, compteur);

            if (comparaison_chaine(arbre->reseau->nom, arrivee->nom))
            {
                compteur++;
            }
        }
    }
    return compteur;
}

int nombre_trajets(noeud* arbre, ville* arrivee)
{
    return _nombre_trajets(arbre, arrivee, 0);
}

trajet* creer_trajet(noeud* feuille)
{
    trajet* traj = NULL;
    traj = malloc(sizeof(trajet));
    traj->arrivee = feuille->reseau->nom;
    if (feuille->pere == NULL)
    {
        traj->depart = NULL;
    }
    else
    {
        traj->depart = feuille->pere->reseau->nom;
    }
    traj->distance = feuille->trajet;
    traj->duree = feuille->temps;
    traj->next = NULL;
    return traj;
}

trajet* insertion_trajet(trajet* chemin, noeud* feuille)
{
    trajet* traj = creer_trajet(feuille);
    if (chemin != NULL)
    {
        trajet* chemin_temp = chemin;
        while(chemin_temp->next != NULL)
        {
            chemin_temp = chemin_temp->next;
        }
        chemin_temp->next = traj;
        return chemin;
    }
    else
    {
        return traj;
    }
}

trajet* creer_liste_trajet(noeud* feuille, trajet* chemin)
{
    if (feuille != NULL)
    {
        noeud* feuille_temp = feuille;
        while (feuille_temp->pere != NULL)
        {
            chemin = insertion_trajet(chemin, feuille_temp);
            feuille_temp = feuille_temp->pere;
        }
    }
    else
    {
        return NULL;
    }
    return chemin;
}

int remplissage_tableau_trajets(noeud* arbre, trajet** tab_trajets, ville* arrivee, int compteur)
{
    if (arbre != NULL)
    {
        if (arbre->reseau != NULL)
        {
            compteur = remplissage_tableau_trajets(arbre->fils1, tab_trajets, arrivee, compteur);
            compteur = remplissage_tableau_trajets(arbre->fils2, tab_trajets, arrivee, compteur);
            compteur = remplissage_tableau_trajets(arbre->fils3, tab_trajets, arrivee, compteur);
            compteur = remplissage_tableau_trajets(arbre->fils4, tab_trajets, arrivee, compteur);
            compteur = remplissage_tableau_trajets(arbre->fils5, tab_trajets, arrivee, compteur);

            if (comparaison_chaine(arbre->reseau->nom, arrivee->nom))
            {
                trajet* chemin = NULL;
                chemin = creer_liste_trajet(arbre, chemin);
                tab_trajets[compteur] = chemin;
                compteur++;
            }
        }
    }
    return compteur;
}

int remp(int* tab, const int taille)
{
    for (int i = 0; i < taille; i++)
    {
        tab[i] = i + 1;
    }
    return tab[taille-1];
}

void meilleur_trajet(trajet** tab_trajets, int taille, int choix)
{
    int index_court = 0;
    int index_rapide = 0;
    double distance_min = tab_trajets[0]->distance;
    double duree_min = tab_trajets[0]->duree;
    for (int i = 1; i < taille; i++)
    {
        if (tab_trajets[i]->distance < distance_min)
        {
            index_court = i;
            distance_min = tab_trajets[i]->distance;
        }
        if (tab_trajets[i]->duree < duree_min)
        {
            index_rapide = i;
            duree_min = tab_trajets[i]->duree;
        }
    }

    int taille_trajet_court = 0;
    int taille_trajet_rapide = 0;
    trajet* trajet_rapide = tab_trajets[index_rapide];
    trajet* trajet_court = tab_trajets[index_court];

    while (trajet_court != NULL)
    {
        taille_trajet_court++;
        trajet_court = trajet_court->next;
    }
    while (trajet_rapide != NULL)
    {
        taille_trajet_rapide++;
        trajet_rapide = trajet_rapide->next;
    }

    if (choix == 3)
    {
        printf("le trajet le plus court fait passer par : ");
        for (int i = taille_trajet_court - 1; i >= 0; i--)
        {
            printf("%s, ", acceder_trajet(tab_trajets[index_court], i)->arrivee);
        }
        printf("\nil fait %gkm de long, et dure %gh\n\n",tab_trajets[index_court]->distance, tab_trajets[index_court]->duree);
    }

    if (choix == 4)
    {
        printf("le trajet le plus rapide fait passer par : ");
        for (int i = taille_trajet_rapide - 1; i >= 0; i--)
        {
            printf("%s, ", acceder_trajet(tab_trajets[index_rapide], i)->arrivee);
        }
        printf("\nil fait %gkm de long, et dure %gh\n\n",tab_trajets[index_rapide]->distance, tab_trajets[index_rapide]->duree);
    }


}

void programme()
{
    const int nb_ville = 23;
	ville** tab_ville = malloc(sizeof(ville*) * nb_ville);
	tab_ville = remplissage_tableau_ville(tab_ville, nb_ville);
	for (int i = 0; i < nb_ville; i++)
    {
        printf("%d : %s\n", i, tab_ville[i]->nom);
    }
    int depart;
    int arrivee;

    while (depart < 0 || depart > 22)
    {
        printf("saisissez le numero de la ville de depart (entre 0 et 22)\n");
        printf("depart : ");
        scanf("%d",&depart);
    }
    printf(" -> %s\n",tab_ville[depart]->nom);
    while (arrivee < 0 || arrivee > 22)
    {
        printf("saisissez le numero de la ville d'arrivee (entre 0 et 22)\n");
        printf("arrivee : ");

        scanf("%d",&arrivee);
    }
    printf(" -> %s\n", tab_ville[arrivee]->nom);

    printf("quel nombre maximal d'etapes acceptez-vous ?");
    int hauteur;
    scanf("%d", &hauteur);
    while (hauteur < 1)
    {
        printf("il est inutile de chercher un itineraire comprenant moins d'1 etape, ressaisissez ce nombre");
        scanf("%d",&hauteur);
    }
    noeud* racine = creer_noeud(tab_ville[depart]);

    racine = remplissage_arbre(racine, tab_ville, tab_ville[arrivee], hauteur);

    int nb_trajets = nombre_trajets(racine, tab_ville[arrivee]);
    printf("%d trajets vers %s trouves\n", nb_trajets, tab_ville[arrivee]->nom);

    trajet** tab_trajets = malloc(sizeof(trajet*) * nb_trajets);
    remplissage_tableau_trajets(racine, tab_trajets, tab_ville[arrivee],0);

    printf("que voulez-vous faire ? \n");
    printf("1 : afficher le premier trajet\n");
    printf("2 : afficher tous les trajets\n");
    printf("3 : afficher le trajet le plus court\n");
    printf("4 : afficher le trajet le plus rapide \n");
    printf("5 : afficher l'arbre des trajets\n");
    int reponse = 0;

    while (reponse < 1 || reponse > 5)
    {
        printf("saisissez le numero associee a l'action que vous voulez");
        scanf("%d", &reponse);
    }

    if (reponse == 1)
    {
        trajet* trajet1 = tab_trajets[0];
        int taille_trajet = 0;
        while (trajet1 != NULL)
        {
            taille_trajet++;
            trajet1 = trajet1->next;
        }
        printf("ce trajet fait passer par : ");
        for (int i = taille_trajet - 1; i >= 0; i--)
        {
            printf("%s, ", acceder_trajet(tab_trajets[0], i)->arrivee);
        }
        printf("\nil fait %g km de long et dure %gh", tab_trajets[0]->distance, tab_trajets[0]->duree);
    }

    if (reponse == 2)
    {
        for (int i = 0; i < nb_trajets; i++)
        {
            trajet* traj = tab_trajets[i];
            int taille_trajet = 0;
            while (traj != NULL)
            {
                taille_trajet++;
                traj = traj->next;
            }
            printf("le trajet %d fait passer par : ", i);
            for (int j = taille_trajet-1; j >= 0; j--)
            {
                printf("%s, ", acceder_trajet(tab_trajets[i], j)->arrivee);
            }
            printf("\nil fait %g km de long et dure %gh\n\n", tab_trajets[i]->distance, tab_trajets[i]->duree);
        }
    }

    if (reponse == 3 || reponse == 4)
    {
        meilleur_trajet(tab_trajets, nb_trajets, reponse);
    }

    if (reponse == 5)
    {
        affichage_reseau(racine);
    }
}



