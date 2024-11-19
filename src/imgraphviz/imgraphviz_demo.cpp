#include "imgraphviz/imgraphviz_demo.hpp"

#include "imgraphviz/imgraphviz.hpp"
#include "imgraphviz/imgraphviz_internal.hpp"


namespace ImGraphviz
{
namespace
{

    template <class T>
    struct RBNode
    {
        T value;
        RBNode * parent;
        RBNode * left;
        RBNode * right;
        bool red;
    };

    template <class T, class Compare = std::less<T>>
    struct RBTree
    {
        RBTree() : root(nullptr), compare()
        { }

        ~RBTree()
        {
            clear();
        }

        void ll(RBNode<T> * x)
        {
            RBNode<T> * p = x->parent;
            RBNode<T> * g = p->parent;
            RBNode<T> * gg = g->parent;
            RBNode<T> * u = g->right;
            RBNode<T> * pright = p->right;
            bool pcolor = p->red;

            IM_ASSERT(p != u);
            IM_ASSERT(pright != x);
            p->right = g;
            g->parent = p;
            g->left = pright;
            if (pright)
            {
                pright->parent = g;
            }
            p->parent = gg;
            p->red = g->red;
            g->red = pcolor;
            if (gg)
            {
                if (gg->left == g)
                {
                    gg->left = p;
                }
                else if (gg->right == g)
                {
                    gg->right = p;
                }
                else
                {
                    IM_ASSERT(false);
                }
            }
            else
            {
                root = p;
            }
            if (!p->parent)
            {
                root = p;
            }
        }

        void lr(RBNode<T> * x)
        {
            RBNode<T> * p = x->parent;
            RBNode<T> * g = p->parent;
            RBNode<T> * xleft = x->left;

            p->right = xleft;
            if (xleft)
            {
                xleft->parent = p;
            }
            x->left = p;
            p->parent = x;
            x->parent = g;
            g->left = x;
            ll(p);
        }

        void rr(RBNode<T> * x)
        {
            RBNode<T> * p = x->parent;
            RBNode<T> * g = p->parent;
            RBNode<T> * gg = g->parent;
            RBNode<T> * u = g->left;
            RBNode<T> * pleft = p->left;
            bool pcolor = p->red;

            IM_ASSERT(p != u);
            IM_ASSERT(pleft != x);
            p->left = g;
            g->parent = p;
            g->right = pleft;
            if (pleft)
            {
                pleft->parent = g;
            }
            p->parent = gg;
            p->red = g->red;
            g->red = pcolor;
            if (gg)
            {
                if (gg->left == g)
                {
                    gg->left = p;
                }
                else if (gg->right == g)
                {
                    gg->right = p;
                }
                else
                {
                    IM_ASSERT(false);
                }
            }
            else
            {
                root = p;
            }
            if (!p->parent)
            {
                root = p;
            }
        }

        void rl(RBNode<T> * x)
        {
            RBNode<T> * p = x->parent;
            RBNode<T> * g = p->parent;
            RBNode<T> * xright = x->right;

            p->left = xright;
            if (xright)
            {
                xright->parent = p;
            }
            x->right = p;
            p->parent = x;
            x->parent = g;
            g->right = x;
            rr(p);
        }

        void fix(RBNode<T> * x)
        {
            RBNode<T> * p = x->parent;
            RBNode<T> * g = p ? p->parent : nullptr;
            RBNode<T> * u = g ? (p == g->left ? g->right : g->left) : nullptr;

            if (x == root)
            {
                x->red = false;
            }
            else if (p && p->red)
            {
                if (u && u->red)
                {
                    p->red = false;
                    u->red = false;
                    g->red = true;
                    fix(g);
                }
                else
                {
                    if (g == nullptr)
                    {
                        IM_ASSERT(false);
                    }
                    else if (g->left == p && p->left == x)
                    {
                        ll(x);
                    }
                    else if (g->left == p && p->right == x)
                    {
                        lr(x);
                    }
                    else if (g->right == p && p->right == x)
                    {
                        rr(x);
                    }
                    else if (g->right == p && p->left == x)
                    {
                        rl(x);
                    }
                    else
                    {
                        IM_ASSERT(false);
                    }
                    if (x->parent)
                    {
                        fix(x->parent);
                    }
                }
            }
        }

