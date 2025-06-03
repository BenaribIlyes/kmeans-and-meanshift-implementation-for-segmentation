## **1. Description**
Le programme tpTI est un outil permettant d'effectuer la **segmentation d'image** en utilisant deux méthodes :

1. **K-Means personnalisé (kmeans_perso)**
2. **Mean-Shift (meanshift)**

Le programme permet :
- D'afficher et sauvegarder les images segmentées.
- D'effectuer une binarisation des images pour comparaison.
- D'inverser les images segmentées si nécessaire.
- D'évaluer la segmentation en la comparant avec une image de référence (ground truth).

---

## **2. Fonctionnement des algorithmes**

### ** K-Means personnalisé (kmeans_perso)**
K-Means est une méthode de classification qui divise une image en K régions en minimisant la variance intra-cluster. 

**Étapes :**
1. **Initialisation** : Choix aléatoire de K centres.
2. **Assignation** : Chaque pixel est associé au centre le plus proche (distance euclidienne).
3. **Mise à jour** : Les centres sont recalculés comme la moyenne des pixels assignés.
4. **Convergence** : L'algorithme s'arrête si les centres ne changent presque plus.

**Caractéristiques :**
- Rapide mais nécessite de fixer K à l’avance.
- Sensible aux valeurs initiales.
- Utilisé pour **une segmentation rapide et simple**.


### ** Mean-Shift (meanshift)**
Mean-Shift est une méthode de regroupement qui se base sur la similarité locale des pixels.

**Étapes :**
1. **Déplacement de la fenêtre de recherche (hs) autour d’un pixel.**
2. **Moyenne des pixels voisins dans l’espace colorimétrique (hc).**
3. **Déplacement du pixel vers cette moyenne.**
4. **Répétition jusqu’à convergence.**

**Caractéristiques :**
- Ne nécessite pas de fixer K à l’avance.
- Fonctionne bien pour **la segmentation d’images complexes**.
- Plus lent que K-Means mais plus précis pour des textures et des objets ombragés.

---

## **3. Utilisation**

### **Commande :**

# **Pour utiliser kmeans**
bash
./kmeans <image_file> <k> [ground_truth] <output_kmeans>

# **Pour utiliser meanshift**
depuis le répertoir tpTI/src/tp/build
bash
./meanshift <hs> <hc> <epsilon> <kmax> <image_file> <output_meanshift> [ground_truth]


# **Pour utiliser tp_image**
bash
./tp_image <image_file> <hs> <hc> <epsilon> <kmax> <output_kmeans> <output_meanshift> [ground_truth]



## **Exemple de commande avec les images de références**

Depuis ce répertoir: 
/Bureau/tpTI/src/tp/build

# **Pour utiliser kmeans**
bash
./kmeans ../../../data/images/texture3.png 2 ../../../data/images/texture3_VT.png ../../../data/images/segmented_kmeans_3.png
# **Pour utiliser meanshift**
depuis le répertoir tpTI/src/tp/build
bash
./meanshift 10 10 0.001 100 ../../../data/images/texture3.png ../../../data/images/segmented_meanshift_3.png ../../../data/images/texture3_VT.png
# **Pour utiliser tp_image**
bash
./tp_image ../../../data/images/texture3.png 20 30 0.001 100 output_kmeans.jpg output_meanshift.jpg ../../../data/images/texture3_VT.png
### **Paramètres :**
| **Paramètre**            | **Description**                                      |
|--------------------------|------------------------------------------------------|
| `<image_file>`           | Chemin vers l’image à segmenter.                   |
| `<hs>`                   | Paramètre spatial de Mean-Shift (ex: `20`).        |
| `<hc>`                   | Paramètre colorimétrique de Mean-Shift (ex: `30`). |
| `<epsilon>`              | Seuil de convergence (ex: `0.001`).                |
| `<kmax>`                 | Nombre maximal d’itérations de Mean-Shift (ex: `100`). |
| `<output_kmeans>`        | Fichier de sortie pour l’image segmentée par K-Means. |
| `<output_meanshift>`     | Fichier de sortie pour l’image segmentée par Mean-Shift. |
| `[ground_truth]` *(optionnel)* | Chemin vers l’image de référence (pour évaluation). |

---

## **4. Exemples d'exécution**

### **Sans évaluation de segmentation :**
bash
./tp_image ./../../data/images/texture3.png 20 30 0.001 100 output_kmeans.jpg output_meanshift.jpg
Génère :
- `output_kmeans.jpg` (segmentation par K-Means)
- `output_meanshift.jpg` (segmentation par Mean-Shift)


### **Avec évaluation de segmentation :**
bash
./tp_image ./../../data/images/texture3.png 20 30 0.001 100 output_kmeans.jpg output_meanshift.jpg ./../../data/images/texture3_VT.png
Comparaison avec `texture3_VT.png` (image de référence)
Affichage des métriques d’évaluation (Précision, Recall, F1-Score)

---

## **5. Fonctionnalités spéciales**

### ** Inversion des images segmentées**
Le programme demande à l'utilisateur s'il veut **inverser les images** après segmentation :
bash
Do you want to invert the image (Binarised KMeans)? (y/n):
Do you want to invert the image (Binarised MeanShift)? (y/n):
Si `y`, l’image est inversée et affichée sous "Image (Inverted)".

---

## **6. Résultats sauvegardés**
| **Fichier généré**        | **Description** |
|--------------------------|----------------|
| `output_kmeans.jpg`      | Résultat de segmentation par K-Means. |
| `output_meanshift.jpg`   | Résultat de segmentation par Mean-Shift. |
| `binarised_kmeans.jpg`   | Version binarisée de `output_kmeans.jpg`. |
| `binarised_meanshift.jpg` | Version binarisée de `output_meanshift.jpg`. |

---

## **7. Dépendances et compilation**
Le programme utilise **OpenCV**. Assurez-vous qu'il est installé :
bash
sudo apt install libopencv-dev
### **Compilation avec CMake**
bash
mkdir build
cd build
cmake ..
make
./tp_TI ...
---
