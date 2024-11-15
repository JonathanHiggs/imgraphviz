#pragma once

#include "imgraphviz/imgraphviz.h"

extern "C"
{
#include <graphviz/gvc.h>
}

#include <map>
#include <string>
#include <vector>


namespace ImGraphviz
{

#define IMGRAPHVIZ_CREATE_LABEL_ALLOCA(_name, _label)                                                             \
    char * _name;                                                                                                      \
    do                                                                                                                 \
    {                                                                                                                  \
        char const * const end = ImGui::FindRenderedTextEnd(_label);                                                   \
        size_t const size = end - _label;                                                                              \
        _name = (char *)alloca(size + 1);                                                                              \
        memcpy(_name, _label, size);                                                                                   \
        _name[size] = '\0';                                                                                            \
    }                                                                                                                  \
    while (0)

#define IMGRAPHVIZ_DRAW_NODE_PATH_COUNT 32
#define IMGRAPHVIZ_DRAW_EDGE_PATH_COUNT 64

struct ImGraphviz_Node
{
    std::string name;
    std::string label;
    ImVec2 pos;
    ImVec2 size;
    ImU32 color;
    ImU32 fillcolor;
};

struct ImGraphviz_Edge
{
    std::vector<ImVec2> points;
    std::string tail;
    std::string head;
    std::string label;
    ImVec2 labelPos;
    ImU32 color;
};

struct ImGraphviz_Graph
{
    std::vector<ImGraphviz_Node> nodes;
    std::vector<ImGraphviz_Edge> edges;
    ImVec2 size;
    float scale;
};

struct ImGraphviz_DrawNode
{
    ImVec2 path[IMGRAPHVIZ_DRAW_NODE_PATH_COUNT];
    ImVec2 textpos;
    char const * text;
    ImU32 color;
    ImU32 fillcolor;
};

struct ImGraphviz_DrawEdge
{
    ImVec2 path[IMGRAPHVIZ_DRAW_EDGE_PATH_COUNT];
    ImVec2 arrow1;
    ImVec2 arrow2;
    ImVec2 arrow3;
    ImVec2 textpos;
    char const * text;
    ImU32 color;
};

struct ImGraphvizContextCache
{
    ImGraphviz_Graph graph;
    ImGraphvizLayout layout = ImGraphvizLayout_Dot;
    float pixel_per_unit = 100.f;
    std::vector<ImGraphviz_DrawNode> drawnodes;
    std::vector<ImGraphviz_DrawEdge> drawedges;
    ImVec2 cursor_previous;
    ImVec2 cursor_current;
    std::string graphid_previous;
    std::string graphid_current;
};

extern ImGraphvizContext g_ctx;

template <size_t N>
class ImGraphviz_ShortString
{
public:
    friend ImGraphviz_ShortString<32> ImGuiIDToString(char const * id);
    friend ImGraphviz_ShortString<16> ImVec4ColorToString(ImVec4 const & color);

    operator char *()
    {
        return buf;
    }
    operator char const *() const
    {
        return buf;
    }

private:
    char buf[N];
};

IMGUI_API ImGraphviz_ShortString<32> ImGuiIDToString(char const * id);
IMGUI_API ImGraphviz_ShortString<16> ImVec4ColorToString(ImVec4 const & color);
IMGUI_API ImU32 ImGraphviz_StringToU32Color(char const * color);
IMGUI_API ImVec4 ImGraphviz_StringToImVec4Color(char const * color);
IMGUI_API char * ImGraphviz_ReadToken(char ** stringp);
IMGUI_API char * ImGraphviz_ReadLine(char ** stringp);
IMGUI_API bool ImGraphviz_ReadGraphFromMemory(ImGraphviz_Graph & graph, char const * data, size_t size);
IMGUI_API char const * ImGraphviz_GetEngineNameFromLayoutEnum(ImGraphvizLayout layout);
IMGUI_API ImVec2 ImGraphviz_BezierVec2(ImVec2 const * points, int count, float x);
IMGUI_API ImVec2 ImGraphviz_BSplineVec2(ImVec2 const * points, int count, float x);
IMGUI_API void ImGraphvizRenderGraphLayout(ImGraphviz_Graph & graph, ImGraphvizLayout layout);

}  // namespace ImGraphviz
