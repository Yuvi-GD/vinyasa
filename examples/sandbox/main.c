#include "chitra.h"
#include <stdio.h>

static uint32_t rectangle = CHITRA_INVALID_HANDLE;
static uint32_t label = CHITRA_INVALID_HANDLE;
static int font = -1;
static int exit_status;
static int last_width, last_height;
static float text_width, text_height;
static const char title[] = "Vinyasa sandbox";

static void fail(const char* message)
{
    fprintf(stderr, "Sandbox: %s\n", message);
    exit_status = 1;
    chitra_quit();
}

static void on_init(void)
{
    font = chitra_load_font("Roboto", VINYASA_SANDBOX_FONT);
    if (font < 0) {
        fail("could not load the bundled font");
        return;
    }
    rectangle = chitra_create(CHITRA_PIPELINE_SHAPE);
    label = chitra_create(CHITRA_PIPELINE_TEXT);
    if (rectangle == CHITRA_INVALID_HANDLE || label == CHITRA_INVALID_HANDLE) {
        fail("could not create drawing elements");
        return;
    }
    chitra_rect_set_fill_color(rectangle, 0xFFB87932u);
    chitra_set_draw_order(label, 1);
    chitra_measure_text(font, 28.0f, title, &text_width, &text_height);
}

static void on_frame(void)
{
    const int width = chitra_width();
    const int height = chitra_height();
    if (exit_status || width <= 0 || height <= 0 ||
        (width == last_width && height == last_height)) return;

    /* Renderer check only; Vinyasa will supply these rectangles later. */
    const float box_width = width < 544 ? (float)width * 0.8f : 480.0f;
    const float box_height = height < 264 ? (float)height * 0.8f : 200.0f;
    const float x = ((float)width - box_width) * 0.5f;
    const float y = ((float)height - box_height) * 0.5f;
    if (!chitra_rect_set_bounds(rectangle, x, y, box_width, box_height) ||
        !chitra_text_set(label, font, 28.0f, title,
            ((float)width - text_width) * 0.5f,
            ((float)height - text_height) * 0.5f)) {
        fail("could not update drawing elements");
        return;
    }
    last_width = width;
    last_height = height;
    chitra_mark_dirty();
}

static void on_cleanup(void)
{
    if (label != CHITRA_INVALID_HANDLE) chitra_destroy(label);
    if (rectangle != CHITRA_INVALID_HANDLE) chitra_destroy(rectangle);
    if (font >= 0) chitra_unload_font(font);
}

int main(void)
{
    const Chitra_Config config = {
        .width = 800,
        .height = 480,
        .title = "Vinyasa sandbox",
        .on_init = on_init,
        .on_frame = on_frame,
        .on_cleanup = on_cleanup
    };
    chitra_run(&config);
    return exit_status;
}
