import pandas as pd
import matplotlib.pyplot as plt

file_path = "results.txt"

# --- Lecture robuste du fichier ---
with open(file_path, "r") as f:
    lines = f.readlines()

# Trouver la ligne du tableau
for i, line in enumerate(lines):
    if line.strip().startswith("Step"):
        skip_rows = i
        break

data = pd.read_csv(file_path, skiprows=skip_rows)

# --- Étend la simulation jusqu'à 1000 itérations ---
max_steps = 1000
last_step = data["Step"].iloc[-1]

if last_step < max_steps:
    last_row = data.iloc[-1].copy()
    # On complète avec des copies de la dernière ligne (population stable)
    extra_steps = pd.DataFrame({
        "Step": range(last_step + 1, max_steps + 1),
        "Rabbits": [last_row["Rabbits"]] * (max_steps - last_step),
        "Wolfs": [last_row["Wolfs"]] * (max_steps - last_step),
        "Grass": [last_row["Grass"]] * (max_steps - last_step),
    })
    data = pd.concat([data, extra_steps], ignore_index=True)

# --- Tracé ---
plt.figure(figsize=(10, 6))
plt.plot(data["Step"], data["Rabbits"], label="Lapins", linewidth=2)
plt.plot(data["Step"], data["Wolfs"], label="Loups", linewidth=2)
plt.plot(data["Step"], data["Grass"], label="Herbe", linewidth=2)

plt.title("Évolution des populations (Loups, Lapins, Herbe)", fontsize=14)
plt.xlabel("Étape de simulation", fontsize=12)
plt.ylabel("Nombre d’individus", fontsize=12)
plt.legend()
plt.grid(True, linestyle="--", alpha=0.6)
plt.xlim(0, max_steps)
plt.tight_layout()

plt.savefig("figure.png", dpi=300)
plt.show()
