#include "draw.h"

void
drawGraph(struct Graph *G,
          cairo_surface_t *surface)
{
    cairo_t *cr = cairo_create(surface);

    int i, j;
    for (i = 0; i < G->size; i ++) {
        for (j = 0; j < G->size; j ++) {
            cairo_rectangle(cr,
                            i * BLOCK_SIZE,
                            j * BLOCK_SIZE,
                            BLOCK_SIZE,
                            BLOCK_SIZE);
            if (G->data[i][j] == Graph_obstacle) {
                cairo_set_source_rgb(cr, OBSTACLE_COLOR);
            } else if (G->data[i][j] == Graph_outset) {
                cairo_set_source_rgb(cr, OUTSET_COLOR);
            } else if (G->data[i][j] == Graph_destination) {
                cairo_set_source_rgb(cr, DESTINATION_COLOR);
            } else if (G->data[i][j] == Graph_road) {
                cairo_set_source_rgb(cr, ROAD_COLOR);
            }
            cairo_fill(cr);
        }
    }

    if (!Graph_solved(G)) {
        cairo_select_font_face(cr, "Sans",
                                    CAIRO_FONT_SLANT_NORMAL,
                                    CAIRO_FONT_WEIGHT_BOLD);
        cairo_set_source_rgb(cr, 1, 1, 1);
        cairo_set_font_size(cr, 20);
        cairo_move_to(cr, 20, 23);
        cairo_show_text(cr, "There is no solution!");    
    }

    cairo_destroy(cr);
}

gboolean
drawPath(GtkWidget *widget,
         struct Graph *G,
         cairo_surface_t *surface)
{
    if (Stack_empty(G->path)) {
        return FALSE;
    }

    cairo_t *cr = cairo_create(surface);

    struct Point pos = Stack_top(G->path);
    Stack_pop(G->path);
    
    cairo_arc(cr,
              (pos.x + 0.5) * BLOCK_SIZE,
              (pos.y + 0.5) * BLOCK_SIZE,
              BLOCK_SIZE / 3,
              0,
              2 * acos(-1));

    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_fill(cr);

    cairo_destroy(cr);
    gtk_widget_queue_draw_area(widget,
                               pos.x * BLOCK_SIZE,
                               pos.y * BLOCK_SIZE,
                               BLOCK_SIZE,
                               BLOCK_SIZE);

    return TRUE;
}
