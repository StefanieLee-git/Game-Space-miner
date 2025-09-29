#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "space_miner.h"

int Spaceship_init(void *self)
{
	Spaceship *ship = self;
	ship->fuel = 100;
	ship->cargo_capacity = 50;
	ship->shields = 100;
	return 1;
}

void Spaceship_destroy(void *self)
{
    Spaceship *ship = (Spaceship *)self;
    
    if (!ship) return;
    
    printf("销毁飞船...\n");
    Object_destroy(ship);
}

void Spaceship_status(void *self)
{
	Spaceship *ship = self;
	printf("Ship status: \n");
	printf("Fuel: %d%%\n", ship->fuel);
	printf("Cargo Space: %d/50\n", ship->cargo_capacity);
	printf("Shields: %d%%\n", ship->shields);
}

Object SpaceshipProto = {
		.init = Spaceship_init,
		.destroy = Spaceship_destroy,
		.describe = Spaceship_status
};

void *Planet_move(void *self, int planet_index)
{
	Planet *planet = self;
	if (planet_index >= 0 && planet_index < planet->planet_count)
	{
		printf("Warping to %s...\n", planet->orbiting_planets[planet_index]->_(description));
		return planet->orbiting_planets[planet_index];
	}

	printf("Invalid destination!.\n");
	return NULL;

}

int Planet_mine(void *self, int amount)
{
	Planet *planet = self;
	if (planet->resources >= amount)
	{
		planet->resources -= amount;
		printf("Mined %d units of resource. Remaining: %d\n", amount, planet->resources);
		return amount;
	}
	else 
	{
		printf("Not enough resources. Available: %d\n", planet->resources);
		return 0;
	}
}

int Planet_scan(void *self, int scan_type)
{
	Planet *planet = self;

	if (scan_type == 1) //详细扫描
	{
		printf("Scanning %s...\n", planet->_(description));
		printf("Climate: %s\n", planet->climate);
		printf("Resource: %d units\n", planet->resources);
		printf("Danger Level: %d/10\n", planet->danger_level);
		printf("Connected planets: %d\n", planet->planet_count);
	}
	else if (scan_type == 0) //快速扫描
	{
		printf("Quick Scan of %s:\n", planet->_(description));
    printf("Resources: %d units\n", planet->resources);
    printf("Danger: %d/10\n", planet->danger_level);
	}

	return planet->resources;
}

void Planet_destroy(void *self)
{
    Planet *planet = (Planet *)self;
    
    if (!planet) return;
    
    printf("销毁星球: %s\n", planet->proto.description ? planet->proto.description : "未知");
    
    // 释放 climate 字符串（如果是动态分配的）
    if (planet->climate && planet->climate != strstr(planet->climate, "Temperate") &&
        planet->climate != strstr(planet->climate, "Arid") &&
        planet->climate != strstr(planet->climate, "Toxic") &&
        planet->climate != strstr(planet->climate, "Crystalline")) {
        free(planet->climate);
        planet->climate = NULL;
    }
    
    // 调用 Object 的基础销毁
    Object_destroy(planet);
}

Object PlanetProto = {
		.move = Planet_move,
		.attack = Planet_scan, //我们认为“扫描”是一种主动行为，这里复用attack（攻击）
		.mine = Planet_mine,
		.destroy = Planet_destroy
};

int Alien_attack(void *self, int damage)
{
	Alien *alien = self;
	printf("%s reacts to your attack\n", alien->_(description));

	if (damage > alien->strength)
	{
		printf("%s is defeated!\n", alien->species);
		return 1;
	}
	else
	{
		alien->strength -= damage;
		printf("%s counterattacks! Strengeh: %d\n", alien->species, alien->strength);
		return 0;
	}
}

int Alien_communicate(void *self)
{
	Alien *alien = self;
	printf("Attempting communication with %s...\n", alien->species);

	if (alien->aggression < 5)
	{
		printf("%s is friendly! They share resources.\n", alien->species);
		return 1;
	}
	else
	{
		printf("%s is hostile!\n", alien->species);
		return 0;
	}

}

Object AlienProto = {
		.attack = Alien_attack,
		.init = Object_init,
		.destroy = Object_destroy
}; 

void *Galaxy_travel(void *self, int planet_index)
{
	Galaxy *galaxy = self;
	Spaceship *ship = galaxy->player_ship;

	if (ship->fuel <= 0)
	{
		printf("Out of fuel! Cannot travel.\n");
		return NULL;
	}

	if (planet_index >= 0 && planet_index <= galaxy->planet_count)
	{
		ship->fuel -= 10;
		galaxy->current_planet = galaxy->planets[planet_index];
		printf("Traveling to %s...Fuel remaining: %d%%\n",
						galaxy->current_planet->_(description), ship->fuel);
		return galaxy->current_planet;
	}

	printf("Invalid planet index.\n");
	return NULL;
}

int Galaxy_encounter(void *self, int y_n)
{
	Galaxy *galaxy = self;
	Planet *planet = galaxy->current_planet;

	if (planet->danger_level > 7 && y_n)
	{
		printf("Hostile alien encounter!\n");
		Alien *alien = NEW(Alien, "Aggressive Xenomorph");
		alien->aggression = 8;
		alien->strength = 15;
		alien->species = "Xenomorph";
		return 1; //遭遇站
	}
	else if (planet->danger_level > 3 && y_n)
	{
		printf("Neutral alien presence detected.\n");
		return 2; //中立遭遇
	}
	else 
	{
		printf("Peaceful environment. No threats detected.\n");
		return 0; //安全
	}
}

