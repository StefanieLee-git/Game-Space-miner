#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include "space_miner.h"

Galaxy *create_galaxy()
{
    Galaxy *galaxy = NEW(Galaxy, "Milky Way Galaxy");
    if (!galaxy) {
        fprintf(stderr, "错误: 无法创建 Galaxy\n");
        return NULL;
    }

    // 初始化 Galaxy 成员
    galaxy->planet_count = 0;
    galaxy->planets = NULL;
    galaxy->current_planet = NULL;
    galaxy->home_base = NULL;

    // 创建玩家飞船
    galaxy->player_ship = NEW(Spaceship, "Starship Enterprise");
    if (!galaxy->player_ship) {
        fprintf(stderr, "错误: 无法创建玩家飞船\n");
        galaxy->_(destroy)(galaxy);
        return NULL;
    }

    // 创建星球数组
    galaxy->planet_count = 4;
    galaxy->planets = calloc(galaxy->planet_count, sizeof(Planet *));
    if (!galaxy->planets) {
        fprintf(stderr, "错误: 无法分配星球数组\n");
        galaxy->_(destroy)(galaxy);
        return NULL;
    }

    // 创建地球
    Planet *earth = NEW(Planet, "Planet Earth - Home Base");
    if (!earth) {
        fprintf(stderr, "错误: 无法创建地球\n");
        galaxy->_(destroy)(galaxy);
        return NULL;
    }
    earth->climate = "Temperate";  // 字符串字面值，不释放
    earth->resources = 1000;
    earth->danger_level = 1;
    earth->planet_count = 0;
    galaxy->planets[0] = earth;

    // 创建火星
    Planet *mars = NEW(Planet, "Mars - Red Planet");
    if (!mars) {
        fprintf(stderr, "错误: 无法创建火星\n");
        galaxy->_(destroy)(galaxy);
        return NULL;
    }
    mars->climate = "Arid";
    mars->resources = 500;
    mars->danger_level = 3;
    mars->planet_count = 0;
    galaxy->planets[1] = mars;

    // 创建 Xenon
    Planet *xenon = NEW(Planet, "Xenon Prime - Alien World");
    if (!xenon) {
        fprintf(stderr, "错误: 无法创建 Xenon\n");
        galaxy->_(destroy)(galaxy);
        return NULL;
    }
    xenon->climate = "Toxic";
    xenon->resources = 800;
    xenon->danger_level = 8;
    xenon->planet_count = 0;
    galaxy->planets[2] = xenon;

    // 创建 Crystalium
    Planet *crystalium = NEW(Planet, "Crystalium - Resource Rich");
    if (!crystalium) {
        fprintf(stderr, "错误: 无法创建 Crystalium\n");
        galaxy->_(destroy)(galaxy);
        return NULL;
    }
    crystalium->climate = "Crystalline";
    crystalium->resources = 65536;
    crystalium->danger_level = 6;
    crystalium->planet_count = 0;
    galaxy->planets[3] = crystalium;

    // 设置当前位置和基地
    galaxy->current_planet = earth;
    galaxy->home_base = earth;

    return galaxy;
}

int main()
{
	srand(time(NULL));

	printf("WELCOME TO SPACE MINER ADVENTURE!\n");
	printf("You are a space miner exploring the galaxy.\n");
	printf("Mine resources, encounter aliens, and survive!\n\n");

	Galaxy *game = create_galaxy();
	
	printf("You are currently orbiting: ");
	game->current_planet->_(describe)(game->current_planet);
	printf("Type 'h' for commands.\n");

	while (space_process_input(game)) {}

	return 0;
}



