import pandas as pd
import matplotlib.pyplot as plt

# Lire les données du fichier CSV
df = pd.read_csv('resultats.csv')

# Vérifier les noms des colonnes
print("Colonnes du fichier CSV :", df.columns)

# Supprimer les espaces et caractères non visibles dans les noms de colonnes
df.columns = df.columns.str.strip()

# Afficher les données pour vérifier leur structure
print("Données du fichier CSV :\n", df)

# Créer une figure pour les graphiques
plt.figure(figsize=(15, 5))

# Tracer le graphique pour le temps d'exécution
plt.subplot(1, 3, 1)
plt.bar(df['Algorithme'], df['Temps (secondes)'], color=['blue', 'orange', 'green'])
plt.xlabel('Algorithme')
plt.ylabel('Temps (secondes)')
plt.title('Temps d\'exécution des algorithmes')

# Tracer le graphique pour la mémoire maximale utilisée
plt.subplot(1, 3, 2)
plt.bar(df['Algorithme'], df['Mémoire maximale (octets)'], color=['blue', 'orange', 'green'])
plt.xlabel('Algorithme')
plt.ylabel('Mémoire maximale (octets)')
plt.title('Mémoire maximale utilisée')

# Tracer le graphique pour la longueur du LCS
plt.subplot(1, 3, 3)
plt.bar(df['Algorithme'], df['Longueur LCS'], color=['blue', 'orange', 'green'])
plt.xlabel('Algorithme')
plt.ylabel('Longueur LCS')
plt.title('Longueur du LCS par algorithme')

# Ajuster l'affichage des graphiques
plt.tight_layout()
plt.show()
