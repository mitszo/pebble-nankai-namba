
#include <pebble.h>

#define NUM_MENU_SECTIONS 2
#define NUM_MENU_ICONS 3
#define NUM_FIRST_MENU_ITEMS 3
#define NUM_SECOND_MENU_ITEMS 1

static Window *s_window;
static MenuLayer *s_menu_layer;
char trains[5][20] = {
 "07:55 [\u666e\u901a]\u307f",
 "07:57 [\u6025\u884c]\u548c",
 "08:00 [\u30e9\u03b1]\u7a7a",
 "08:02 [\u7a7a\u6025]\u7a7a",
 "08:05 [\u666e\u901a]\u548c"
};

static uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  switch (section_index) {
    case 0:
//      return sizeof(trains);
      return 5;

    default:
      return 0;
  }
}

static uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data) {
  return NUM_MENU_SECTIONS;
}

static void menu_draw_row_callback(
        GContext* ctx, const Layer *cell_layer,
        MenuIndex *cell_index, void *data) {
  if (cell_index->row <= sizeof(trains)) {
    menu_cell_basic_draw(ctx, cell_layer, trains[cell_index->row], NULL, NULL);
  }
}

static int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
    return MENU_CELL_BASIC_HEADER_HEIGHT;
}

static void menu_draw_header_callback(
        GContext* ctx, const Layer *cell_layer,
        uint16_t section_index, void *data) {

  switch (section_index) {
    case 0:
      // Draw title text in the section header
      menu_cell_basic_header_draw(ctx, cell_layer, "Nankai Namba Sta.");
      break;
  }
}

static void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
  // Use the row to specify which item will receive the select action
  switch (cell_index->row) {
  // This is the menu item with the cycling icon
  case 1:
    // Cycle the icon
    //s_current_icon = (s_current_icon + 1) % NUM_MENU_ICONS;
    // After changing the icon, mark the layer to have it updated
    layer_mark_dirty(menu_layer_get_layer(menu_layer));
    break;
  }
}

static void init(void) {
  // Create a window and get information about the window
  s_window = window_create();
  Layer *window_layer = window_get_root_layer(s_window);
  GRect bounds = layer_get_bounds(window_layer);
  
  // Create a menu layer
  s_menu_layer = menu_layer_create(bounds);
  menu_layer_set_callbacks(s_menu_layer, NULL, (MenuLayerCallbacks){
    .get_num_sections = menu_get_num_sections_callback,
    .get_num_rows = menu_get_num_rows_callback,
    .get_header_height = menu_get_header_height_callback,
    .draw_header = menu_draw_header_callback,
    .draw_row = menu_draw_row_callback,
    .select_click = menu_select_callback,
  });
  
  // Bind the menu layer's click config provider to the window for interactivity
  menu_layer_set_click_config_onto_window(s_menu_layer, s_window);
  
  layer_add_child(window_layer, menu_layer_get_layer(s_menu_layer));


  // Push the window, setting the window animation to 'true'
  window_stack_push(s_window, true);
  
  // App Logging!
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Just pushed a window!");
}

static void deinit(void) {
  // Destroy the menu layer
  menu_layer_destroy(s_menu_layer);
  
  // Destroy the window
  window_destroy(s_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}