        void insert(RBNode<T> * newnode, RBNode<T> * parent, RBNode<T> * leaf, bool parentcmp)
        {
            if (leaf)
            {
                parentcmp = compare(newnode->value, leaf->value);
                if (parentcmp)
                {
                    insert(newnode, leaf, leaf->left, parentcmp);
                }
                else
                {
                    insert(newnode, leaf, leaf->right, parentcmp);
                }
            }
            else
            {
                newnode->parent = parent;
                if (parentcmp)
                {
                    parent->left = newnode;
                }
                else
                {
                    parent->right = newnode;
                }
            }
        }

        void insert(T const & value)
        {
            RBNode<T> * newnode = new RBNode<T>();

            newnode->value = value;
            newnode->parent = nullptr;
            newnode->left = nullptr;
            newnode->right = nullptr;
            newnode->red = true;
            if (root)
            {
                insert(newnode, nullptr, root, false);
            }
            else
            {
                root = newnode;
            }
            fix(newnode);
        }

        void clear(RBNode<T> * node)
        {
            if (node)
            {
                clear(node->left);
                clear(node->right);
                delete node;
            }
        }

        void clear()
        {
            clear(root);
            root = nullptr;
        }

        RBNode<T> * find(RBNode<T> * node, T const & value)
        {
            if (!node)
            {
                return nullptr;
            }

            if (compare(value, node->value))
            {
                return find(node->left, value);
            }
            else
            {
                if (compare(node->value, value))
                {
                    return find(node->right, value);
                }
                else
                {
                    return node;
                }
            }
        }

        RBNode<T> * find(T const & value)
        {
            return find(root, value);
        }

        RBNode<T> * root;
        Compare compare;
    };

    void draw_rbnode(RBNode<std::string> * node, RBNode<std::string> * found_node)
    {
        char * nodea = nullptr;
        char * nodeb = nullptr;
        char edge[64];
        int len = 0;

        len = snprintf(nullptr, 0, "%s##%p", node->value.c_str(), (void *)node);
        nodea = (char *)alloca(len + 1);
        sprintf(nodea, "%s##%p", node->value.c_str(), (void *)node);
        NodeGraphAddNode(
            nodea,
            node == found_node ? ImVec4(0.f, 1.f, 0.f, 1.f) : ImVec4(1.f, 1.f, 1.f, 1.f),
            node->red ? ImVec4(1.f, 0.f, 0.f, 0.7f) : ImVec4(0.f, 0.f, 0.f, 0.7f));
        for (auto * child : { node->left, node->right })
        {
            if (child)
            {
                draw_rbnode(child, found_node);
                len = snprintf(nullptr, 0, "%s##%p", child->value.c_str(), (void *)child);
                nodeb = (char *)alloca(len + 1);
                sprintf(nodeb, "%s##%p", child->value.c_str(), (void *)child);
                sprintf(edge, "##%p->%p", (void *)node, (void *)child);
                NodeGraphAddEdge(edge, nodea, nodeb);
            }
        }
    }

