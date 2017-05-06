#include "starmap.h"

int starmap_setup()
{
	printf("Loading travel...\n");
	main_scene = SCENE_STARMAP;
	frame_skip=0;

	t_stars1 = Load_tex("sprites/stars/stars1_256.png");
	t_stars2 = Load_tex("sprites/stars/stars2_256.png");
	t_stars3 = Load_tex("sprites/stars/stars3_256.png");
	t_stars4 = Load_tex("sprites/stars/stars4_256.png");
	t_stars5 = Load_tex("sprites/stars/stars5_256.png");
	t_stars6 = Load_tex("sprites/stars/stars6_256.png");

	t_node = Load_tex("sprites/starmap/starmap_star_inactive.png");
	t_node_current = Load_tex("sprites/starmap/starmap_star_active.png");
	SDL_QueryTexture(t_node, NULL, NULL, &half_node_sprite, NULL);
	half_node_sprite /= 2;

	update_starmap_icons();

	SDL_SetRenderDrawColor(main_renderer, 0x00, 0x00, 0x00, 255);

	music_stop(500);
	Travel_Node *cn = (Travel_Node *)vector_get(starmap, current_node);
	//if (cn->is_inhabited) //Do we care whether it's inhabited or not?
	music_loop_group(MUSIC_ROLE_STARMAP, cn->f, 0, -1);
	return 0;
}

void starmap_set_seed(int s)
{
	seed = s;
}

void starmap_init()
{
	starmap_load_names("text/star_name_chunks.txt");
}

void starmap_load_names(char *fname)
{
	printf("Loading star names...\n");

	star_names = malloc(sizeof(Vector));
	vector_init(star_names, 10);

	FILE *f;
	char line[10];
	Vector *segments = malloc(sizeof(Vector));
	vector_init(segments, 10);

	f = fopen(fname, "r");
	if (f != NULL)
	{
		//load line by line
		while (fgets(line, 10 -1, f) != NULL)
		{
			char *s = malloc(sizeof(char) * (strlen(line) + 1));
			strcpy(s, line);
			if (s[strlen(s) - 1] == '\n')
			{
				s[strlen(s) - 1] = '\0';
			}
			vector_add(segments, s);
		}
	}

	for (int i = 0; i < vector_get_size(segments); ++i)
	{
		for (int j = 0; j < vector_get_size(segments); ++j)
		{
			char *name = malloc(sizeof(char) * (strlen((char *)vector_get(segments, i)) + strlen((char *)vector_get(segments, j)) + 1));
			name[0] = '\0';
			strcat(name, (char *)vector_get(segments, i));
			strcat(name, (char *)vector_get(segments, j));
			vector_add(star_names, name);
		}
	}
	for (int i = 0; i < vector_get_size(segments); ++i)
	{
		for (int j = 0; j < vector_get_size(segments); ++j)
		{
			for (int k = 0; k < vector_get_size(segments); ++k)
			{
				char *name = malloc(sizeof(char) * (strlen((char *)vector_get(segments, i)) + strlen((char *)vector_get(segments, j)) + strlen((char *)vector_get(segments, k)) + 1));
				name[0] = '\0';
				strcat(name, (char *)vector_get(segments, i));
				strcat(name, (char *)vector_get(segments, j));
				strcat(name, (char *)vector_get(segments, k));
				vector_add(star_names, name);
			}
		}
	}
	printf("Generated %d star names.\n", vector_get_size(star_names));
	vector_free_and_free(segments);
	free(segments);

	srand(seed);
	vector_shuffle(star_names);
}


void starmap_animate(int x, int y){
}

int starmap_move_sector(int direction){
	int newX=t_sectorX;
	int newY=t_sectorY;
	gui_clear();
	switch(direction){
		case 0:
			newX=round(((Travel_Node *)vector_get(starmap, current_node))->x/64)*64;
			newY=round(((Travel_Node *)vector_get(starmap, current_node))->y/64)*64;
			break;
		case 1:
			newY-=64;
			break;
		case 2:
			newX+=64;
			break;
		case 3:
			newY+=64;
			break;
		case 4:
			newX-=64;
			break;
	}
	if(newX<0){newX=0;}
	if(newY<0){newY=0;}
	if(newX>448){newX=448;}
	if(newX>448){newY=448;}
	while(t_sectorX!=newX || t_sectorY!=newY){
		if(t_sectorX < newX){t_sectorX+=4;}
		if(t_sectorX > newX){t_sectorX-=4;}
		if(t_sectorY < newY){t_sectorY+=4;}
		if(t_sectorY > newY){t_sectorY-=4;}
		starmap_draw(starmap);
		SDL_RenderPresent(main_renderer);
		SDL_Delay(20);
	}
	update_starmap_icons();
	return 0;
}

