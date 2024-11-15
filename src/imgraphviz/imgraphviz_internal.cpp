#include "imgraphviz/imgraphviz_internal.h"

#include "imgraphviz/imgraphviz.h"
#include "imgui_internal.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>


namespace ImGraphviz
{

char * strsep(char ** stringp, char const * delim)
{
    char * start = *stringp;
    char * p;

    p = (start != NULL) ? strpbrk(start, delim) : NULL;

    if (p == NULL)
    {
        *stringp = NULL;
    }
    else
    {
        *p = '\0';
        *stringp = p + 1;
    }

    return start;
}

ImGraphvizContext g_ctx;

ImGraphviz_ShortString<32> ImGuiIDToString(char const * id)
{
    ImGraphviz_ShortString<32> str;

    sprintf(str.buf, "%u", ImGui::GetID(id));
    return str;
}

ImGraphviz_ShortString<16> ImVec4ColorToString(ImVec4 const & color)
{
    ImGraphviz_ShortString<16> str;
    ImU32 const rgba = ImGui::ColorConvertFloat4ToU32(color);

    sprintf(str.buf, "#%x", rgba);
    return str;
}

ImU32 ImGraphviz_StringToU32Color(char const * color)
{
    ImU32 rgba;

    sscanf(color, "#%x", &rgba);
    return rgba;
}

ImVec4 ImGraphviz_StringToImVec4Color(char const * color)
{
    return ImGui::ColorConvertU32ToFloat4(ImGraphviz_StringToU32Color(color));
}

char * ImGraphviz_ReadToken(char ** stringp)
{
    if (*stringp && **stringp == '"')
    {
        *stringp += 1;
        char * token = strsep(stringp, "\"");
        strsep(stringp, " ");
        return token;
    }
    return strsep(stringp, " ");
}

char * ImGraphviz_ReadLine(char ** stringp)
{
    return strsep(stringp, "\n");
}

bool ImGraphviz_ReadGraphFromMemory(ImGraphviz_Graph & graph, char const * data, size_t size)
{
    char * copy = static_cast<char *>(alloca(sizeof(*copy) * size + 1));
    char * line = nullptr;

    memcpy(copy, data, size);
    copy[size] = '\0';
    while ((line = ImGraphviz_ReadLine(&copy)) != nullptr)
    {
        char * token = ImGraphviz_ReadToken(&line);

        if (strcmp(token, "graph") == 0)
        {
            graph.scale = atof(ImGraphviz_ReadToken(&line));
            graph.size.x = atof(ImGraphviz_ReadToken(&line));
            graph.size.y = atof(ImGraphviz_ReadToken(&line));
        }
        else if (strcmp(token, "node") == 0)
        {
            ImGraphviz_Node node;

            node.name = ImGraphviz_ReadToken(&line);
            node.pos.x = atof(ImGraphviz_ReadToken(&line));
            node.pos.y = atof(ImGraphviz_ReadToken(&line));
            node.size.x = atof(ImGraphviz_ReadToken(&line));
            node.size.y = atof(ImGraphviz_ReadToken(&line));
            node.label = ImGraphviz_ReadToken(&line);
            ImGraphviz_ReadToken(&line);  // style
            ImGraphviz_ReadToken(&line);  // shape
            node.color = ImGraphviz_StringToU32Color(ImGraphviz_ReadToken(&line));
            node.fillcolor = ImGraphviz_StringToU32Color(ImGraphviz_ReadToken(&line));
            graph.nodes.push_back(node);
        }
        else if (strcmp(token, "edge") == 0)
        {
            ImGraphviz_Edge edge;

            edge.tail = ImGraphviz_ReadToken(&line);
            edge.head = ImGraphviz_ReadToken(&line);
            int const n = atoi(ImGraphviz_ReadToken(&line));
            edge.points.resize(n);
            for (int i = 0; i < n; ++i)
            {
                edge.points[i].x = atof(ImGraphviz_ReadToken(&line));
                edge.points[i].y = atof(ImGraphviz_ReadToken(&line));
            }

            char const * s1 = ImGraphviz_ReadToken(&line);
            char const * s2 = ImGraphviz_ReadToken(&line);
            char const * s3 = ImGraphviz_ReadToken(&line);
            char const * s4 = ImGraphviz_ReadToken(&line);
            (void)s4;  // style
            char const * s5 = ImGraphviz_ReadToken(&line);

            if (s3)
            {
                edge.label = s1;
                edge.labelPos.x = atof(s2);
                edge.labelPos.y = atof(s3);
                edge.color = ImGraphviz_StringToU32Color(s5);
            }
            else
            {
                edge.color = ImGraphviz_StringToU32Color(s2);
            }
            graph.edges.push_back(edge);
        }
        else if (strcmp(token, "stop") == 0)
        {
            return true;
        }
        else
        {
            IM_ASSERT(false);
        }
    }
    return true;
}

char const * ImGraphviz_GetEngineNameFromLayoutEnum(ImGraphvizLayout layout)
{
    switch (layout)
    {
    case ImGraphvizLayout_Circo: return "circo";
    case ImGraphvizLayout_Dot:   return "dot";
    case ImGraphvizLayout_Fdp:   return "fdp";
    case ImGraphvizLayout_Neato: return "neato";
    case ImGraphvizLayout_Osage: return "osage";
    case ImGraphvizLayout_Sfdp:  return "sfdp";
    case ImGraphvizLayout_Twopi: return "twopi";
    default:                     IM_ASSERT(false); return "";
    }
}

float ImGraphviz_BSplineVec2ComputeK(ImVec2 const * p, float const * t, int i, int k, float x)
{
    auto const f = [](float const * t, int i, int k, float x) -> float {
        if (t[i + k] == t[i])
        {
            return 0.f;
        }
        return (x - t[i]) / (t[i + k] - t[i]);
    };
    auto g = ImGraphviz_BSplineVec2ComputeK;
    if (k == 0)
    {
        return (x < t[i] || x >= t[i + 1]) ? 0.f : 1.f;
    }
    return f(t, i, k, x) * g(p, t, i, k - 1, x) + (1.f - f(t, i + 1, k, x)) * g(p, t, i + 1, k - 1, x);
}

ImVec2 ImGraphviz_BSplineVec2(ImVec2 const * p, int m, int n, float x)
{
    if (n > (m - 2))
    {
        n = m - 2;
    }
    int const knotscount = m + n + 1;
    float * const knots = (float *)alloca(knotscount * sizeof(*knots));
    int i = 0;

    for (; i <= n; ++i)
    {
        knots[i] = 0.f;
    }
    for (; i < m; ++i)
    {
        knots[i] = i / (float)(m + n);
    }
    for (; i < knotscount; ++i)
    {
        knots[i] = 1.f;
    }

    ImVec2 result(0.f, 0.f);

    for (i = 0; i < m; ++i)
    {
        float const k = ImGraphviz_BSplineVec2ComputeK(p, knots, i, n, x);

        result.x += p[i].x * k;
        result.y += p[i].y * k;
    }
    return result;
}

ImVec2 ImGraphviz_BSplineVec2(ImVec2 const * points, int count, float x)
{
    return ImGraphviz_BSplineVec2(points, count, 3, ImClamp(x, 0.f, 0.9999f));
}

#define IMGRAPHVIZ_BINOMIALCOEFFICIENT_TABLE_CELL_1(_line, _cell) ImGraphviz_BinomialCoefficient(_line, _cell),

#define IMGRAPHVIZ_BINOMIALCOEFFICIENT_TABLE_CELL_2(_line, _cell)                                                      \
    IMGRAPHVIZ_BINOMIALCOEFFICIENT_TABLE_CELL_1(_line, _cell)                                                          \
    IMGRAPHVIZ_BINOMIALCOEFFICIENT_TABLE_CELL_1(_line, _cell + 1)

#define IMGRAPHVIZ_BINOMIALCOEFFICIENT_TABLE_CELL_4(_line, _cell)                                                      \
    IMGRAPHVIZ_BINOMIALCOEFFICIENT_TABLE_CELL_2(_line, _cell)                                                          \
    IMGRAPHVIZ_BINOMIALCOEFFICIENT_TABLE_CELL_2(_line, _cell + 2)

#define IMGRAPHVIZ_BINOMIALCOEFFICIENT_TABLE_CELL_8(_line, _cell)                                                      \
    IMGRAPHVIZ_BINOMIALCOEFFICIENT_TABLE_CELL_4(_line, _cell)                                                          \
    IMGRAPHVIZ_BINOMIALCOEFFICIENT_TABLE_CELL_4(_line, _cell + 4)

#define IMGRAPHVIZ_BINOMIALCOEFFICIENT_TABLE_CELL_16(_line, _cell)                                                     \
    IMGRAPHVIZ_BINOMIALCOEFFICIENT_TABLE_CELL_8(_line, _cell)                                                          \
    IMGRAPHVIZ_BINOMIALCOEFFICIENT_TABLE_CELL_8(_line, _cell + 8)

#define IMGRAPHVIZ_BINOMIALCOEFFICIENT_TABLE_LINE_1(_line) { IMGRAPHVIZ_BINOMIALCOEFFICIENT_TABLE_CELL_16(_line, 0) },

#define IMGRAPHVIZ_BINOMIALCOEFFICIENT_TABLE_LINE_2(_line)                                                             \
    IMGRAPHVIZ_BINOMIALCOEFFICIENT_TABLE_LINE_1(_line)                                                                 \
    IMGRAPHVIZ_BINOMIALCOEFFICIENT_TABLE_LINE_1(_line + 1)

#define IMGRAPHVIZ_BINOMIALCOEFFICIENT_TABLE_LINE_4(_line)                                                             \
    IMGRAPHVIZ_BINOMIALCOEFFICIENT_TABLE_LINE_2(_line)                                                                 \
    IMGRAPHVIZ_BINOMIALCOEFFICIENT_TABLE_LINE_2(_line + 2)

#define IMGRAPHVIZ_BINOMIALCOEFFICIENT_TABLE_LINE_8(_line)                                                             \
    IMGRAPHVIZ_BINOMIALCOEFFICIENT_TABLE_LINE_4(_line)                                                                 \
    IMGRAPHVIZ_BINOMIALCOEFFICIENT_TABLE_LINE_4(_line + 4)

#define IMGRAPHVIZ_BINOMIALCOEFFICIENT_TABLE_LINE_16(_line)                                                            \
    IMGRAPHVIZ_BINOMIALCOEFFICIENT_TABLE_LINE_8(_line)                                                                 \
    IMGRAPHVIZ_BINOMIALCOEFFICIENT_TABLE_LINE_8(_line + 8)

constexpr int ImGraphviz_BinomialCoefficient(int n, int k)
{
    return (n == 0 || k == 0 || n == k)
             ? 1
             : ImGraphviz_BinomialCoefficient(n - 1, k - 1) + ImGraphviz_BinomialCoefficient(n - 1, k);
}

constexpr int ImGraphviz_BinomialCoefficient_Table[16][16] = { IMGRAPHVIZ_BINOMIALCOEFFICIENT_TABLE_LINE_16(0) };

int ImGraphviz_BinomialCoefficientTable(int n, int k)
{
    if (n >= 0 && n < 16 && k >= 0 && k < 16)
    {
        return ImGraphviz_BinomialCoefficient_Table[n][k];
    }
    else
    {
        return ImGraphviz_BinomialCoefficient(n, k);
    }
}

ImVec2 ImGraphviz_BezierVec2(ImVec2 const * points, int count, float x)
{
    ImVec2 result(0.f, 0.f);

    for (int i = 0; i < count; ++i)
    {
        float const k = ImGraphviz_BinomialCoefficientTable(count - 1, i) * ImPow(1 - x, count - i - 1) * ImPow(x, i);

        result.x += points[i].x * k;
        result.y += points[i].y * k;
    }
    return result;
}

void ImGraphvizRenderGraphLayout(ImGraphviz_Graph & graph, ImGraphvizLayout layout)
{
    char * data = nullptr;
    unsigned int size = 0;
    char const * const engine = ImGraphviz_GetEngineNameFromLayoutEnum(layout);
    int ok = 0;

    graph = ImGraphviz_Graph();
    IM_ASSERT(g_ctx.gvcontext != nullptr);
    IM_ASSERT(g_ctx.gvgraph != nullptr);
    agattr(g_ctx.gvgraph, AGEDGE, (char *)"dir", "none");
    ok = gvLayout(g_ctx.gvcontext, g_ctx.gvgraph, engine);
    IM_ASSERT(ok == 0);
    ok = gvRenderData(g_ctx.gvcontext, g_ctx.gvgraph, "plain", &data, &size);
    IM_ASSERT(ok == 0);
    ImGraphviz_ReadGraphFromMemory(graph, data, size);
    gvFreeRenderData(data);
    gvFreeLayout(g_ctx.gvcontext, g_ctx.gvgraph);
}

}  // namespace ImGraphviz
