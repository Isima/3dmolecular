# 3dmolecular

Calcul de la forme 3D d'une molécule à partir de la liste de ses atomes et de ses liaisons.
================================================================

La forme 3D est calculé en utilisant la théorie des jeux. La molécule est vu comme un graphe sur lequel on construit un arbre couvrant. Dans ce modèle chaque arrête du graphe est un joueur.

================================================================

Installation
------------

git clone https://github.com/Isima/3dmolecular.git
make

================================================================

Execution
---------

./molecular3d "nomDuFichierDeLiaisons.txt" numéroDeLaRun

Exemple
-------

./molecular3d "Exemples/cycle1.txt" 1