void update_starmap_icons()
{
	gui_clear();
	Travel_Node *cn = (Travel_Node *)vector_get(starmap, current_node);

	gui_add_symbol_button(SYMBOL_ARROW_CENTRE, 32, 55, -1, BUTTON_STATE_ENABLED, BUTTON_STYLE_GUI, -1, &starmap_move_sector, 0, NULL, -1, NULL, -1);
	gui_add_symbol_button(SYMBOL_COMMS, 47, 55, -1, BUTTON_STATE_DISABLED, BUTTON_STYLE_GUI, -1, &starmap_move_sector, 0, NULL, -1, NULL, -1);
	gui_add_symbol_button(SYMBOL_MENU, 55, 55, -1, BUTTON_STATE_DISABLED, BUTTON_STYLE_GUI, -1, &starmap_move_sector, 0, NULL, -1, NULL, -1);

	int b = gui_add_symbol_button(SYMBOL_ARROW_LEFT, 0, 55, -1, BUTTON_STATE_ENABLED, BUTTON_STYLE_GUI, -1, &starmap_move_sector, 4, NULL, -1, NULL, -1);
	if(t_sectorX<=0){
		update_button_state(b, BUTTON_STATE_DISABLED);
	}
	b = gui_add_symbol_button(SYMBOL_ARROW_UP, 8, 55, -1, BUTTON_STATE_ENABLED, BUTTON_STYLE_GUI, -1, &starmap_move_sector, 1, NULL, -1, NULL, -1);
	if(t_sectorY<=0){
		update_button_state(b, BUTTON_STATE_DISABLED);
	}
	b = gui_add_symbol_button(SYMBOL_ARROW_DOWN, 16, 55, -1, BUTTON_STATE_ENABLED, BUTTON_STYLE_GUI, -1, &starmap_move_sector, 3, NULL, -1, NULL, -1);
	if(t_sectorY>=448){
		update_button_state(b, BUTTON_STATE_DISABLED);
	}
	b = gui_add_symbol_button(SYMBOL_ARROW_RIGHT, 24, 55, -1, BUTTON_STATE_ENABLED, BUTTON_STYLE_GUI, -1, &starmap_move_sector, 2, NULL, -1, NULL, -1);
	if(t_sectorX>=448){
		update_button_state(b, BUTTON_STATE_DISABLED);
	}

	int default_button = -1;
	for (int i = 0; i < vector_get_size(starmap); i++)
	{
		Travel_Node *t = (Travel_Node *)vector_get(starmap, i);
		if(t->x >= t_sectorX && t->x < t_sectorX+64 && t->y >= t_sectorY && t->y < t_sectorY+64){
			SDL_Texture* tex = t_node;
			int state = BUTTON_STATE_DISABLED;
			if (current_node == i || t->depth == 0)
			{
				tex = t_node_current;
			}

			for (int j = 0; j < sizeof(cn->connections) / sizeof(Travel_Node *); ++j)
			{
				if (cn->connections[j] == t)
				{
					state = BUTTON_STATE_ENABLED;
				}
			}

			if (t == (Travel_Node *)vector_get(starmap, current_node))
			{
				state = BUTTON_STATE_ENABLED;
			}
			b = gui_add_sprite_button(tex, (t->x-t_sectorX) - half_node_sprite, (t->y-t_sectorY) - half_node_sprite, -1,  state, BUTTON_STYLE_STARMAP, -1, &starmap_go, i, starmap_update_star_name, i, starmap_update_star_name, -1, NOFLIP, faction_colors[t->f]);
			if (current_node == i)
			{
				default_button = b;
			}
		}
	}
	update_button_state(default_button, BUTTON_STATE_SELECTED);
}

int starmap_update_star_name(int star)
{
	if (star >= 0)
	{
		Travel_Node *n = (Travel_Node *)vector_get(starmap, star);
		current_star = n->node_name;
	}
	else
	{
		current_star = "";
	}

	return 0;
}

