from sys import stdin
import networkx as nx
import matplotlib.pyplot as plt


def graph_coloring(gr, deg):
    if len(gr) <= 6:  # если вершин не больше шести, то для покраски n вершин используем n цветов
        coloring_dict = {}
        color = 0
        for el in gr.keys():
            coloring_dict[el] = color
            color += 1
    else:
        n = deg[0]
        deg.pop()
        adjacent = gr[n]  # cохраняем вершины, соединённые с n
        for vertex in adjacent:
            vertex = vertex
            gr[vertex].remove(n)
            if len(gr[vertex]) == 5:  # обновление deg
                deg.append(vertex)
        del gr[n]
        coloring_dict = graph_coloring(gr, deg_5)
        gr[n] = adjacent  # после покраски графа с меньшим количеством вершин возвращаем n
        deg.append(n)
        for vertex in adjacent:
            gr[vertex].append(n)
        colors = [coloring_dict[k] for k in adjacent]  # составляем список цветов, в которые покрашены соседи n
        coloring_dict[n] = 0
        while coloring_dict[n] in colors:  # ищем неиспользованный цвет
            coloring_dict[n] += 1
    return coloring_dict


def int_col_to_str_col(col_dict: dict):
    all_colors = ["gray", "green", "blue", "orange", "purple", "red"]
    string_dict = dict()
    all_nodes = list(col_dict.keys())
    for n in all_nodes:
        string_dict[n] = all_colors[col_dict[n]]
    return string_dict


def color_one_node(gr, n, col_dict: dict, p):
    col = col_dict[n]
    nx.draw_networkx_nodes(gr, p, nodelist=[n], node_color=col, node_size=600)


def initializing_graph():
    # Initialising graph
    gr = dict()
    line_ind = 1
    for line in stdin:
        nums = line.split()
        for i in range(len(nums)):
            if nums[i] == '1':
                if line_ind in gr.keys():
                    gr[line_ind].append(i + 1)
                else:
                    gr[line_ind] = []
                    gr[line_ind].append(i + 1)
        line_ind += 1
    print(f'\nGRAPH:')
    print(gr)
    return gr


def draw_original(gr, p):
    original_gr = nx.Graph()

    nodes = list(gr.keys())
    from_list = []
    for n_from in nodes:
        from_list.append(n_from)
        for n_to in gr[n_from]:
            if n_to not in from_list:
                original_gr.add_edge(n_from, n_to)

    nx.draw_networkx_nodes(original_gr, p, node_color="tab:gray", node_size=600)

    nx.draw_networkx_edges(original_gr, p, width=1.0, alpha=0.5)

    labels = dict()
    for node in nodes:
        labels[node] = str(node)
    nx.draw_networkx_labels(original_gr, p, labels, font_size=14, font_color="whitesmoke")

    ax = plt.gca()
    ax.margins(0.20)
    plt.axis("off")
    plt.show()


def draw_colored(gr, p, col_dict):
    new_gr = nx.Graph()

    nodes = list(gr.keys())
    from_list = []
    for n_from in nodes:
        from_list.append(n_from)
        for n_to in gr[n_from]:
            if n_to not in from_list:
                new_gr.add_edge(n_from, n_to)

    for node in nodes:
        color_one_node(new_gr, node, col_dict, p)

    nx.draw_networkx_edges(new_gr, p, width=1.0, alpha=0.5)

    labels = dict()
    for node in nodes:
        labels[node] = str(node)
    nx.draw_networkx_labels(new_gr, p, labels, font_size=14, font_color="whitesmoke")

    ax = plt.gca()
    ax.margins(0.20)
    plt.axis("off")
    plt.show()


# START. Инициализация графа матрицей смежности.
graph = initializing_graph()
pos = {1: (-4, 0), 2: (3, 10), 3: (10, 0), 4: (3, 1.3), 5: (1, 5), 6: (5, 5), 7: (3, 3.5)}
draw_original(graph, pos)

# Recoloring
deg_5 = []  # список вершин со степенями не больше пяти
for key, val in graph.items():
    if len(val) <= 5:
        deg_5.append(key)
coloring = graph_coloring(graph, deg_5)
str_coloring = int_col_to_str_col(coloring)
print(f'COLORING: {str_coloring}')
draw_colored(graph, pos, str_coloring)
