#include "gui.h"

gboolean
configureEvent(GtkWidget *widget,
                 gpointer data)
{
    if (surface) {
        cairo_surface_destroy(surface);
    }

    surface = gdk_window_create_similar_surface(
        gtk_widget_get_window(widget),
        CAIRO_CONTENT_COLOR,
        gtk_widget_get_allocated_width(widget),
        gtk_widget_get_allocated_height(widget));

    drawGraph(graph, surface);

    return TRUE;
}

gboolean
drawMaze(GtkWidget *widget,
         cairo_t   *cr,
         gpointer   data)
{
    cairo_set_source_surface(cr, surface, 0, 0);
    cairo_paint(cr);

    return FALSE;
}

gboolean
drawPathTimer(gpointer data)
{
    struct PointerContianer *PC = data;
    GtkWidget *drawing_area = PointerContianer_get(PC, "drawing_area");
    guint timer = *(guint*)PointerContianer_get(PC, "pointer_to_timer");

    if (!drawPath(drawing_area, graph, surface)) {
        g_source_remove(timer);
    }
    return TRUE;
}

void
closeWindowMaze()
{
    if (surface) {
        cairo_surface_destroy(surface);
    }
}

void
closeWindowInput(GtkWidget *window_input,
                 gpointer   data)
{
    struct PointerContianer *PC1, *PC2;
    GtkWidget *window_maze, *drawing_area;

    PC1 = data;
    PC2 = PointerContianer_new(NULL);

    window_maze = PointerContianer_get(PC1, "window_maze");
    drawing_area = PointerContianer_get(PC1, "drawing_area");

    gtk_widget_show_all(window_maze);

    guint timer = g_timeout_add(300, (GSourceFunc)drawPathTimer, PC2);
    guint *pointer_to_timer = (guint*)malloc(sizeof(guint));
    *pointer_to_timer = timer;

    *PC2 = PointerContianer_make(
               "drawing_area",     drawing_area,
               "pointer_to_timer", pointer_to_timer,
               NULL);
}

gboolean
buttonClickedCallback(GtkWidget *button,
                      gpointer   data)
{
    int size;
    struct PointerContianer *PC = data;

    GtkWidget *drawing_area = PointerContianer_get(PC, "drawing_area");

    /* set size */
    gchar *size_text = (gchar*)gtk_entry_get_text(
        GTK_ENTRY(PointerContianer_get(PC, "maze_size_entry")));
    sscanf(size_text, "%d", &size);

    /* vaildate size */
    if (size < 4 || size > 99) {
        size = 15;
    }

    gtk_widget_set_size_request(drawing_area, size * BLOCK_SIZE, size * BLOCK_SIZE);

    /* set random mode */
    char random_mode;
    if (gtk_toggle_button_get_active(PointerContianer_get(PC, "radio_kru"))) {
        random_mode = MODE_KRUSKAL;
    } else if (gtk_toggle_button_get_active(PointerContianer_get(PC, "radio_nai"))) {
        random_mode = MODE_NAIVE;
    }

    /* create graph */
    graph = Graph_new(size);
    Graph_randomlyGenerateGraph(graph, random_mode);
    Graph_findDestination(graph);

    gtk_widget_destroy(GTK_WIDGET(PointerContianer_get(PC, "window_input")));
    return FALSE;
}

void
activate(GtkApplication *app,
         gpointer        user_data)
{
    GtkWidget *window_maze = gtk_application_window_new(app);
    GtkWidget *frame = gtk_frame_new(NULL);
    GtkWidget *drawing_area = gtk_drawing_area_new();

    /* the window of maze */
    gtk_window_set_title(GTK_WINDOW(window_maze), "Maze");
    gtk_container_add(GTK_CONTAINER(window_maze), frame);
    g_signal_connect(window_maze, "destroy", G_CALLBACK(closeWindowMaze), NULL);

    /* drawing area */
    gtk_container_add(GTK_CONTAINER(frame), drawing_area);
    g_signal_connect(drawing_area, "draw", G_CALLBACK(drawMaze), NULL);
    g_signal_connect(drawing_area, "configure-event", G_CALLBACK(configureEvent), NULL);
    gtk_widget_set_events(drawing_area, gtk_widget_get_events(drawing_area));

    /* the window of input area */
    GtkWidget *window_input = gtk_application_window_new(app);
    GtkWidget *grid = gtk_grid_new();
    GtkWidget *maze_size_label = gtk_label_new(NULL);
    GtkWidget *maze_size_entry = gtk_entry_new();
    GtkWidget *generate_algorithm_label = gtk_label_new(NULL);
    GtkWidget *radio_kru = gtk_radio_button_new(NULL);
    GtkWidget *radio_nai = gtk_radio_button_new_from_widget(
        GTK_RADIO_BUTTON(radio_kru));
    GtkWidget *enter_button = gtk_button_new();

    gtk_window_set_title(GTK_WINDOW(window_input), "Size of maze");
    gtk_label_set_text(GTK_LABEL(maze_size_label), "Maze size");
    gtk_label_set_text(GTK_LABEL(generate_algorithm_label),
        "Algorithm of maze generation");
    gtk_button_set_label(GTK_BUTTON(radio_kru), "Kruskal's algorithm");
    gtk_button_set_label(GTK_BUTTON(radio_nai), "Naive algorithm");
    gtk_button_set_label(GTK_BUTTON(enter_button), "Enter");

    /*  input box */
    gtk_grid_attach(GTK_GRID(grid), maze_size_label,
        0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), maze_size_entry,
        1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), generate_algorithm_label,
        0, 1, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), radio_kru,
        0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), radio_nai,
        1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), enter_button,
        0, 3, 2, 1);
    gtk_container_add(GTK_CONTAINER(window_input), grid);

    g_signal_connect(enter_button, "clicked", G_CALLBACK(buttonClickedCallback),
        (gpointer)PointerContianer_new("window_input",    window_input,
                                       "maze_size_entry", maze_size_entry,
                                       "drawing_area",    drawing_area,
                                       "radio_kru",       radio_kru,
                                       "radio_nai",       radio_nai,
                                       NULL));
    g_signal_connect(window_input, "destroy", G_CALLBACK(closeWindowInput),
        (gpointer)PointerContianer_new("window_maze",  window_maze,
                                       "drawing_area", drawing_area,
                                       NULL));

    gtk_widget_show_all(window_input);
}