int starmap_go(int destination)
{
	//get selected button
	current_node = destination;
	update_starmap_icons();

	Travel_Node *cn = (Travel_Node *)vector_get(starmap, current_node);
	printf("Initiating travel to %s!\n", cn->node_name);

//	if (cn->is_inhabited)
	{
		//printf("Found inhabited planet! %d\n", cn->f);
		comms_set_faction(cn->f);
		comms_set_subject_name(cn->node_name);
		planet_setup();
		comms_setup();
	}
	return 0;
}



void make_child_nodes(Vector *node_list, Travel_Node *n, int max_depth, int spread, int jitter, int merge_dist, int bounds, int page_bounds, int direction)
{
	Travel_Node *nn = malloc(sizeof(Travel_Node));
	nn->depth = n->depth + 1;
	nn->x = (rand() % jitter) + spread;
	nn->y = (rand() % jitter) + spread;
	nn->connected = false;
	nn->f = n->f;

	for (int i = 0; i < sizeof(nn->connections) / sizeof(Travel_Node *); ++i)
	{
		nn->connections[i] = NULL;
	}

	//Make sure that our position is an appropriate distance from other sibling nodes
	switch (direction)
	{
		case 0:
			nn->x = n->x + (nn->x * 2 - nn->x);
			nn->y = n->y - nn->y * 2;
			break;
		case 1:
			nn->y = n->y + (nn->y * 2 - nn->y);
			nn->x = n->x + nn->x * 2;
			break;
		case 2:
			nn->x = n->x + (nn->x *2 - nn->x);
			nn->y = n->y + nn->y * 2;
			break;
		case 3:
			nn->x = n->x - nn->x * 2;
			nn->y = n->y + (nn->y * 2 - nn->y);
			break;
	}

	//Clamp to 0 and bounds
	if (nn->x > bounds)
	{
		nn->x = bounds - (rand() % jitter);
	}
	else if (nn->x < 0)
	{
		nn->x = 0 + (rand() % jitter);
	}
	if (nn->y > bounds)
	{
		nn->y = bounds - (rand() % jitter);
	}
	else if (nn->y < 0)
	{
		nn->y = 0 + (rand() % jitter);
	}

	//Avoid pagination bounds (more on top and bottom for UI)
	if (nn->x % page_bounds < 4)
	{
		nn->x += 4;
	}
	else if (nn->x % page_bounds > page_bounds - 4)
	{
		nn->x -= 4;
	}

	if (nn->y % page_bounds < 8)
	{
		nn->y += 8;
	}
	else if (nn->y % page_bounds > page_bounds - 8)
	{
		nn->y -= 8;
	}

	for (int i = 0; i < vector_get_size(node_list); ++i)
	{
		Travel_Node *on = (Travel_Node *)vector_get(node_list, i);
		//If this node is too close to any other nodes, destroy it.
		if ((on->x - nn->x) * (on->x - nn->x) + (on->y - nn->y) * (on->y - nn->y) < merge_dist * merge_dist)
		{
			free(nn);
			return;
		}
	}

	vector_add(node_list, nn);

	//If we're not too deep, make more nodes
	if(nn->depth < max_depth)
	{
		switch (direction)
		{
			case 0:
				make_child_nodes(node_list, nn, max_depth, spread, jitter, merge_dist, bounds, page_bounds, 0);
				make_child_nodes(node_list, nn, max_depth, spread, jitter, merge_dist, bounds, page_bounds, 1);
				make_child_nodes(node_list, nn, max_depth, spread, jitter, merge_dist, bounds, page_bounds, 3);
				break;
			case 1:
				make_child_nodes(node_list, nn, max_depth, spread, jitter, merge_dist, bounds, page_bounds, 0);
				make_child_nodes(node_list, nn, max_depth, spread, jitter, merge_dist, bounds, page_bounds, 1);
				make_child_nodes(node_list, nn, max_depth, spread, jitter, merge_dist, bounds, page_bounds, 2);
				break;
			case 2:
				make_child_nodes(node_list, nn, max_depth, spread, jitter, merge_dist, bounds, page_bounds, 1);
				make_child_nodes(node_list, nn, max_depth, spread, jitter, merge_dist, bounds, page_bounds, 2);
				make_child_nodes(node_list, nn, max_depth, spread, jitter, merge_dist, bounds, page_bounds, 3);
				break;
			case 3:
				make_child_nodes(node_list, nn, max_depth, spread, jitter, merge_dist, bounds, page_bounds, 0);
				make_child_nodes(node_list, nn, max_depth, spread, jitter, merge_dist, bounds, page_bounds, 2);
				make_child_nodes(node_list, nn, max_depth, spread, jitter, merge_dist, bounds, page_bounds, 3);
				break;
		}
	}

	//Do the faction specific stuff after we've made our children so that they can inheret randomness (if set)
	if (nn->f < 0)
	{
		nn->f = rand() % 4;
	}
	sprintf(nn->node_name, "%s %d", (char *)vector_get(star_names, current_name++), nn->depth);
	Planet *p = malloc(sizeof(Planet));
	nn->p = p;
	planet_set_default(nn->p, nn->f);
	planet_set_random(nn->p);

	nn->t = malloc(sizeof(Trade_Entity));
	trade_setup_entity(nn->t);
}

