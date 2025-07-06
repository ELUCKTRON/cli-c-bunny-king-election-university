
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>

#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <signal.h>
#include <unistd.h>
#include <wait.h>

#define BUNNYGAME_FILE "BUNNYGAME.txt"

// my objects

struct BunnyGuy {
  char name[50];
  char poem[1000];
  int collectedEGG;
  int smoothness;
};

struct BunnyGirl {
  char name[50];
  int hotness;
};

struct TheGame {

  struct BunnyGuy *guys;
  struct BunnyGirl *girls;

  int numBunnyGuys;
  int numBunnyGirls;

  int limitOfContestants;
};

// my function

void getKiss(struct BunnyGirl *g, struct BunnyGuy *b) {
  bool chance = (float)b->smoothness / g->hotness > 1;

  if (chance) {
    printf(" The girl bunny liked the poem! (+1 egg)\n");
    b->collectedEGG++;
  } else {
    printf(" The girl bunny hated the poem. No kiss.\n");
  }
}

void addBunnyGuy(struct TheGame *game, struct BunnyGuy *b) {
  if (abs(game->numBunnyGuys - game->numBunnyGirls) <
      game->limitOfContestants) {
    game->guys[game->numBunnyGuys] = *b;
    game->numBunnyGuys++;
  } else {
    printf("No space for more BunnyGuys!\n");
  }
}

void addBunnyGirl(struct TheGame *game, struct BunnyGirl *g) {
  if (abs(game->numBunnyGuys - game->numBunnyGirls) <
      game->limitOfContestants) {
    game->girls[game->numBunnyGirls] = *g;
    game->numBunnyGirls++;
  } else {
    printf("No space for more BunnyGirls!\n");
  }
}

void toStringBunnyBoy(struct BunnyGuy b, char *out, size_t size) {
  snprintf(out, size, "Name: %s\nPoem: %s\nEggs: %d\nSmoothness: %i\n", b.name,
           b.poem, b.collectedEGG, b.smoothness);
}

void toStringBunnyGirl(struct BunnyGirl g, char *out, size_t size) {
  snprintf(out, size, "Name: %s\nhotness: %i\n", g.name, g.hotness);
}

void registeredBunnies(struct TheGame *game) {
  char output[50000] = "";
  char temp[1000];
  int offset = 0;

  offset += snprintf(output + offset, sizeof(output) - offset, "The Boys:\n");

  for (int i = 0; i < game->numBunnyGuys; i++) {
    toStringBunnyBoy(game->guys[i], temp, sizeof(temp));
    offset += snprintf(output + offset, sizeof(output) - offset, "%s\n", temp);
  }

  offset +=
      snprintf(output + offset, sizeof(output) - offset, "\nThe Girls:\n");

  for (int i = 0; i < game->numBunnyGirls; i++) {
    toStringBunnyGirl(game->girls[i], temp, sizeof(temp));
    offset += snprintf(output + offset, sizeof(output) - offset, "%s\n", temp);
  }

  printf("%s", output);
}

struct BunnyGuy createBunnyGuy(char *name, char *poem, int collectedEGG,
                               int smoothness) {

  struct BunnyGuy *bunnyguy = malloc(sizeof(struct BunnyGuy));
  if (bunnyguy == NULL) {
    perror("failed to aloc memory");
    exit(1);
  } else {
    memset(bunnyguy, 0, sizeof(struct BunnyGuy));
  }

  strcpy(bunnyguy->name, name);
  strcpy(bunnyguy->poem, poem);
  bunnyguy->collectedEGG = collectedEGG;
  bunnyguy->smoothness = smoothness;

  return *bunnyguy;
}

struct BunnyGirl createBunnyGirl(char *name, int hotness) {

  struct BunnyGirl *bunnygirl = malloc(sizeof(struct BunnyGirl));
  if (bunnygirl == NULL) {
    perror("failed to aloc memory");
    exit(1);
  } else {
    memset(bunnygirl, 0, sizeof(struct BunnyGirl));
  }

