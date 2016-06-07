#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include "../inc/behavior.h"
#include "../inc/entity.h"
#include "../inc/io.h"
#include "../inc/globals.h"

// Ends ncurses and makes the program terminate.
void terminate()
{
    endwin();
    printf("An error occurred. The program had to shut down.");
    exit(EXIT_FAILURE);
}

// Sets up ncurses and shows misc. visual content.
void startVisuals()
{ 
    initscr();
    keypad(stdscr, TRUE);
    curs_set(0);
    noecho();
    timeout(0);

    mvprintw(P_HEIGHT + 1, 0, "Controls: A - move left, D - move right, SPACEBAR - shoot");
    mvprintw(P_HEIGHT + 2, 0, "Press ESC to exit");
    
    refresh();
}

// Makes ncurses terminate.
void stopVisuals()
{
    endwin();
}

// Prints the padding of a space.
void drawSpacePadding(struct space *space)
{
    if (space == NULL)
    {
        exit(EXIT_FAILURE);
    }

    if (!P_PADDING)
    {
        return;
    }

    // Draw vertical lines
    for (unsigned char y = 0; y < space->height; y++)
    {
        for (unsigned char x = 0; x < space->width; x++)
        {
            mvprintw(y, x * (P_PADDING + 1) + 1, "%c", P_PADDING_CHAR_V);
        }
    }
}

// Prints the contents of a space on the screen.
void drawSpace(struct space *space)
{
    if (space == NULL)
    {
        terminate();
    }

    struct entity entity;
    struct pos currentPos;

    for (signed char y = 0; y < space->height; y++)
    {
        currentPos.y = y;

        for (signed char x = 0; x < space->width; x++)
        {
            currentPos.x = x;
            entity = getEntity(getSpace(), currentPos);

            if (entity.health == 0)
            {
                mvprintw(y, x * (P_PADDING + 1), "%c", P_NO_ENTITY_CHAR);
            }
            else
            {
                if (entity.canFire == 1)
                {
                    attron(A_BOLD);
                }

                mvprintw(y, x * (P_PADDING + 1), "%c", entity.symbol);
                attroff(A_BOLD);
            }
        }
    }

    refresh();
}

// Reads possible keyboard input and returns whether the program should continue.
bool handleInput()
{
    int ch = getch();

    if (ch == K_EXIT || ch == K_EXIT_ALT)
    {
        return false;
    }
    else if (ch == K_MOVE_LEFT || ch == K_MOVE_LEFT_ALT)
    {
        movePlayer(-1);
    }
    else if (ch == K_MOVE_RIGHT || ch == K_MOVE_RIGHT_ALT)
    {
        movePlayer(1);
    }
    else if (ch == K_SHOOT || ch == K_SHOOT_ALT)
    {
        spawnPlayerLaser();
    }
    else if (ch == K_SAVE || ch == K_SAVE_ALT)
    {
        saveSpaceToFile(getSpace());
    }
    else if (ch == K_LOAD || ch == K_LOAD_ALT)
    {
        setSpace(loadSpaceFromFile());
        updateCanFire();
    }

    return true;
}

// Saves the current state of a space to a file.
void saveSpaceToFile(struct space *space)
{
    if (space == NULL)
    {
        terminate();
    }

    FILE *saveFile = fopen("savefile", "w+");
    fprintf(saveFile, "SP %hhu %hhu\n", space->width, space->height);
    struct entity currentEntity;
    struct pos currentPos;

    for (signed char y = 0; y < space->height; y++)
    {
        currentPos.y = y;

        for (signed char x = 0; x < space->width; x++)
        {
            currentPos.x = x;
            currentEntity = getEntity(space, currentPos);

            // Only save alive entities
            if (currentEntity.health > 0)
            {
                fprintf(saveFile, "%hhi %hhi %u %u %u\n", x, y, currentEntity.type, currentEntity.symbol, currentEntity.health);
            }
        }
    }

    mvprintw(space->height + 5, 0, "File saved!");
    fclose(saveFile);
}

// Reads a save file and returns the saved space within.
struct space *loadSpaceFromFile()
{
    FILE *saveFile = fopen("savefile", "r");
    if (saveFile == NULL)
    {
        terminate();
    }

    unsigned char width, height;
    fscanf(saveFile, "SP %hhu %hhu", &width, &height);

    struct space *space = newSpace(width, height);
    struct entity entity = newEntity(T_INVADER, '.', 0);
    struct pos coords;
    signed char x, y;
    unsigned type, symbol, health;

    while (fscanf(saveFile, "%hhi %hhi %u %u %u", &x, &y, &type, &symbol, &health) != EOF)
    {
        coords = (struct pos){ x, y };
        entity.type = type;
        entity.symbol = symbol;
        entity.health = health;

        if (type == T_PLAYER)
        {
            setPlayer(space, coords, entity);
        }
        else
        {
            setEntity(space, coords, entity);
        }
    }

    return space;
}