//yoinked from http://stackoverflow.com/a/14795484
int get_line_intersection(Travel_Node *p0, Travel_Node *p1, Travel_Node *p2, Travel_Node *p3)
{
	float s02_x, s02_y, s10_x, s10_y, s32_x, s32_y, s_numer, t_numer, denom, t;
	s10_x = p1->x - p0->x;
	s10_y = p1->y - p0->y;
	s32_x = p3->x - p2->x;
	s32_y = p3->y - p2->y;

	denom = s10_x * s32_y - s32_x * s10_y;
	if (denom == 0)
	{
		return false; // Collinear
	}

	bool denomPositive = denom > 0;

	s02_x = p0->x - p2->x;
	s02_y = p0->y - p2->y;
	s_numer = s10_x * s02_y - s10_y * s02_x;
	if ((s_numer < 0) == denomPositive)
	{
		return false; // No collision
	}

	t_numer = s32_x * s02_y - s32_y * s02_x;
	if ((t_numer < 0) == denomPositive)
	{
		return false; // No collision
	}

	if (((s_numer > denom) == denomPositive) || ((t_numer > denom) == denomPositive))
	{
		return false; // No collision
	}

	// Collision detected
	t = t_numer / denom;
	float i_x = p0->x + (t * s10_x);
	float i_y = p0->y + (t * s10_y);

	if ((i_x == p0->x && i_y == p0->y) || (i_x == p1->x && i_y == p1->y) || (i_x == p2->x && i_y == p2->y) || (i_x == p3->x && i_y == p3->y))
	{
		return false; //Collision is at node intersection and not relevant
	}

	return true;
}

//Return whether or not this connection is safe
bool check_connection(Vector *node_list, Travel_Node *na1, Travel_Node *na2)
{
	Travel_Node *nb1;
	Travel_Node *nb2;

	for (int i = 0; i < vector_get_size(node_list); ++i)
	{
		nb1 = vector_get(node_list, i);

		//Loop through each of this node's connections
		for (int j = 0; j < sizeof(nb1->connections) / sizeof(Travel_Node *); ++j)
		{
			if (nb1->connections[j] == NULL)
			{
				continue;
			}
			else
			{
				nb2 = nb1->connections[j];
			}
			
			if ((nb1 == nb2) || ((nb1 == na1 && nb2 == na2) || (nb1 == na2 && nb2 == na1)))
			{
				continue;
			}

			if (get_line_intersection(na1, na2, nb1, nb2) == true)
			{
				return false;
			}
		}
	}
	return true;
}

bool add_connection(Travel_Node *local_node, Travel_Node *remote_node)
{
	bool connection_success = false;

	//Set up the local side of the node connection
	for (int i = 0; i < sizeof(local_node->connections) / sizeof(Travel_Node *); ++i)
	{
		if (local_node->connections[i] == NULL)
		{
			local_node->connections[i] = remote_node;
			connection_success = true;
			break;
		}
	}

	if(connection_success)
	{
		//Set up the remote side of the node connection
		for (int i = 0; i < sizeof(remote_node->connections) / sizeof(Travel_Node *); ++i)
		{
			if (remote_node->connections[i] == NULL)
			{
				remote_node->connections[i] = local_node;
				break;
			}
		}
	}

	return connection_success;
}

