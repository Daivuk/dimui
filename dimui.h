#pragma once

#include <cinttypes>
#include <string>

//
//--- types
//
struct dimui_rect_t
{
    int x, y, w, h;
};

struct dimui_color_t
{
    float r, g, b, a;
};

struct dimui_vec2i_t
{
    int x, y;
};

struct dimui_vec2_t
{
    float x, y;
};

struct dimui_vertex_t
{
    dimui_vec2_t position;
    dimui_vec2_t uv;
    dimui_color_t color;
};

enum dimui_draw_cmd_type_t
{
    DIMUI_DRAW_CMD_TYPE_DRAW,
    DIMUI_DRAW_CMD_TYPE_SCISSORS
};

struct dimui_draw_cmd_t
{
    union
    {
        struct
        {
            uintptr_t texture;
            int start_vertex;
            int vertex_count;
        };
        dimui_rect_t scissors;
    };
    dimui_draw_cmd_type_t cmd_type;
};

struct dimui_draw_frame_data_t
{
    int vertex_count;
    dimui_vertex_t* vertices;
    int cmd_count;
    dimui_draw_cmd_t* cmds;
};

typedef uintptr_t(*load_texture_fn_t)(const void* data, int w, int h);

struct dimui_init_desc_t
{
    load_texture_fn_t load_texture_fn;
};

struct dimui_new_frame_desc_t
{
    int w, h;
};


// Color conversion macros. To convert from hex or RGB to floats
#define DIMUI_RGB(r, g, b) { (float)(r) / 255.0f, (float)(g) / 255.0f, (float)(b) / 255.0f, 1.0f }
#define DIMUI_HEX(hex) DIMUI_RGB((hex >> 16) & 0xFF, (hex >> 8) & 0xFF, hex & 0xFF)

// Default dimui palette
#define DIMUI_PAL_RED0      DIMUI_HEX(0x5b2b2b)
#define DIMUI_PAL_RED1      DIMUI_HEX(0x984a49)
#define DIMUI_PAL_RED2      DIMUI_HEX(0xc59392)
#define DIMUI_PAL_RED3      DIMUI_HEX(0xf3dcdc)
#define DIMUI_PAL_BLUE0     DIMUI_HEX(0x2c495b)
#define DIMUI_PAL_BLUE1     DIMUI_HEX(0x497a98)
#define DIMUI_PAL_BLUE2     DIMUI_HEX(0x92b1c5)
#define DIMUI_PAL_BLUE3     DIMUI_HEX(0xdce9f3)
#define DIMUI_PAL_YELLOW0   DIMUI_HEX(0x925f3d)
#define DIMUI_PAL_YELLOW1   DIMUI_HEX(0xc99350)
#define DIMUI_PAL_YELLOW2   DIMUI_HEX(0xcfbd5e)
#define DIMUI_PAL_YELLOW3   DIMUI_HEX(0x7d835c)
#define DIMUI_PAL_GRAY0     DIMUI_HEX(0x1e1a1e)
#define DIMUI_PAL_GRAY1     DIMUI_HEX(0x433a43)
#define DIMUI_PAL_GRAY2     DIMUI_HEX(0x716271)
#define DIMUI_PAL_GRAY3     DIMUI_HEX(0xaca2ac)

#define DIMUI_DARK_FONT_COLOR   DIMUI_PAL_GRAY0
#define DIMUI_LIGHT_FONT_COLOR  DIMUI_PAL_RED3
#define DIMUI_WORKSPACE_COLOR   DIMUI_PAL_GRAY1


bool dimui_init(const dimui_init_desc_t& init_desc);
void dimui_shutdown();
void dimui_new_frame(const dimui_new_frame_desc_t& new_frame_desc);
const dimui_draw_frame_data_t* dimui_get_draw_frame_data();
bool dimui_begin_menu(const std::string& text, bool enabled, char shortcut_character);
void dimui_end_menu();
bool dimui_menu_item(const std::string& text, bool enabled, char shortcut_character);
void dimui_separator();
