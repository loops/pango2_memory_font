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

int main() {

    cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 2560, 1440);
    cairo_t *ctx = cairo_create(surface);

    cairo_font_options_t *font_options = cairo_font_options_create();
    cairo_font_options_set_hint_style(font_options, CAIRO_HINT_STYLE_FULL);
    cairo_font_options_set_antialias(font_options, CAIRO_ANTIALIAS_SUBPIXEL);
    cairo_set_font_options(ctx, font_options);

    Pango2FontMap *fontmap = pango2_font_map_new();
    fontmap_load_font(fontmap, script, script_len, "Mandala Script");
    fontmap_load_font(fontmap, oldenglish, oldenglish_len, "Mandala Old");
    Pango2Context *pango_ctx = pango2_cairo_create_context(ctx);
    pango2_context_set_font_map(pango_ctx, fontmap);
    Pango2Layout *pango_layout = pango2_layout_new(pango_ctx);

    pango2_layout_set_text(pango_layout, "Hello, Pango2 1.9!", -1);

    cairo_set_source_rgb(ctx, 1.0, 1.0, 1.0);
    cairo_paint(ctx);
    cairo_set_source_rgb(ctx, 0.0, 0.2, 1.0);

    cairo_move_to(ctx, 0, 0);
    layout_select_font(pango_layout, "Mandala Old 230");
    pango2_cairo_update_layout(ctx, pango_layout);
    pango2_cairo_show_layout(ctx, pango_layout);

    cairo_move_to(ctx, 200, 400);
    layout_select_font(pango_layout, "Mandala Script 230");
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