void configure_connections(Vector *node_list, Travel_Node *n, int max_connection_dist)
{
	if (!n->connected)
	{
		int c[4] = {0};
		Travel_Node *close_node[4] = {NULL};

		//Work out the four closest nodes that have connections available
		for (int i = 0; i < vector_get_size(node_list); ++i)
		{
			Travel_Node *on = (Travel_Node *)vector_get(node_list, i);
			bool available_connection = false;
			for (int j = 0; j < sizeof(on->connections) / sizeof(Travel_Node *); ++j)
			{
				if (on->connections[j] == NULL)
				{
					available_connection = true;
					break;
				}
			}
			if (on != n && available_connection)
			{
				int d = (on->x - n->x) * (on->x - n->x) + (on->y - n->y) * (on->y - n->y);
				if ((c[0] == 0 || d < c[0]) && d < max_connection_dist * max_connection_dist)
				{
					if (check_connection(node_list, n, on))
					{
						c[3] = c[2];
						c[2] = c[1];
						c[1] = c[0];
						c[0] = d;

						close_node[3] = close_node[2];
						close_node[2] = close_node[1];
						close_node[1] = close_node[0];
						close_node[0] = on;
					}
				}
				else if ((c[1] == 0 || d < c[1]) && d < max_connection_dist * max_connection_dist)
				{
					if (check_connection(node_list, n, on))
					{
						c[3] = c[2];
						c[2] = c[1];
						c[1] = d;

						close_node[3] = close_node[2];
						close_node[2] = close_node[1];
						close_node[1] = on;
					}
				}
				else if ((c[2] == 0 || d < c[2]) && d < max_connection_dist * max_connection_dist)
				{
					if (check_connection(node_list, n, on))
					{
						c[3] = c[2];
						c[2] = d;

						close_node[3] = close_node[2];
						close_node[2] = on;
					}
				}
				else if ((c[3] == 0 || d < c[3]) && d < max_connection_dist * max_connection_dist)
				{
					if (check_connection(node_list, n, on))
					{
						c[3] = d;

						close_node[3] = on;
					}
				}
			}
		}
		if (close_node[0] != NULL)
		{
			if (!add_connection(n, close_node[0]))
			{
				//We're full up. There's no more we can do here.
				n->connected = true;
				return;
			}
			else if (close_node[1] != NULL)
			{
				if (!add_connection(n, close_node[1]))
				{
					//We're full up. There's no more we can do here.
					n->connected = true;
					return;
				}
				else if (close_node[2] != NULL)
				{
					if (!add_connection(n, close_node[2]))
					{
						//We're full up. There's no more we can do here.
						n->connected = true;
						return;
					}
					else if (close_node[3] != NULL)
					{
						if (!add_connection(n, close_node[3]))
						{
							//We're full up. There's no more we can do here.
							n->connected = true;
							return;
						}
					}
				}
			}
		}
	}

	for (int i = 0; i < sizeof(n->connections) / sizeof(Travel_Node *); ++i)
	{
		bool connections_full = true;
		if (n->connections[i] == NULL)
		{
			connections_full = false;
			break;
		}
		n->connected = connections_full;
	}
}

void make_tree(Vector *node_list, Travel_NodeDefs defs, int root_x, int root_y)
{
	Travel_Node *root = malloc(sizeof(Travel_Node));
	root->x = root_x;
	root->y = root_y;
	root->f = defs.faction;
	root->depth = 0;
	root->connected = false;
	for (int i = 0; i < sizeof(root->connections) / sizeof(Travel_Node *); ++i)
	{
		root->connections[i] = NULL;
	}

	vector_add(node_list, root);
	make_child_nodes(node_list, root, defs.max_depth, defs.spread, defs.jitter, defs.merge_dist, defs.bounds, defs.page_bounds, 0);
	make_child_nodes(node_list, root, defs.max_depth, defs.spread, defs.jitter, defs.merge_dist, defs.bounds, defs.page_bounds, 1);
	make_child_nodes(node_list, root, defs.max_depth, defs.spread, defs.jitter, defs.merge_dist, defs.bounds, defs.page_bounds, 2);
	make_child_nodes(node_list, root, defs.max_depth, defs.spread, defs.jitter, defs.merge_dist, defs.bounds, defs.page_bounds, 3);

	if (defs.faction < 0)
	{
		root->f = rand() % 4;
	}
	sprintf(root->node_name, "%s", faction_homeworlds[root->f]);
	Planet *p = malloc(sizeof(Planet));
	root->p = p;
	planet_set_default(root->p, root->f);
	planet_set_random(root->p);

	root->t = malloc(sizeof(Trade_Entity));
	trade_setup_entity(root->t);
}