int Galaxy_trade(void *self, int y_n)
{
	Galaxy *galaxy = self;
	Spaceship *ship = galaxy->player_ship;

	if (ship->cargo_capacity > 0 && y_n)
	{
		int traded = galaxy->current_planet->_(attack)(galaxy->current_planet, 0);
		if (traded > 0)
		{
			ship->cargo_capacity -= 10;
			printf("Traded resources. Cargo space: %d/50\n", ship->cargo_capacity);
			return traded;
		}
		else
		{
			printf("Cargo hold full! Cannot collect more resources.\n");
		}
	}

	return 0;
}

void Galaxy_destroy(void *self)
{
    Galaxy *galaxy = (Galaxy *)self;
    
    if (!galaxy) return;
    
    printf("销毁 Galaxy...\n");
    
    // 销毁玩家飞船
    if (galaxy->player_ship) {
        printf("销毁玩家飞船...\n");
        if (galaxy->player_ship->_(destroy)) {
            galaxy->player_ship->_(destroy)(galaxy->player_ship);
        } else {
            free(galaxy->player_ship);
        }
        galaxy->player_ship = NULL;
    }
    
    // 销毁所有星球
    if (galaxy->planets) {
        printf("销毁 %d 个星球...\n", galaxy->planet_count);
        for (int i = 0; i < galaxy->planet_count; i++) {
            if (galaxy->planets[i]) {
                printf("销毁星球 %d...\n", i);
                if (galaxy->planets[i]->_(destroy)) {
                    galaxy->planets[i]->_(destroy)(galaxy->planets[i]);
                } else {
                    free(galaxy->planets[i]);
                }
                galaxy->planets[i] = NULL;
            }
        }
        free(galaxy->planets);
        galaxy->planets = NULL;
    }
    
    // 最后释放 Galaxy 对象本身
    if (galaxy->proto.description) {
        free(galaxy->proto.description);
        galaxy->proto.description = NULL;
    }
    
    free(galaxy);
}

Object GalaxyProto = {
		.move = Galaxy_travel,
		.attack = Galaxy_encounter,
		.mine = Galaxy_trade,
		.init = Object_init,
		.destroy = Galaxy_destroy
};

int space_process_input(Galaxy *game)
{
	printf("\n SPACE COMMD > ");

	char ch = getchar();
	getchar(); //消耗回车符
			
	switch (ch)
	{
		case 's': // 飞船状态/扫描
			printf("Enter action: (1) Get the Spaceship status, (2) Scan the Planet");
			char action = getchar();
			getchar();
			if (action == '1')
			{
				game->player_ship->_(describe)(game->player_ship);
			}
			else if (action == '2')
			{
				printf("Enter action: (0) Quick scan, (1) Detalied scan");
				char way = getchar();
				getchar();
				if (way == '0')
				{
					game->current_planet->_(attack)(game->current_planet, 0);
				}
				else if (way == '1')
				{
					game->current_planet->_(attack)(game->current_planet, 1);
				}
			}
			break;

		case 'm': // 移动/采矿
			printf("Enter action: (1) Move to planet, (2) Mine resources: ");
			char action1 = getchar();
			getchar();
			if (action1 == '1')
			{
				printf("Available planets: \n");
				for (int i = 0; i < game->planet_count; i++)
				{
					printf("	%d:%s\n", i, game->planets[i]->_(description));
				}
				printf("\nSelect planet: ");
				int planet_index;
				scanf("%d", &planet_index);
				getchar();
				game->_(move)(game, planet_index);
			}
			else if (action1 == '2')
			{
				game->current_planet->_(mine)(game->current_planet, 10);
			}
			break;	

		case 'e': // 探索/遭遇
			int encounter = game->_(attack)(game, 1);
			if (encounter == 1)
			{
				printf("Battle mode engaged! Press 'a' to attack.\n");
			}
			break;

		case 'a':
			if (game->current_planet->danger_level > 5 )
			{
				Alien *temp_alien = NEW(Alien, "Hostile Alien");
				assert(temp_alien != NULL);
				temp_alien->_(attack)(temp_alien, 12);
				temp_alien->_(destroy)(temp_alien);  // 正确销毁
			}
			else
			{
				printf("You are safe, idiot! Don't attack the air!");
			}
			break;

		case 't': // 交易
			game->_(mine)(game, 1);
			break;

		case 'q':
			printf("Returning to Earth...Thanks for playing!\n");
			game->_(destroy)(game);
			return 0;

		case 'h':
    printf("SPACE MINER COMMANDS:\n");
    printf("s - Ship status/Scan\n");
    printf("m - Move/Mine\n");
    printf("e - Explore/Encounter\n");  
    printf("a - Attack\n");
    printf("t - Trade\n");
    printf("q - Quit\n");
    printf("h - Help\n");  
    break;

		default:
			printf("Unknow command: '%c'. Type 'h' for help.\n", ch);
	}

	return 1;

}