    void draw_rbtree(ImGraphvizLayout layout, float ppu)
    {
        static RBTree<std::string> tree;
        static char bufadd[64] = { 0 };
        static char bufsearch[64] = { 0 };
        static RBNode<std::string> * found_node = nullptr;
        static bool lazy_init = true;

        if (ImGui::Button("Clear"))
        {
            found_node = nullptr;
            tree.clear();
        }
        ImGui::SameLine();

        if (ImGui::Button("Lorem Ipsum") || lazy_init)
        {
            lazy_init = false;
            tree.clear();
            tree.insert("lorem");
            tree.insert("ipsum");
            tree.insert("dolor");
            tree.insert("sit");
            tree.insert("amet");
            tree.insert("consectetur");
            tree.insert("adipiscing");
            tree.insert("elit");
            tree.insert("sed");
            tree.insert("do");
            tree.insert("eiusmod");
            tree.insert("tempor");
            tree.insert("incididunt");
            tree.insert("ut");
            tree.insert("labore");
            tree.insert("et");
            tree.insert("dolore");
            tree.insert("magna");
            tree.insert("aliqua");
        }

        if (ImGui::InputTextWithHint(
                "Add",
                "Type text and press enter to add a new node",
                bufadd,
                sizeof(bufadd) - 1,
                ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
        {
            tree.insert(bufadd);
            *bufadd = '\0';
        }

        if (ImGui::InputTextWithHint(
                "Search",
                "Type text to search node",
                bufsearch,
                sizeof(bufsearch) - 1,
                ImGuiInputTextFlags_AutoSelectAll))
        {
            found_node = tree.find(bufsearch);
        }

        if (*bufsearch && !found_node)
        {
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "node '%s' not found", bufsearch);
        }

        if (BeginNodeGraph("example3", layout, ppu))
        {
            if (tree.root)
            {
                draw_rbnode(tree.root, found_node);
            }
            EndNodeGraph();
        }
    }

    void draw_example1(ImGraphvizLayout layout, float ppu)
    {
        if (BeginNodeGraph("example1", layout, ppu))
        {
            NodeGraphAddNode("A");
            NodeGraphAddNode("B");
            NodeGraphAddNode("C");
            NodeGraphAddNode("D");
            NodeGraphAddEdge("a->b", "A", "B");
            NodeGraphAddEdge("b->c", "B", "C");
            NodeGraphAddEdge("c->d", "C", "D");
            NodeGraphAddEdge("d->a", "D", "A");
            EndNodeGraph();
        }
    }

    void draw_example2(ImGraphvizLayout layout, float ppu)
    {
        if (BeginNodeGraph("example2", layout, ppu))
        {
            NodeGraphAddNode("LR_0");
            NodeGraphAddNode("LR_1");
            NodeGraphAddNode("LR_2");
            NodeGraphAddNode("LR_3");
            NodeGraphAddNode("LR_4");
            NodeGraphAddNode("LR_5");
            NodeGraphAddNode("LR_6");
            NodeGraphAddNode("LR_7");
            NodeGraphAddNode("LR_8");
            NodeGraphAddEdge("SS(B)", "LR_0", "LR_2");
            NodeGraphAddEdge("SS(S)", "LR_0", "LR_1");
            NodeGraphAddEdge("S($end)", "LR_1", "LR_3");
            NodeGraphAddEdge("SS(b)", "LR_2", "LR_6");
            NodeGraphAddEdge("SS(a)", "LR_2", "LR_5");
            NodeGraphAddEdge("S(A)", "LR_2", "LR_4");
            NodeGraphAddEdge("S(b)", "LR_5", "LR_7");
            NodeGraphAddEdge("S(a)", "LR_5", "LR_5");
            NodeGraphAddEdge("S(b)", "LR_6", "LR_6");
            NodeGraphAddEdge("S(a)", "LR_6", "LR_5");
            NodeGraphAddEdge("S(b)", "LR_7", "LR_8");
            NodeGraphAddEdge("S(a)", "LR_7", "LR_5");
            NodeGraphAddEdge("S(b)", "LR_8", "LR_6");
            NodeGraphAddEdge("S(a)", "LR_8", "LR_5");
            EndNodeGraph();
        }
    }

}  // namespace


void ShowGraphNodeDemoWindow(bool * p_open)
{
    static ImGraphvizLayout layout = ImGraphvizLayout_Dot;
    static bool autoResize = true;
    static float ppu = 100.0f;
    ImGraphvizLayout flags = autoResize ? ImGuiWindowFlags_AlwaysAutoResize : ImGuiWindowFlags_None;

    if (ImGui::Begin("ImGraphviz demo window", p_open, flags))
    {
        auto const items_getter = [](void *, int idx, char const ** out_text) {
            *out_text = ImGraphviz_GetEngineNameFromLayoutEnum((ImGraphvizLayout)idx);
            return true;
        };

        ImGui::Checkbox("Auto Resize Window", &autoResize);
        ImGui::Combo("Layout", (int *)&layout, items_getter, nullptr, 7);
        ImGui::SliderFloat("Pixel-per-unit", &ppu, 30.f, 200.f);

        if (ImGui::BeginTabBar("tabbar", ImGuiTabBarFlags_None))
        {
            if (ImGui::BeginTabItem("Example 1"))
            {
                draw_example1(layout, ppu);
                ImGui::EndTabItem();
            }
            if (ImGui::IsItemClicked())
            {
                layout = ImGraphvizLayout_Circo;
            }

            if (ImGui::BeginTabItem("Example 2"))
            {
                draw_example2(layout, ppu);
                ImGui::EndTabItem();
            }
            if (ImGui::IsItemClicked())
            {
                layout = ImGraphvizLayout_Dot;
            }

            if (ImGui::BeginTabItem("Red-black tree"))
            {
                draw_rbtree(layout, ppu);
                ImGui::EndTabItem();
            }
            if (ImGui::IsItemClicked())
            {
                layout = ImGraphvizLayout_Dot;
            }

            ImGui::EndTabBar();
        }

        ImGui::End();
    }
}

}  // namespace ImGraphviz