void make_connections(Vector *node_list, Travel_NodeDefs defs)
{
	//TODO: There are better breadth-first search strategies
	for (int i = 0; i <= defs.max_depth; ++i)
	{
		for (int j = 0; j < vector_get_size(node_list); ++j)
		{
			Travel_Node *n = (Travel_Node *)vector_get(node_list, j);
			if (n->depth == i)
			{
				configure_connections(node_list, n, defs.max_connection_dist);
			}
		}
	}
}

void remove_orphan_nodes(Vector *node_list)
{
	for (int i = 0; i < vector_get_size(node_list); ++i)
	{
		Travel_Node *n = (Travel_Node *)vector_get(node_list, i);
		if (!n->connected)
		{
			int connection_count = 0;
			for (int j = 0; j < sizeof(n->connections) / sizeof(Travel_Node *); ++j)
			{
				if (n->connections[j] != NULL)
				{
					connection_count ++;
				}
			}

			if (connection_count == 0)
			{
				vector_remove(node_list, i);
			}
		}
	}
}

void starmap_draw(Vector *node_list)
{
	SDL_SetRenderDrawColor(main_renderer, 0, 0, 0, 255);
	SDL_RenderClear(main_renderer);
	SDL_Texture* newtexture = SDL_CreateTexture(main_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, main_resX, main_resY);
	//Set the new texture as the render target
	SDL_SetRenderTarget(main_renderer, newtexture);
	SDL_RenderClear(main_renderer);
	Travel_Node *cn = (Travel_Node *)vector_get(node_list, current_node);
	if(t_sectorX<256 && t_sectorY<256){
		main_blit(t_stars1,0-t_sectorX,0-t_sectorY, NOFLIP, NULL);
	}
	if(t_sectorX>=192 && t_sectorY<256){
		main_blit(t_stars1,256-t_sectorX,0-t_sectorY, FLIPH, NULL);
	}
	if(t_sectorX<256 && t_sectorY>=192){
		main_blit(t_stars1,0-t_sectorX,256-t_sectorY, FLIPV, NULL);
	}
	if(t_sectorX>=192 && t_sectorY>=192){
		main_blit(t_stars1,256-t_sectorX,256-t_sectorY, FLIPHV, NULL);
	}

	for (int i = 0; i < vector_get_size(node_list); ++i)
	{
		//Render connections before nodes so that nodes can be on top
		Travel_Node *n = (Travel_Node *)vector_get(node_list, i);
		Travel_Node *nn;
		
		for (int j = 0; j < sizeof(n->connections) / sizeof(Travel_Node *); ++j)
		{
			if (n->connections[j] != NULL)
			{
				SDL_SetRenderDrawBlendMode(main_renderer, SDL_BLENDMODE_BLEND);
				int a = 32;
				nn = n->connections[j];
				if (n == cn || nn == cn)
				{
					a = 92;
				}
				if (n->f == nn->f)
				{
					SDL_SetRenderDrawColor(main_renderer, faction_colors[n->f].r, faction_colors[n->f].g, faction_colors[n->f].b, a);
				}
				else
				{
					SDL_SetRenderDrawColor(main_renderer, faction_colors[FACTION_NONE].r, faction_colors[FACTION_NONE].g, faction_colors[FACTION_NONE].b, a);
				}
				
				SDL_RenderDrawLine(main_renderer, n->x-t_sectorX, n->y-t_sectorY, nn->x-t_sectorX, nn->y-t_sectorY);
			}
		}
		SDL_SetRenderDrawColor(main_renderer, 0, 0, 0, 255);
	}
	for (int i = 0; i < vector_get_size(node_list); ++i)
	{
		Travel_Node *n = (Travel_Node *)vector_get(node_list, i);

		SDL_SetRenderDrawColor(main_renderer, faction_colors[n->f].r, faction_colors[n->f].g, faction_colors[n->f].b, faction_colors[n->f].a);
		int x=n->x - t_sectorX;
		int y=n->y - t_sectorY;
/*
		SDL_RenderDrawPoint(main_renderer,  x-1, y);
		SDL_RenderDrawPoint(main_renderer,  x, y-1);
		SDL_RenderDrawPoint(main_renderer,  x-1, y-1);
		SDL_RenderDrawPoint(main_renderer,  x+1, y);
		SDL_RenderDrawPoint(main_renderer,  x, y+1);
		SDL_RenderDrawPoint(main_renderer,  x+1, y+1);
		SDL_RenderDrawPoint(main_renderer,  x, y);
		SDL_RenderDrawPoint(main_renderer,  x-1, y+1);*/
		SDL_RenderDrawPoint(main_renderer,  x+3, y-3);
	}

	SDL_SetRenderTarget(main_renderer, NULL);
	SDL_RenderCopy(main_renderer, newtexture, NULL, NULL);
	SDL_DestroyTexture(newtexture);

	if (strcmp(current_star, "") != 0)
	{
		int w = 0;
		for (int i = 0; i < strlen(current_star); ++i)
		{
			w += fonts[(int)current_star[i]].a;
		}
		draw_text((64 - w) / 2, 2, current_star, strlen(current_star), FONT_EARTH, -1, -1, GUI_DEFAULT_COLOR);
	}
}

