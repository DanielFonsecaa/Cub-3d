#include "../includes/cub.h"

 
#include <stdio.h>
#include <stdint.h>
/* Helper: safe print of pointer as hex and (if char*) as string */
static void print_ptr_hex(const char *name, const void *p)
{
    printf("  %s: %p\n", name, p);
}

static void print_str_field(const char *name, const char *s)
{
    if (!s)
        printf("  %s: (NULL)\n", name);
    else
        printf("  %s: \"%s\"\n", name, s);
}

/* Print a null-terminated array of strings (char **) */
static void print_string_array(const char *name, char **arr)
{
    if (!arr) {
        printf("  %s: (NULL)\n", name);
        return;
    }

    printf("  %s:\n", name);
    for (size_t i = 0; arr[i] != NULL; ++i) {
        printf("    [%zu] %s", i, arr[i]);
    }
}

/* Print t_color */
void dbg_print_color(const char *name, const t_color *c)
{
    if (!c) {
        printf("%s: (NULL)\n", name);
        return;
    }
    printf("%s:\n", name);
    printf("  red:   %d\n", c->red);
    printf("  green: %d\n", c->green);
    printf("  blue:  %d\n", c->blue);
}

/* Print t_texture */
void dbg_print_texture(const char *name, const t_texture *tx)
{
    if (!tx) {
        printf("%s: (NULL)\n", name);
        return;
    }

    printf("%s:\n", name);
    print_ptr_hex("img", tx->img);
    print_str_field("data (ptr)", tx->data);
    printf("  width:     %d\n", tx->width);
    printf("  height:    %d\n", tx->height);
    printf("  bpp:       %d\n", tx->bpp);
    printf("  size_line: %d\n", tx->size_line);
    printf("  endian:    %d\n", tx->endian);
    print_str_field("path", tx->path);
}

/* Print t_mapi (map file/meta + map lines) */
void dbg_print_mapi(const char *name, const t_mapi *m)
{
    if (!m) {
        printf("%s: (NULL)\n", name);
        return;
    }

    printf("%s:\n", name);
    print_string_array("map", m->map);
    print_str_field("file_name", m->file_name);
    printf("  height:     %d\n", m->height);
    printf("  skip_lines: %d\n", m->skip_lines);
    printf("  fd:         %d\n", m->fd);
}

/* Weak stubs for player and canvas printing.
 * If you want field-level prints provide your own implementations in your project:
 * void dbg_print_player(const char *name, t_player *p) { ... }
 * void dbg_print_canvas(const char *name, t_canvas *c) { ... }
 *
 * The weak attribute lets these be overridden by user definitions.
 */
void dbg_print_player(const char *name, t_player *p) __attribute__((weak));
void dbg_print_canvas(const char *name, t_canvas *c) __attribute__((weak));

void dbg_print_player(const char *name, t_player *p)
{
    /* Default fallback: print pointer/address and a note. */
    if (!p) {
        printf("%s: (NULL)\n", name);
        return;
    }
    printf("%s: (player struct at %p) -- implement dbg_print_player() to show fields\n", name, (void*)p);
    printf("x: %.5f\n", p->x);
    printf("y: %.5f\n", p->y);
    printf("angle: %.5f\n", p->angle);
}

void dbg_print_canvas(const char *name, t_canvas *c)
{
    if (!c) {
        printf("%s: (NULL)\n", name);
        return;
    }
    printf("%s: (canvas struct at %p) -- implement dbg_print_canvas() to show fields\n", name, (void*)c);
}

/* Print the t_game struct (top-level) */
void dbg_print_game(const t_game *g)
{
    if (!g) {
        printf("t_game: (NULL)\n");
        return;
    }

    printf("t_game at %p\n", (void*)g);

    print_str_field("data", g->data);
    printf("bpp: %d\n", g->bpp);
    printf("size_line: %d\n", g->size_line);
    printf("endian: %d\n", g->endian);

    /* game.map is char ** */
    print_string_array("map", g->map);

    printf("assets_ready: %d\n", g->assets_ready);
    printf("floor_set: %d\n", g->floor_set);
    printf("ceiling_set: %d\n", g->ceiling_set);

    dbg_print_color("floor", &g->floor);
    dbg_print_color("cealing", &g->cealing);

    /* player and canvas: call printers (may be weak stubs) */
    dbg_print_player("player", (t_player *)&g->player);
    dbg_print_canvas("canvas", (t_canvas *)&g->canvas);

    /* Textures */
    dbg_print_texture("north", &g->north);
    dbg_print_texture("south", &g->south);
    dbg_print_texture("east", &g->east);
    dbg_print_texture("west", &g->west);

    /* grid (t_mapi) */
    dbg_print_mapi("grid", &g->grid);
}

/* Optional: a convenience printer that prints a separator */
void dbg_print_separator(const char *title)
{
    if (title)
        printf("\n===== %s =====\n", title);
    else
        printf("\n====================\n");
}

void some_debug_function(t_game *game)
{
	dbg_print_separator("GAME STATE");
	dbg_print_game(game);
}

int	main(int argc, char **argv)
{
	t_game	game;

	if (argc != 2)
	{
		ft_printf("Usage: %s <map_file>\n", argv[0]);
		return (1);
	}
	ft_bzero(&game, sizeof(t_game));
	parsing(&game, argv[1]);
	some_debug_function(&game);
	if (!game.grid.map)
	{
		perror("get_map");
		exit(1);
	}
	start(&game);
	return (0);
}
