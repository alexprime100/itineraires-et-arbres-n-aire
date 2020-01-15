#ifndef LISTE_H_INCLUDED
#define LISTE_H_INCLUDED

typedef struct _ville
{
    //cette structure est une liste chainee de villes
    //le nom est le neme pour chaque element
    //chaque element correspond a un voisin de la ville correspondant a la liste
    //la distance correspond a celle entre la ville et la ville voisine (idem pour la duree)
	char* nom;
	char* voisin;
	double distance;
	double duree;
	struct _ville *suivant;
} ville;

typedef struct _liste_ville
{
    //cette structure est une liste chainee de noms de villes
    //elle est utilisee pour caracteriser les villes ascendantes d'un noeud dans l'arbre des trajets
    char* nom;
    struct _liste_ville *next;
} liste_ville;

typedef struct _noeud
{
    //cette structure est un arbre quinaire. dans le fichier connexions, on remarque que le nombre maximal de voisins d'une ville est de 5 (pour Paris, Le-Mans et Lyon)
    //un noeud peut donc avoir jusqu'a 5 fils (dans le cas contraire, certains sont NULL)
    //un noeud est constitue d'une ville, de la liste des noms des villes ascendantes, de la distance et de la duree entre la racine et le noeud
    //enfin un noeud est relie a son noeud pere et a ses noeuds fils
    ville* reseau;
    liste_ville* ancetres;
    double trajet;
    double temps;
    struct _noeud * pere;
    struct _noeud * fils1;
    struct _noeud * fils2;
    struct _noeud * fils3;
    struct _noeud * fils4;
    struct _noeud * fils5;
}noeud;

typedef struct _trajet
{
    //cette structure est une liste chainee de trajets
    char* depart;
    char* arrivee;
    double distance;
    double duree;
    struct _trajet *next;
}trajet;


ville* creer_ville(char * nom_ville, char* nom_voisin, double distance, double temps);

liste_ville* creer_liste_ville(char* nom_ville);

ville* initialisation_villes();

double puissance(double nombre, int exposant);

double conversion_char_double(char* nombre);

double conversion_char_heure(char* c_heure);

bool comparaison_chaine(char* chaine1, char* chaine2);

bool comparaison2(char* chaine, liste_ville* liste);

ville* insertion_tete(char * nom_ville, char* nom_voisin, double distance, double temps, ville* reseau);

ville* insertion_queue(char * nom_ville, char* nom_voisin, double distance, double temps, ville* reseau);

liste_ville* insertion_liste_ville(char* nom, liste_ville* ancetres);

liste_ville* suppression_liste(liste_ville* liste);

void affichage(ville* reseau);

ville* suppression_voisin(ville* reseau);

void suppression_totale(ville* reseau);

ville** remplissage_tableau_ville(ville** tab_ville, const int nb_ville);

trajet* acceder_ville(trajet* chemin, int k);

noeud* creer_noeud(ville* vill);

noeud* association_fils(noeud* parent, ville** tab_ville);

void decalage(int offset);

void _affichage_reseau(noeud* racine, int offset);

void affichage_reseau(noeud* racine);

void affichage_noeud(noeud* racine, int offset);

noeud* _remplissage_arbre(noeud* racine, ville** tab_ville, ville* arrivee, int hauteur, int compteur);

noeud* remplissage_arbre(noeud* racine, ville** tab_ville, ville* arrivee, int hauteur);

int _nombre_trajets(noeud* arbre, ville* arrivee, int compteur);

int nombre_trajets(noeud* arbre, ville* arrivee);

trajet* creer_trajet(noeud* feuille);

trajet* insertion_trajet(trajet* chemin, noeud* feuille);

trajet* creer_liste_trajet(noeud* feuille, trajet* chemin);

int remp(int* tab, const int taille);

int remplissage_tableau_trajets(noeud* arbre, trajet** tab_trajets, ville* arrivee, int compteur);

void meilleur_trajet(trajet** tab_trajets, int taille, int choix);

void programme();

#endif // LISTE_H_INCLUDED
