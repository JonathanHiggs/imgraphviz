#pragma once

#include <imgui.h>

extern "C"
{
#include <graphviz/gvc.h>
}

#include <map>


namespace ImGraphviz
{

typedef int ImGraphvizLayout;

enum ImGraphvizLayout_
{
    ImGraphvizLayout_Circo,
    ImGraphvizLayout_Dot,
    ImGraphvizLayout_Fdp,
    ImGraphvizLayout_Neato,
    ImGraphvizLayout_Osage,
    ImGraphvizLayout_Sfdp,
    ImGraphvizLayout_Twopi
};

struct ImGraphvizContextCache;

struct ImGraphvizContext
{
    GVC_t * gvcontext = nullptr;
    graph_t * gvgraph = nullptr;
    ImGuiID lastid = 0;
    std::map<ImGuiID, ImGraphvizContextCache> graph_caches;
};

IMGUI_API ImGraphvizContext * CreateContext();
IMGUI_API void DestroyContext();

IMGUI_API bool BeginNodeGraph(
    char const * id, ImGraphvizLayout layout = ImGraphvizLayout_Dot, float pixel_per_unit = 100.f);
IMGUI_API void NodeGraphAddNode(char const * id);
IMGUI_API void NodeGraphAddNode(char const * id, ImVec4 const & color, ImVec4 const & fillcolor);
IMGUI_API void NodeGraphAddEdge(char const * id, char const * node_id_a, char const * node_id_b);
IMGUI_API void NodeGraphAddEdge(char const * id, char const * node_id_a, char const * node_id_b, ImVec4 const & color);
IMGUI_API void EndNodeGraph();

}  // namespace ImGraphviz