void clear_node(Travel_Node *n)
{
	if (n->p != NULL)
	{
		free(n->p);
	}
	for (int i = 0; i < sizeof(n->connections) / sizeof(Travel_Node *); ++i)
	{
		n->connections[i] = NULL;
	}
}

void starmap_clear(Vector *node_list)
{
	while (vector_get_size(node_list) > 0)
	{
		Travel_Node *n = (Travel_Node *)vector_get(node_list, 0);
		vector_remove(node_list, 0);
		clear_node(n);
		free(n);
	}
}

int get_node_closest_to(Vector *node_list, int x, int y)
{
	int closest_node = -1;
	int closest_x = -1;
	int closest_y = -1;

	for (int i = 0; i < vector_get_size(node_list); ++i)
	{
		Travel_Node *n = (Travel_Node *)vector_get(node_list, i);
		if ((abs(x - n->x) < closest_x || closest_x < 0) && (abs(y - n->y) < closest_y || closest_y < 0))
		{
			closest_x = abs(x - n->x);
			closest_y = abs(y - n->y);
			closest_node = i;
		}
	}

	return closest_node;
}

void generate_starmap(Vector *node_list)
{
	//TODO: We'll need to set this somewhere else, or maybe pass it in?
	srand(seed);

	current_name = 0;

	Travel_NodeDefs defs;
	defs.merge_dist = 20;
	defs.max_depth = 5;
	defs.spread =  10;
	defs.jitter = 25;
	defs.bounds = 500;
	defs.page_bounds = 64;
	defs.max_connection_dist = 75; //something around merge_dist + spread + jitter
	defs.faction = 1; //-1 gives nodes random factions

	//int root_x = defs.bounds / 2 + 32;
	//int root_y = defs.bounds / 2 + 32;

	int root_x = 128 + 32;
	int root_y = 128 - 32;

	make_tree(node_list, defs, root_x, root_y);


	root_x = 128 + 32;
	root_y = 384 - 32;
	defs.faction = 2; //-1 gives nodes random factions
	make_tree(node_list, defs, root_x, root_y);

	root_x = 384 + 32;
	root_y = 384 - 32;
	defs.faction = 3; //-1 gives nodes random factions
	make_tree(node_list, defs, root_x, root_y);

	root_x = 384 + 32;
	root_y = 128 - 32;
	defs.faction = 0; //-1 gives nodes random factions
	make_tree(node_list, defs, root_x, root_y);

	make_connections(node_list, defs);
	remove_orphan_nodes(node_list);

	current_node = get_node_closest_to(starmap, 280, 250);
	t_sectorX=round(((Travel_Node *)vector_get(starmap, current_node))->x/64)*64;
	t_sectorY=round(((Travel_Node *)vector_get(starmap, current_node))->y/64)*64;
	printf("Generated starmap with %d nodes, using a seed of %d\n", vector_get_size(starmap), seed);
}
