#include <pango2/pangocairo.h>
#include "oldenglish.h"
#include "script.h"

void layout_select_font(Pango2Layout *pango_layout, const char* desc) {
  Pango2FontDescription *font_desc = pango2_font_description_from_string(desc);
  pango2_layout_set_font_description(pango_layout, font_desc);
  pango2_font_description_free(font_desc);
}

void fontmap_load_font(Pango2FontMap *fontmap, unsigned char data[], unsigned int size, const char* family) {
    hb_blob_t *blob = hb_blob_create((const char *)data, size, HB_MEMORY_MODE_READONLY, NULL, NULL);
    hb_face_t *hb_face = hb_face_create(blob, 0);
    hb_face_make_immutable(hb_face);
    hb_blob_destroy(blob);

    Pango2FontDescription *desc = pango2_font_description_new();
    pango2_font_description_set_family(desc, family);
    Pango2HbFace *pango2_hbface = pango2_hb_face_new_from_hb_face(hb_face, -1, NULL, desc);
    pango2_font_description_free(desc);

    pango2_font_map_add_face(fontmap, PANGO2_FONT_FACE(pango2_hbface));
}

void show_on_baseline(cairo_t *ctx, int x, int baseline, Pango2Layout *pango_layout)
{
    pango2_cairo_update_layout(ctx, pango_layout);
    Pango2Lines *lines = pango2_layout_get_lines(pango_layout);
    int adj = pango2_lines_get_baseline(lines) / 1024;
    cairo_move_to(ctx, x, baseline - adj);
    pango2_cairo_show_layout(ctx, pango_layout);
}

int main() {

    cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 2560, 1440);
    cairo_t *ctx = cairo_create(surface);

    cairo_font_options_t *font_options = cairo_font_options_create();
    cairo_font_options_set_hint_style(font_options, CAIRO_HINT_STYLE_FULL);
    cairo_font_options_set_antialias(font_options, CAIRO_ANTIALIAS_SUBPIXEL);
    cairo_set_font_options(ctx, font_options);

    Pango2FontMap *fontmap = pango2_font_map_new_default();
    fontmap_load_font(fontmap, script, script_len, "Script");
    fontmap_load_font(fontmap, oldenglish, oldenglish_len, "Old");
    Pango2Context *pango_ctx = pango2_cairo_create_context(ctx);
    pango2_context_set_font_map(pango_ctx, fontmap);

    cairo_set_source_rgb(ctx, 0.9, 0.9, 0.75);
    cairo_paint(ctx);
    cairo_set_source_rgb(ctx, 0.0, 0.2, 1.0);

    Pango2Layout *pango_layout = pango2_layout_new(pango_ctx);
    pango2_layout_set_text(pango_layout, "Hello, Pango2 1.9!", -1);
    layout_select_font(pango_layout, "Old 120");
    show_on_baseline(ctx, 150, 450, pango_layout);
    layout_select_font(pango_layout, "Script 100");
    show_on_baseline(ctx, 1280, 450, pango_layout);

    layout_select_font(pango_layout, "Serif 120");
    const char* markup = "<span foreground='red'><span font='Script' underline='solid'> Red </span><span font='Old'>Text</span></span> is <i><span underline='dotted'>Cool</span></i>!";
    pango2_layout_set_markup (pango_layout, markup, -1);
    cairo_move_to(ctx, 600, 800);
    pango2_cairo_update_layout(ctx, pango_layout);
    pango2_cairo_show_layout(ctx, pango_layout);

    cairo_surface_write_to_png(surface, "output.png");

    g_object_unref(pango_ctx);
    cairo_destroy(ctx);
    cairo_surface_destroy(surface);
    g_object_unref(pango_layout);
    g_object_unref(fontmap);

    return EXIT_SUCCESS;
}