  strcpy(bunnygirl->name, name);
  bunnygirl->hotness = hotness;

  return *bunnygirl;
}

struct TheGame *createGame(const char *filename) {
  struct TheGame *game = malloc(sizeof(struct TheGame));
  if (game == NULL) {
    perror("failed to aloc memory");
    exit(1);
  } else {
    memset(game, 0, sizeof(struct TheGame));
  }
  game->numBunnyGuys = 0;
  game->numBunnyGirls = 0;
  game->limitOfContestants = 100;

  game->guys = malloc(10 * sizeof(struct BunnyGuy));
  game->girls = malloc(10 * sizeof(struct BunnyGirl));

  FILE *f = fopen(filename, "r");
  if (!f) {
    perror("Error opening file");
    exit(1);
  }

  char buffer[1000];
  while (fgets(buffer, sizeof(buffer), f)) {

    buffer[strcspn(buffer, "\n")] = '\0';

    char *token = strtok(buffer, " ");
    if (token == NULL)
      continue;

    if (strcmp(token, "B") == 0) {
      char *name = strtok(NULL, " ");
      char *next;
      char poem[1000] = "";
      int eggs = 0, smoothness = 0;
      while ((next = strtok(NULL, " ")) != NULL) {
        if (sscanf(next, "%d", &eggs) == 1) {

          char *smoothingToken = strtok(NULL, " ");
          if (smoothingToken != NULL &&
              sscanf(smoothingToken, "%d", &smoothness) == 1) {
            break;
          } else {
            printf("Error parsing smoothness\n");
          }
        } else {

          strcat(poem, next);
          strcat(poem, " ");
        }
      }

      struct BunnyGuy boy = createBunnyGuy(name, poem, eggs, smoothness);
      addBunnyGuy(game, &boy);

    } else if (strcmp(token, "G") == 0) {

      char *name = strtok(NULL, " ");
      int hotness = atoi(strtok(NULL, " "));

      struct BunnyGirl girl = createBunnyGirl(name, hotness);
      addBunnyGirl(game, &girl);
    }
  }
  fclose(f);

  return game;
}

int rateSmoothness(char *poem) {
  srand(time(NULL));
  int r = (rand() % 100) + 1;
  return r;
}

int rateHotness(char *name) {
  srand(time(NULL));
  int r = (rand() % 100) + 1;
  return r;
}

void freeGame(struct TheGame *game) {
  free(game->guys);
  free(game->girls);
  free(game);
}

void saveGameToFile(struct TheGame *game, const char *filename) {
  FILE *f = fopen(filename, "w");
  if (!f) {
    perror("Error saving game to file");
    return;
  }

  for (int i = 0; i < game->numBunnyGuys; i++) {
    fprintf(f, "B %s %s %d %d\n", game->guys[i].name, game->guys[i].poem,
            game->guys[i].collectedEGG, game->guys[i].smoothness);
  }

  for (int i = 0; i < game->numBunnyGirls; i++) {
    fprintf(f, "G %s %d\n", game->girls[i].name, game->girls[i].hotness);
  }

  fclose(f);
  printf("Game saved to file: %s\n", filename);
}

void mostEggBunny(struct TheGame *game) {
  if (game->numBunnyGuys == 0) {
    printf("No bunny boys registered yet.\n");
    return;
  }

  int maxIndex = 0;
  for (int i = 1; i < game->numBunnyGuys; i++) {
    if (game->guys[i].collectedEGG > game->guys[maxIndex].collectedEGG) {
      maxIndex = i;
    }
  }

  printf("\n Bunny King Candidate:\n");
  printf("Name: %s\nPoem: %s\nEggs: %d\nSmoothness: %d\n\n",
         game->guys[maxIndex].name, game->guys[maxIndex].poem,
         game->guys[maxIndex].collectedEGG, game->guys[maxIndex].smoothness);
}

// new functions for hmw2

