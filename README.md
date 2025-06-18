# TP6 - Segmentation en C++

> **BINP - Université de Rennes**  
>  
> **Étudiants :**  
> - *Damien Vailland*  
> - *Axel Plessis*  
>   
> **Date** : 07/11/2024

## 🧠 Sujet du TP

Ce TP avait pour but de nous initier aux méthodes de **segmentation d'images** à travers deux techniques principales :  
- L’algorithme des **k-means**,  
- La **quantification scalaire**.

Ces méthodes permettent de **réduire le nombre de niveaux de gris** dans une image tout en conservant une qualité visuelle suffisante.

Le travail a été effectué en **C++** à l’aide de la bibliothèque **ViSP**.

---

## 📌 Objectifs

- Comprendre le fonctionnement de l’algorithme **k-means** appliqué à la segmentation d’images.
- Étudier l’impact du paramètre `k` sur le rendu visuel.
- Comparer k-means à une **quantification scalaire uniforme**.
- Visualiser et analyser les **histogrammes** résultants.

---

## ⚙️ Fonctionnement de l’algorithme k-means

L’algorithme suit les étapes classiques :

1. Choix aléatoire de `k` centroïdes dans l’image.
2. Assignation de chaque pixel au centroïde le plus proche.
3. Mise à jour des centroïdes en fonction des clusters formés.
4. Répétition jusqu’à stabilisation.

L’algorithme a été testé sur plusieurs images avec différentes valeurs de `k` (1, 2, 4, 8, 10, 12, 24).  
> 🖼️ Résultat : Plus `k` augmente, plus l’image retrouve sa fidélité visuelle, à partir d’un certain seuil (souvent autour de 8).

---

## 📊 Visualisation par histogrammes

Pour mieux analyser les résultats, des **histogrammes** ont été générés avant et après segmentation, en utilisant des outils développés dans un TP précédent.

On observe :

- **k-means** génère des pics **non uniformes**, adaptés à la distribution réelle des niveaux de gris.
- Les pics reflètent **les zones dominantes de l’image**.

---

## ⚖️ Comparaison avec la quantification scalaire

La **quantification scalaire** répartit les niveaux de gris **de manière fixe et régulière**. En comparaison :

| Méthode        | Avantage                                 | Inconvénient                            |
|----------------|------------------------------------------|-----------------------------------------|
| k-means        | Fidèle à l'image originale               | Plus coûteux en calcul                  |
| Quantification | Plus rapide et plus contrasté            | Moins adapté au contenu de l’image      |

> 📌 Utiliser la quantification pour **accentuer les contrastes**, et le k-means pour **préserver les nuances**.

---

## 🛠️ Dépendances

- **ViSP** (Visual Servoing Platform)  
- **CMake** pour la compilation  
- Compilateur C++ compatible C++11+

---

📚 Conclusion
La segmentation par k-means et la quantification scalaire sont deux outils puissants pour simplifier les images.
Chacune a ses avantages selon l’objectif visé :

- k-means : segmentation intelligente et fidèle
- Quantification : traitement rapide et contrasté
