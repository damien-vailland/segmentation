/****************************************************************************
 *
 *
 * Copyright (C) 2016 Universit� de Rennes 1. All rights reserved.
 *
 * This software was developed at:
 * Universite de Rennes 1
 * Campus Universitaire de Beaulieu
 * 35042 Rennes Cedex
 *
 * This file uses the ViSP library.
 *
 
 *
 *****************************************************************************/

/*!
 tp6.cpp : Segmentation
 
 */

/****************************************************************************
 * NOMS - PRENOMS:
 *  - Vailland Damien
 *	- Plessis Axel
 *
 * Date : 7 novembre
 *****************************************************************************/
#include <iostream>

#include <visp/vpConfig.h>
#include <visp/vpDebug.h>

#include <visp/vpImage.h>
#include <visp/vpImageIo.h>
#include <visp/vpDisplayX.h>

using namespace std;



////////////////////////////////////////////////////////////////////////////////////
////////////////////// FONCTIONS UTILES ////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief affiche une image � l'�cran � la position (posX,posY) et attend un clic
 * @param img : l'image � afficher
 * @param posX, posY : coordonn�es spatiales pour positionner affichage de la fenetre sur l'�cran
 * @param title : titre de la fenetre graphique
 * @warning : fonction bloquante
 */
void afficheImage(vpImage<unsigned char> img, int posX, int posY, const char *title)
{
    vpDisplayX d(img, posX, posY, title);
    vpDisplay::display(img);
    vpDisplay::flush(img);
    vpDisplay::getClick(img);
    vpDisplay::close(img);
}



/**
 * @brief retourne l'indice du centroide le plus proche du niveau de gris 'ndg'
 * @param ndg : valeur du niveau de gris dont on cherche le centroide le plus proche
 * @param centers : tableau contenant l'ensemble des centroides auxquels on compare 'ndg'
 * @param size : nombre de centres (taille du tableau)
 */
int find_center(unsigned char ndg, const int* centers, const int size){
    
    int index=0;
    int distance = abs(ndg - centers[0]);

    for(int i=1;i<size;i++){
        if(abs(ndg - centers[i]) < distance) {
            index = i;
            distance = abs(ndg - centers[i]) ;
        }
    }
    return index;
   
}



void k_means(const vpImage<unsigned char>  &I, vpImage<unsigned char>  &Is, int k)
{
    int nb_ite=0;
    // structures de donnees pour recalcul du centre de gravite : somme des ndg et nombre d'elements attribues a chaque centre
    int* cumul_ndg= new int[k];
    int* nb_el= new int[k];
    
    // initialisation des k centres
    int* center= new int[k];

    bool converge = true ;

    // calcul des valeurs des k centres
    for(int i = 0; i<k;i++){
        center[i] = rand() % 256 ;
    }

    while(converge){

        for (int n=0; n<k;n++) {
            cumul_ndg[n]=0;
            nb_el[n]=0;
        }
            
        // 1. classification des pixels par rapport à leur distance aux centres courants
        for (int i = 0; i < I.getHeight(); i++) {
            for (int j = 0; j < I.getWidth(); j++) {
                int i_center = find_center(I[i][j],center,k);
                cumul_ndg[i_center] += I[i][j];
                nb_el[i_center] ++ ;
            }
        }

        // 2. recalcul des centres de gravite en fonction des pixels qui leur ont ete assigne
        for(int n=0;n<k;n++){
            // 3. calcul de la condition d'arret
            if(nb_el[n] != 0){
                if(abs(center[n] - cumul_ndg[n] / nb_el[n] < 1)){
                    converge = false;
                } 
                center[n] = cumul_ndg[n] / nb_el[n] ;
            }
        }

        nb_ite++;
    }
    
    cout << "nb iterations : " << nb_ite << endl;
    
    // Calcul de l'image segmentee finale
     for (int i=0; i<I.getHeight();i++){
        for (int j=0; j<I.getWidth();j++){
            Is[i][j] = center[find_center(I[i][j],center,k)];
        }
    }
    
    delete[] center;
    delete[] cumul_ndg;
    delete[] nb_el;
}


void quantification_uniforme(const vpImage<unsigned char>  &I, vpImage<unsigned char>  &Iq, int nb_nv)
    {
	Iq.resize(I.getHeight(), I.getWidth());

	int niv_gris = pow(2,nb_nv);

	int tab_niv_gris[niv_gris];
	int ecart_tranche = 256 / niv_gris ;

	for(int x = 0 ; x < niv_gris ; x++){
		// tab_niv_gris[x] = ecart_tranche * x;
		tab_niv_gris[x] = ecart_tranche * x + ecart_tranche / 2;
	}

	for(int i=0 ; i<I.getHeight() ; i++){
		for(int j=0 ; j<I.getWidth() ; j++){
			Iq[i][j] = tab_niv_gris[I[i][j] / ecart_tranche];
		}
	}
}

void histogramme(const vpImage<unsigned char>  &I){
    float histo[256];
    float max;

	for (int i = 0; i < 256; i++) {
        histo[i] = 0;
    }
	
    for (int i = 0; i < I.getHeight(); i++) {
        for (int j = 0; j < I.getWidth(); j++) {
            histo[I[i][j]]++;
        }
    }

    max = histo[0];
    for (int key = 1; key < 256; key++) {
        if (histo[key] > max) {
            max = histo[key];
        }
    }

    vpImage<unsigned char> Ih(100,256) ;
	Ih = 0;
	
	vpDisplayX d2(Ih,500,500) ;
	vpDisplay::display(Ih) ;
	vpDisplay::flush(Ih) ;
	
	int i;
	for(i=0;i<256;i++){
		vpDisplay::displayLine(Ih,99,i,99-(int)(histo[i]*99/max),i,vpColor::red);
	}
	vpDisplay::flush(Ih) ;
	vpDisplay::getClick(Ih) ;
	vpDisplay::close(Ih) ;	
}

////////////////////////////////////////////////////////////////////////////////////
//////////////////////      MAIN        ////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{
	// creation du menu
	cout << "BINP TP 6 :  SEGMENTATION " << endl ;
	cout << "--" << endl ;
	
    int k;

	// chargement d'une image
	vpImage<unsigned char>  I0;
	string sIm;
    cout << "Nom de l'image : "; cin >> sIm; // Ex: ../images/lena.pgm
    sIm = "../images/" + sIm ;
	vpImageIo::read(I0,sIm) ;

    cout << "Valeur de k : "; cin >> k; 

    /* // A decommenter pour debugger
    vpImageIo::read(I0,"../images/graines.jpg");*/
    afficheImage(I0,100,100,"Image originale") ;
    
    
    /// Segmentation par k-means
  	vpImage<unsigned char>  Isegm(I0.getHeight(),I0.getWidth(),0);
    k_means(I0, Isegm, k);
    afficheImage(Isegm,100,500,"Image segmentee") ;
    vpImageIo::write(Isegm,"../resultats/Isegm.pgm");
    
    //Segmentation par quantification uniforme
    vpImage<unsigned char>  Iquantif(I0.getHeight(),I0.getWidth(),0);
    quantification_uniforme(I0,Iquantif,k/2);
    afficheImage(Iquantif,100,500,"Image quantifiee") ;
    vpImageIo::write(Iquantif,"../resultats/Iquantif.pgm");
    
    histogramme(I0);
    histogramme(Isegm);
    histogramme(Iquantif);
    
	cout << "Fin du programme " << endl ;
	return(0);
}















