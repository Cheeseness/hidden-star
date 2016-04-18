#include "travel.h"

int travel_setup()
{
	printf("Loading travel...\n");
	main_scene = SCENE_TRAVEL;
	frame_skip=0;

	t_stars1 = Load_tex("sprites/stars/stars1_256.png");
	t_stars2 = Load_tex("sprites/stars/stars2_256.png");
	t_stars3 = Load_tex("sprites/stars/stars3_256.png");
	t_stars4 = Load_tex("sprites/stars/stars4_256.png");
	t_stars5 = Load_tex("sprites/stars/stars5_256.png");
	t_stars6 = Load_tex("sprites/stars/stars6_256.png");

	t_node = Load_tex("sprites/starmap/starmap_star_inactive.png");
	SDL_QueryTexture(t_node, NULL, NULL, &half_node_sprite, NULL);
	half_node_sprite /= 2;

	vector_free_and_free(&node_list);
	vector_init(&node_list, TRAVEL_MAX_NODES);
	for (int i = 0; i < TRAVEL_MAX_NODES; i ++)
	{
		Travel_Node *t = malloc(sizeof(Travel_Node));
		t->x = rand() % 60;
		t->y = rand() % 60;
		t->faction = rand() % 4;
		t->connectedNode1 = -1;
		t->connectedNode2 = -1;
		printf("Adding node %d, %d\n", t->x, t->y);
		vector_add(&node_list, t);
	}

	for (int i = 0; i < vector_get_size(&node_list); i++)
	{
		Vector *adjacentNodes = malloc(sizeof(Vector));
		vector_init(adjacentNodes, 2);
		Travel_Node *t = (Travel_Node *)vector_get(&node_list, i);

		for (int j = 0; j < vector_get_size(&node_list); j++)
		{
			if (j != i)
			{
				Travel_Node *n = vector_get(&node_list, j);
				if (n->connectedNode1 == -1)
				{
					n->connectedNode1 = i;
					t->connectedNode1 = j;
				}
				else if (n->connectedNode2 == -1)
				{
					n->connectedNode2 = i;
					t->connectedNode1 = j;
				}
			}
			if (t->connectedNode1 != -1)
			{
				break;
			}
		}

		for (int j = 0; j < vector_get_size(&node_list); j++)
		{
			int j = rand() % TRAVEL_MAX_NODES;
			if (j != i)
			{
				Travel_Node *n = vector_get(&node_list, j);
				if (n->connectedNode1 == -1)
				{
					n->connectedNode1 = i;
					t->connectedNode2 = j;
				}
				else if (n->connectedNode2 == -1)
				{
					n->connectedNode2 = i;
					t->connectedNode2 = j;
				}
			}
			if (t->connectedNode1 != -1)
			{
				break;
			}
		}
	}

	SDL_SetRenderDrawColor(main_renderer, 0x00, 0x00, 0x00, 255);
	gui_clear();
	return 0;
}

void travel_draw()
{
	SDL_RenderClear(main_renderer);
	for (int i = 0; i < vector_get_size(&node_list); i++)
	{
		Travel_Node *t = (Travel_Node *)vector_get(&node_list, i);
		Travel_Node *c1 = (Travel_Node *)vector_get(&node_list, t->connectedNode1);
		Travel_Node *c2 = (Travel_Node *)vector_get(&node_list, t->connectedNode2);

		main_blit(t_node, t->x - half_node_sprite, t->y - half_node_sprite, NOFLIP, NULL);


		SDL_SetRenderDrawColor(main_renderer, 255, 255, 255, 128);
		SDL_RenderDrawLine(main_renderer, t->x, t->y, c1->x, c1->y);
		SDL_RenderDrawLine(main_renderer, t->x, t->y, c2->x, c2->y);
		SDL_SetRenderDrawColor(main_renderer, 0, 0, 0, 255);
		//printf("Drawing node %d, %d, %d, %d\n", t->x, t->y, t->connectedNode1, t->connectedNode2);
	}
}
