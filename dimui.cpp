#include "dimui.h"
#include <unordered_map>
#include <vector>


// 64-bit FNV-1a
#define FNV_OFFSET_64 0xcbf29ce484222325ULL
#define FNV_PRIME_64  0x100000001b3ULL
uint64_t fnv1a_64(const void* data, size_t size)
{
    const unsigned char* ptr = (const unsigned char*)data;
    uint64_t hash = FNV_OFFSET_64;

    for (size_t i = 0; i < size; i++)
    {
        hash ^= ptr[i];
        hash *= FNV_PRIME_64;
    }

    return hash;
}


struct control_state
{
    dimui_rect_t rect;
};


static dimui_new_frame_desc_t frame_desc;
static dimui_draw_frame_data_t frame_data;
static std::vector<dimui_draw_cmd_t> cmds;
static std::vector<dimui_vertex_t> vertices;
static std::unordered_map<std::string, control_state> control_states;
static dimui_draw_cmd_t current_cmd;


static void draw_rect(const dimui_rect_t& rect, const dimui_color_t& color)
{
    dimui_vertex_t vert0 = { {(float)rect.x, (float)rect.y}, {0, 0}, color };
    dimui_vertex_t vert1 = { {(float)rect.x, (float)rect.y + (float)rect.h}, {0, 0}, color };
    dimui_vertex_t vert2 = { {(float)rect.x + (float)rect.w, (float)rect.y + (float)rect.h}, {0, 0}, color };
    dimui_vertex_t vert3 = { {(float)rect.x + (float)rect.w, (float)rect.y}, {0, 0}, color };

    vertices.push_back(vert0);
    vertices.push_back(vert1);
    vertices.push_back(vert2);
    vertices.push_back(vert2);
    vertices.push_back(vert3);
    vertices.push_back(vert0);

    current_cmd.vertex_count += 6;
}

static void flush()
{
    if (current_cmd.vertex_count > 0)
    {
        cmds.push_back(current_cmd);
        current_cmd.start_vertex = (int)vertices.size();
        current_cmd.vertex_count = 0;
    }
}


bool dimui_init(const dimui_init_desc_t& init_desc)
{
    return true;
}

void dimui_shutdown()
{
}

void dimui_new_frame(const dimui_new_frame_desc_t& new_frame_desc)
{
    frame_desc = new_frame_desc;
    vertices.clear();
    cmds.clear();

    // Set scissor
    dimui_draw_cmd_t cmd;
    cmd.cmd_type = DIMUI_DRAW_CMD_TYPE_SCISSORS;
    cmd.scissors = { 0, 0, frame_desc.w, frame_desc.h };
    cmds.push_back(cmd);

    current_cmd.cmd_type = DIMUI_DRAW_CMD_TYPE_DRAW;
    current_cmd.start_vertex = 0;
    current_cmd.vertex_count = 0;

    draw_rect({ 0, 0, frame_desc.w, frame_desc.h }, DIMUI_WORKSPACE_COLOR);
}

const dimui_draw_frame_data_t* dimui_get_draw_frame_data()
{
    flush();

    frame_data.cmds = cmds.data();
    frame_data.cmd_count = (int)cmds.size();
    frame_data.vertices = vertices.data();
    frame_data.vertex_count = (int)vertices.size();

    return &frame_data;
}

bool dimui_begin_menu(const std::string& text, bool enabled, char shortcut_character)
{
    return false;
}

void dimui_end_menu()
{
}

bool dimui_menu_item(const std::string& text, bool enabled, char shortcut_character)
{
    return false;
}

void dimui_separator()
{
}