void handler(int signumber) {

  printf("signal with the number %i arrived -> guy bunnies have reached the "
         "girls \n",
         signumber);
  fflush(stdout);
}

void getRandomKiss(struct BunnyGuy *b) {

  int r = (rand() % 20) + 1;

  b->collectedEGG += r;
  printf(" The girl bunny liked the poem of %s now he has : %i eggs\n", b->name,
         r);
}

// my simulation

int main() {

  printf("Welcome to Easter Bunny King competition\n");

  struct TheGame *game = createGame(BUNNYGAME_FILE);

  bool running = true;
  int choice;

  while (running) {

    printf("1- display registered bunnies.\n2-register a bunny.\n3-modify a "
           "bunny.\n4-delete a bunny.\n5-bunny girl react to a bunny boy's "
           "poem\n6-winner.\n7-save Game.\n8-COMPETITION(HMW2).\n9-save & "
           "exit\n10-Only Exit\n");

    // printf('please select from options below\n(eg : 1)');
    printf("Please select from options below (eg: 1): ");

    scanf("%d", &choice);

    switch (choice) {

    case 1: {
      printf("_____________________________________________________________\n");
      registeredBunnies(game);
      printf("_____________________________________________________________\n");
      break;
    }

    case 2: {
      printf("_____________________________________________________________\n");
      char boyOrGirl[10];
      do {
        printf("What type of bunny you want to add? (boy/girl): ");
        scanf("%s", boyOrGirl);
      } while (strcmp(boyOrGirl, "boy") != 0 && strcmp(boyOrGirl, "girl") != 0);

      if (strcmp(boyOrGirl, "boy") == 0) {
        char name[50], poem[1000];
        printf("Enter guy's name : ");
        scanf("%s", name);
        printf("Enter guy's poem : ");
        scanf(" %[^\n]", poem);

        struct BunnyGuy boy =
            createBunnyGuy(name, poem, 0, rateSmoothness(poem));
        addBunnyGuy(game, &boy);

        printf("guy bunny added\n");
        printf(
            "_____________________________________________________________\n");
      } else {
        char name[50];
        printf("Enter girl's name: ");
        scanf("%s", name);

        struct BunnyGirl girl = createBunnyGirl(name, rateHotness(name));
        addBunnyGirl(game, &girl);
        printf("girl bunny added\n");
        printf(
            "_____________________________________________________________\n");
      }

      break;
    }

    case 3: {
      printf("_____________________________________________________________\n");
      char modifyName[100];
      char gender[10];
      printf("Type the name of the bunny you wish to modify: ");
      scanf("%s", modifyName);

      printf("Is it a boy or girl? ");
      scanf("%s", gender);

      if (strcmp(gender, "boy") == 0) {
        bool found = false;
        for (int i = 0; i < game->numBunnyGuys; i++) {
          if (strcmp(game->guys[i].name, modifyName) == 0) {
            int modifyChoice;
            printf("\nWhat would you like to modify for '%s'?\n",
                   game->guys[i].name);
            printf("1 - Edit name\n2 - Edit poem\n3 - Edit eggs\n4 - Back to "
                   "main menu\n");
            printf("Select an option: ");
            scanf("%d", &modifyChoice);

            switch (modifyChoice) {
            case 1:
              printf("Current name: %s\n", game->guys[i].name);
              printf("Enter new name: ");
              scanf("%s", game->guys[i].name);
              printf("Name updated!\n");
              printf("_________________________________________________________"
                     "____\n");

              break;

            case 2:
              printf("Current poem: %s\n", game->guys[i].poem);
              printf("Enter new poem: ");
              scanf(" %[^\n]", game->guys[i].poem);
              game->guys[i].smoothness = rateSmoothness(game->guys[i].poem);
              printf("Poem and smoothness updated!\n");
              printf("_________________________________________________________"
                     "____\n");

              break;

            case 3:
              printf("Current eggs: %d\n", game->guys[i].collectedEGG);
              printf("Enter new egg count: ");
              scanf("%d", &game->guys[i].collectedEGG);
              printf("Egg count updated!\n");
              printf("_________________________________________________________"
                     "____\n");

              break;

            case 4:
              printf("Returning to main menu.\n");
              printf("_________________________________________________________"
                     "____\n");

              break;

            default:
              printf("Invalid option.\n");
              printf("_________________________________________________________"
                     "____\n");

              break;
            }

            found = true;
            break;
          }
        }
        if (!found) {
          printf("No bunny boy named '%s' found.\n", modifyName);
          printf("_____________________________________________________________"
                 "\n");
        }

      } else if (strcmp(gender, "girl") == 0) {
        bool found = false;
        for (int i = 0; i < game->numBunnyGirls; i++) {
          if (strcmp(game->girls[i].name, modifyName) == 0) {
            printf("Current hotness: %d\n", game->girls[i].hotness);
            printf("Enter new hotness (1-100): ");
            scanf("%d", &game->girls[i].hotness);
            printf("Hotness updated!\n");
            printf("___________________________________________________________"
                   "__\n");

            found = true;
            break;
          }
        }
        if (!found) {
          printf("No bunny girl named '%s' found.\n", modifyName);
          printf("_____________________________________________________________"
                 "\n");
        }

      } else {
        printf("Invalid gender input.\n");
        printf(
            "_____________________________________________________________\n");
      }

      break;
    }

    case 4: {
      printf("_____________________________________________________________\n");
      char deleteName[100];
      char gender[10];
      printf("Type the name of the bunny you wish to delete: ");
      scanf("%s", deleteName);

      printf("Is it a boy or girl? ");
      scanf("%s", gender);

      if (strcmp(gender, "boy") == 0) {
        bool found = false;
        for (int i = 0; i < game->numBunnyGuys; i++) {
          if (strcmp(game->guys[i].name, deleteName) == 0) {

            for (int j = i; j < game->numBunnyGuys - 1; j++) {
              game->guys[j] = game->guys[j + 1];
            }
            game->numBunnyGuys--;
            printf("Bunny boy '%s' deleted.\n", deleteName);
            printf("___________________________________________________________"
                   "__\n");

            found = true;
            break;
          }
        }
        if (!found) {
          printf("No bunny boy named '%s' found.\n", deleteName);
          printf("_____________________________________________________________"
                 "\n");
        }
      } else if (strcmp(gender, "girl") == 0) {
        bool found = false;
        for (int i = 0; i < game->numBunnyGirls; i++) {
          if (strcmp(game->girls[i].name, deleteName) == 0) {

            for (int j = i; j < game->numBunnyGirls - 1; j++) {
              game->girls[j] = game->girls[j + 1];
            }
            game->numBunnyGirls--;
            printf("Bunny girl '%s' deleted.\n", deleteName);
            printf("___________________________________________________________"
                   "__\n");

            found = true;
            break;
          }
        }
        if (!found) {
          printf("No bunny girl named '%s' found.\n", deleteName);
          printf("_____________________________________________________________"
                 "\n");
        }
      } else {
        printf("Invalid gender input.\n");
        printf(
            "_____________________________________________________________\n");
      }

      break;
    }

    case 5: {
      printf("_____________________________________________________________\n");
      char boyName[50], girlName[50];
      printf("Enter bunny boy's name: ");
      scanf("%s", boyName);
      printf("Enter bunny girl's name: ");
      scanf("%s", girlName);

      struct BunnyGuy *boy = NULL;
      struct BunnyGirl *girl = NULL;

      for (int i = 0; i < game->numBunnyGuys; i++) {
        if (strcmp(game->guys[i].name, boyName) == 0) {
          boy = &game->guys[i];
          break;
        }
      }

      for (int i = 0; i < game->numBunnyGirls; i++) {
        if (strcmp(game->girls[i].name, girlName) == 0) {
          girl = &game->girls[i];
          break;
        }
      }

      if (boy && girl) {
        getKiss(girl, boy);
      } else {
        printf("Could not find the specified boy or girl.\n");
        printf(
            "_____________________________________________________________\n");
      }

      break;
    }

    case 6: {
      printf("_____________________________________________________________\n");
      mostEggBunny(game);
      printf("_____________________________________________________________\n");
      break;
    }
    case 7: {
      printf("_____________________________________________________________\n");
      saveGameToFile(game, BUNNYGAME_FILE);
      printf("Game SAVED.\n");
      printf("_____________________________________________________________\n");

      break;
    }

    case 8: {
      printf("_______________________NEW "
             "competition___________________________________\n");

      if (game->numBunnyGuys > 0) {

        for (int i = 0; i < game->numBunnyGuys; i++) {
          game->guys[i].collectedEGG = 0;
        }
        printf("ALL BUNNIES NOW HAVE 0 EGGS \n");

        int status;
        int child_to_parent[2];
        int message;

        int parent_to_child[2];
        char ack;

        if (pipe(child_to_parent) == -1) {

          printf("error opening the pipe child_to_parent");
          exit(1);
        }

        if (pipe(parent_to_child) == -1) {
          printf("error opening the pipe parent_to_child");
          exit(1);
        }

        signal(SIGTERM, handler);

        pid_t child = fork();
        if (child < 0) {

          perror("the fork was unssuccesful");
          exit(1);
        }

        if (child > 0) {
          // parent Chief Bunny
          int maxIndex = 0;
          int maxValue = game->guys[0].collectedEGG;

          pause();

          close(child_to_parent[1]);
          close(parent_to_child[0]);

          for (int i = 0; i < game->numBunnyGuys; i++) {

            read(child_to_parent[0], &message, sizeof(message));

            printf(
                "Chief recieved the message that bunny %s got %i eggs !!! \n",
                game->guys[i].name, message);
            printf("___________________________________________________________"
                   "__\n");
            fflush(stdout);
            game->guys[i].collectedEGG = message;
            if (message > maxValue) {
              maxIndex = i;
              maxValue = message;
            }
            ack = 'x';
            write(parent_to_child[1], &ack, sizeof(char));
          }
          close(child_to_parent[0]);
          close(parent_to_child[1]);

          wait(&status);
          printf("_____________________________________________________________"
                 "\n");
          printf("Judging complete (child proccess ended) ! Now announcing the "
                 "winner...\n");

          printf(
              "the WINNER is %s and the NUMBER of eggs he collected is %i \n",
              game->guys[maxIndex].name, game->guys[maxIndex].collectedEGG);
          printf("_____________________________________________________________"
                 "\n");

        } else {

          kill(getppid(), SIGTERM);

          srand(time(NULL) + getpid());

          close(child_to_parent[0]);
          close(parent_to_child[1]);

          for (int i = 0; i < game->numBunnyGuys; i++) {
            printf("___________________________________________________________"
                   "__\n\n");
            getRandomKiss(&game->guys[i]);
            write(child_to_parent[1], &game->guys[i].collectedEGG, sizeof(int));

            read(parent_to_child[0], &ack, sizeof(char));
          }
          close(child_to_parent[1]);
          close(parent_to_child[0]);
          exit(0);
        }
      } else {
        printf("there is no bunny boy \n");
      }

      printf("_____________________________________________________________\n");

      break;
    }

    case 9: {
      printf("_____________________________________________________________\n");
      running = false;
      saveGameToFile(game, BUNNYGAME_FILE);
      printf("Good Bye\n");
      printf("_____________________________________________________________\n");

      freeGame(game);
      break;
    }

    case 10: {
      printf("_____________________________________________________________\n");
      running = false;
      printf("Good Bye\n");
      printf("_____________________________________________________________\n");

      freeGame(game);
      break;
    }

    default: {
      printf("_____________________________________________________________\n");
      printf("type the number between 1-5 only: \n");
      printf("_____________________________________________________________\n");

      break;
    }
    }
  }

  return 0;
};
