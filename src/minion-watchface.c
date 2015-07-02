#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_time_layer;
static GFont s_time_font;
static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;


static void update_time() {
    // Get a tm structure
    time_t temp = time(NULL);
    struct tm *tick_time = localtime(&temp);

    // Create a long-lived buffer
    static char buffer[] = "00:00";

    // Write the current hours and minutes into the buffer
    if (clock_is_24h_style() == true) {
        // Use 24 hour format
        strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
    } else {
        // Use 12 hour format
        strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
    }

    // Display this time on the TextLayer
    text_layer_set_text(s_time_layer, buffer);
}


static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
    update_time();
}


static void main_window_load(Window *window) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "[minion-watchface.c:main_window_load]");

    s_time_font = fonts_get_system_font(FONT_KEY_ROBOTO_BOLD_SUBSET_49);

    // Create GBitmap, then set to created BitmapLayer
    s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_MINION_BG);
    s_background_layer = bitmap_layer_create(
        GRect(0, 0, 144, 168)
    );
    bitmap_layer_set_bitmap(
        s_background_layer,
        s_background_bitmap
    );
    layer_add_child(
        window_get_root_layer(window),
        bitmap_layer_get_layer(s_background_layer)
    );

    // Create time TextLayer
    s_time_layer = text_layer_create(GRect(5, 62, 139, 50));

    // Set Colors and Text
    text_layer_set_background_color(s_time_layer, GColorClear);
    text_layer_set_text_color(s_time_layer, GColorBlack);

    // Improve the layout to be more like a watchface
    text_layer_set_font(s_time_layer, s_time_font);
    text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

    // Add it as a child layer to the Window's root layer
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
}


static void main_window_unload(Window *window) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "[minion-watchface.c:main_window_unload] unloading window");

    // Destroy TextLayer
    text_layer_destroy(s_time_layer);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "[minion-watchface.c:main_window_unload] time layer destroyed");

    // Destroy GBitmap
    gbitmap_destroy(s_background_bitmap);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "[minion-watchface.c:main_window_unload] background bitmap destroyed");

    // Destroy BitmapLayer
    bitmap_layer_destroy(s_background_layer);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "[minion-watchface.c:main_window_unload] background layer destroyed");

    // Unregister with TickTimerService
    tick_timer_service_unsubscribe();
    APP_LOG(APP_LOG_LEVEL_DEBUG, "[minion-watchface.c:main_window_unload] timer service unsubscribed");
}


static void init() {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "[minion-watchface.c:init]");

    // Create main Window element and assign to pointer
    s_main_window = window_create();

    // Set handlers to manage the elements inside the Window
    window_set_window_handlers(s_main_window, (WindowHandlers) {
        .load = main_window_load,
        .unload = main_window_unload
    });

    // Show the Window on the watch, with animated=true
    window_stack_push(s_main_window, true);

    // Register with TickTimerService
    tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
    update_time();
}


static void deinit() {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "[minion-watchface.c:deinit] deinit");

    // Destroy Window
    window_destroy(s_main_window);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "[minion-watchface.c:deinit] window destroyed");
}


int main(void) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "[minion-watchface.c:main]");

    init();
    app_event_loop();
    deinit();
}
