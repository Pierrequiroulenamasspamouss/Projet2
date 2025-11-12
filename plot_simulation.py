import pandas as pd
import matplotlib.pyplot as plt

file_path = "results.txt"

with open(file_path, "r") as f:
    lines = f.readlines()

for i, line in enumerate(lines):
    if line.strip().startswith("Step"):
        skip_rows = i
        break

data = pd.read_csv(file_path, skiprows=skip_rows)

max_steps = 1000
last_step = data["Step"].iloc[-1]

if last_step < max_steps:
    last_row = data.iloc[-1].copy()
    extra_steps = pd.DataFrame({
        "Step": range(last_step + 1, max_steps + 1),
        "Rabbits": [last_row["Rabbits"]] * (max_steps - last_step),
        "Wolfs": [last_row["Wolfs"]] * (max_steps - last_step),
        "Grass": [last_row["Grass"]] * (max_steps - last_step),
    })
    data = pd.concat([data, extra_steps], ignore_index=True)

fig, ax1 = plt.subplots(figsize=(10, 6))

# Axe gauche pour Lapins et Loups
ax1.plot(data["Step"], data["Rabbits"], label="Lapins", linewidth=2, color="blue")
ax1.plot(data["Step"], data["Wolfs"], label="Loups", linewidth=2, color="red")
ax1.set_xlabel("Étape de simulation", fontsize=12)
ax1.set_ylabel("Nombre de Lapins et Loups", fontsize=12, color="black")
ax1.tick_params(axis='y', labelcolor='black')
ax1.grid(True, linestyle="--", alpha=0.6)

# Axe droit pour l'Herbe
ax2 = ax1.twinx()
ax2.plot(data["Step"], data["Grass"], label="Herbe", linewidth=2, color="green")
ax2.set_ylabel("Nombre d'Herbe", fontsize=12, color="green")
ax2.tick_params(axis='y', labelcolor='green')


# Légende combinée
lines1, labels1 = ax1.get_legend_handles_labels()
lines2, labels2 = ax2.get_legend_handles_labels()
ax1.legend(lines1 + lines2, labels1 + labels2, loc="upper left")

plt.title("Évolution des populations (Loups, Lapins, Herbe)", fontsize=14)
ax1.set_xlim(0, max_steps)
fig.tight_layout()

plt.savefig("figure.png", dpi=300)
plt.show()
