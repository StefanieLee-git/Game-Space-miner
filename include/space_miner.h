#ifndef _SPACE_MINER_H
#define _SPACE_MINER_H

#include "object.h"

struct Spaceship {
		Object proto;
		int fuel;
		int cargo_capacity;
		int shields;
};

typedef struct Spaceship Spaceship;

int Spaceship_init(void *self);
void Spaceship_destroy(void *self);
void Spaceship_status(void *self);

struct Planet {
		Object proto;
		char *climate;
		int resources;
		int danger_level;
		struct Planet *orbiting_planets[5];
		int planet_count;
};

typedef struct Planet Planet;

void *Planet_move(void *self, int planet_index);
int Planet_mine(void *self, int amount);
int Planet_scan(void *self, int scan_type);

struct Alien {
		Object proto;
		int aggression;
		int strength;
		char *species;
};

typedef struct Alien Alien;

int Alien_attack(void *self, int damage);
int Alien_communicate(void *self);

struct Galaxy {
		Object proto;
		Planet *current_planet;
		Planet *home_base;
		Spaceship *player_ship;
		Planet **planets;
		int planet_count;
};

typedef struct Galaxy Galaxy;

void *Galaxy_travel(void *self, int planet_index);
int Galaxy_encounter(void *self, int y_n);
int Galaxy_trade(void *self, int y_n);

int space_process_input(Galaxy *game);

extern Object SpaceshipProto;
extern Object PlanetProto;
extern Object AlienProto;
extern Object GalaxyProto;

#endif
