#include <stdio.h>
#include <gtk/gtk.h>
#include <string.h>
#include <math.h>
#include <time.h>

typedef struct {
    double x;
    double y;
    double size;
    cairo_pattern_t *gradient;
}txtInstance;

typedef struct {
    GtkWidget *area;
    GArray *instances;
    int count;
    GRand *rand;
}state;

static int isInShape(double x, double y) {
    double a = x*x+y*y-1;
    return 0.2*a*a*a-x*x*y*y*y <= 0;
}

static int isInShape2(double x, double y) {

}

static void getInstance(state *s ,txtInstance *instance) {
    do {
        instance -> x = g_rand_double_range(s->rand, -1.8,1.8);
        instance -> y = g_rand_double_range(s->rand, -1.8,1.8);
    }while (!isInShape(instance->x, instance->y));
    instance -> size = g_rand_double_range(s->rand, 0.05,0.10);
    instance -> gradient = cairo_pattern_create_linear(instance->x, 0,instance->x + instance->size *6, 0);
    cairo_pattern_add_color_stop_rgba(instance->gradient,0.0, 1.0, 0, g_rand_double_range(s->rand, 0,1), 0.8);
    cairo_pattern_add_color_stop_rgba(instance->gradient,1.0, 1.0, 0, g_rand_double_range(s->rand, 0,1), 0.8);
    return;
}

static gboolean on_timer(gpointer data) {
    state *s = data;
    s->count++;
    txtInstance t;
    getInstance(s, &t);
    g_array_append_val(s->instances, t);

    gtk_widget_queue_draw(s->area);

    if (s->count >= 1500) {
        return G_SOURCE_REMOVE;
    }else {
        return G_SOURCE_CONTINUE;
    }
}

static void draw_cb(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer user_data) {
    state *s = user_data;

    cairo_set_source_rgba(cr, 0, 0, 0, 1.0);
    cairo_paint(cr);

    double scale = MIN(width, height) / 3.0;
    cairo_translate(cr, width / 2.0, height / 2.0+70);
    cairo_scale(cr, scale, scale);

    for (int i = 0; i < s->instances->len; i++) {
        txtInstance *t = &g_array_index(s->instances, txtInstance, i);

        cairo_set_source(cr, t->gradient);
        cairo_set_font_size(cr, t->size);
        cairo_move_to(cr, t->x, -t->y);
        cairo_show_text(cr, "............................");

    }
}

static void on_activate(GtkApplication *app, gpointer user_data) {
    state *s = user_data;
    GtkWidget *win = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(win), "C_GTK_APP");
    gtk_window_set_default_size(GTK_WINDOW(win), 1920, 1080);

    GtkWidget *area = gtk_drawing_area_new();
    s->area = area;
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(area), draw_cb, s, NULL);
    gtk_window_set_child(GTK_WINDOW(win), area);
    gtk_window_present(GTK_WINDOW(win));



    g_timeout_add(10, on_timer, s);
}

int main(int argc, char **argv) {
    state s = {0};
    s.instances = g_array_new(FALSE, FALSE, sizeof(txtInstance));
    s.rand = g_rand_new();
    GtkApplication *app = gtk_application_new("org.example.cairo", G_APPLICATION_DEFAULT_FLAGS);

    g_signal_connect(app, "activate", G_CALLBACK(on_activate), &s);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    g_array_free(s.instances, TRUE);
    g_rand_free(s.rand);
    return status;
}