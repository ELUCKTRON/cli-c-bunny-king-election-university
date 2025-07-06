# 🐰 CLI Bunny King Election 👑 (C Project – Semester 3, University)

A command-line simulation of the whimsical **Easter Bunny King** contest — built in C as part of our **Operating Systems** course at university.

This interactive project showcases key systems programming techniques:

- 👶 `fork()` – child process creation
- 📢 `signal()` – inter-process signaling
- 📨 `pipe()` – unidirectional communication
- 💾 File I/O – storing bunny data
- 🎲 `rand()` – randomness in egg rewards

All within a Unix/Linux environment (`opsys.inf.elte.hu` compatible).

Perfect for showcasing **process control**, **IPC**, and **file manipulation** in a playful, engaging way! 🧠🐣

# 🐰 Easter Bunny King 👑 — A C Systems Programming Simulation

Spring has arrived 🌸, and so has the legendary **Easter Bunny King Election**!  
The **Chief Bunny** is organizing a poetic and playful contest between eager bunny boys 🐇.  
Each competitor brings his finest verse 💌 and hopes to collect the most red eggs 🥚 from the bunny girls 🐰 during the traditional Easter Monday watering ceremony.  
With eggs, poetry, signals, and child processes — the game is on!  
Who will win the crown and become the glorious **Easter Bunny King**? 👑

---

## 📜 Storyline (Multiline Edition)

In the peaceful bunny village, springtime awakens an age-old tradition:  
> 🐇 The bunny boys prepare their best poems.  
> 🐰 The bunny girls await the charm and verses.  
> 👑 The Chief Bunny oversees the competition.  

Each bunny boy must **register** for the contest with their **name** and **watering poem**.  
Initially, their **egg count** is zero — but not for long!  
On **Easter Monday**, the **Chief Bunny** dispatches the boys (child processes)  
to perform their poems and win red eggs from the bunny girls.  
The girls grant **1 to 20 eggs**, depending on how much they liked the poem.  
All this unfolds in a well-structured C simulation using **pipes**, **signals**, and **files**.  
At the end, the **Easter Bunny King** is crowned — based on total eggs collected.

---


## ⚙️ How It Works

### ✅ Registration Phase
- Bunny boys register with:
  - Name
  - Poem
  - Egg count (initially 0)
- Records are stored in `BUNNYGAME.txt`.

### 🧼 File Options
- Register new bunny boys
- Modify or delete existing entries
- List all current participants

### 🐣 Watering Day
- Bunny boys become child processes (`fork`)
- They:
  - Signal the Chief Bunny upon arrival
  - Recite their poems
  - Receive a **random egg count** (1–20)
  - Send the result back via **pipe**
- The file is updated with their new egg count
- The winner is announced 🎉

---

## 📄 Sample Data (`BUNNYGAME.txt`)

```
B Speedy RosesAreRed 0 25
B Rocky SpringIsCool 0 65
B Jumper HoppyJoy 0 40
B Sneaky EggSurprise 0 10
G Lily 80
G Bella 50
G Fluffy 60
```

---

## 🚀 Compilation & Run Instructions

🐧 This project runs on Linux systems — tested on Debian-based environments like Ubuntu.
✅ Works perfectly on opsys.inf.elte.hu or any similar Debian-compatible setup.

```
gcc bunnyGame.c -o  bunnyGame.out
./bunnyGame.out
```

---

## 📚 Technologies & Concepts Used

- ✅ C Programming
- 🧵 Process creation with `fork()`
- 📢 Signals (`kill`, `signal`)
- 📬 Pipes (IPC)
- 💾 File I/O
- 🎲 Randomized reward system

---

## 🎓 Academic Info

- 👨‍🎓 Student: Saeed Khanloo  
- 🧪 Course: Operating Systems  
- 🗓️ Semester: 3  
- 🏫 University: ELTE

---

## 🧪 Future Enhancements

- Networked version (remote bunny villages 🗺️)
- GUI version using ncurses or SDL
- Advanced personality system for bunny girls
- Persistent leaderboard 🏆

---

## 🪪 License

MIT License – Free to use, study, and adapt.

---

✨ Let the poetry flow and the eggs roll. Long live the Bunny King! 👑
