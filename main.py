from sys import stdin
import networkx as nx
import matplotlib.pyplot as plt

# Initialising graph
graph = dict()
line_ind = 1
for line in stdin:
    nums = line.split()
    for i in range(len(nums)):
        if nums[i] == '1':
            if line_ind in graph.keys():
                graph[line_ind].append(i + 1)
            else:
                graph[line_ind] = []
                graph[line_ind].append(i + 1)
    line_ind += 1
print(f'\nGRAPH:')
print(graph)

# Drawing the original graph
original_G = nx.Graph()

nodes = list(graph.keys())
from_list = []
for n_from in nodes:
    from_list.append(n_from)
    for n_to in graph[n_from]:
        if n_to not in from_list:
            original_G.add_edge(n_from, n_to)
pos = nx.spring_layout(original_G, seed=3113794652)
nx.draw_networkx_nodes(original_G, pos, node_color="tab:gray", node_size=600)

nx.draw_networkx_edges(original_G, pos, width=1.0, alpha=0.5)

labels = dict()
for node in nodes:
    labels[node] = str(node)
nx.draw_networkx_labels(original_G, pos, labels, font_size=14, font_color="whitesmoke")

ax = plt.gca()
ax.margins(0.20)
plt.axis("off")
plt.show()

# Recoloring
deg_5 = []  # список вершин со степенями не больше пяти
for key, val in graph.items():
    if len(val) <= 5:
        deg_5.append(key)


def graph_6_coloring(G, deg):
    if len(G) <= 6:  # если вершин не больше шести, то для покраски n вершин используем n цветов
        coloring_dict = {}
        color = 0
        for el in G.keys():
            coloring_dict[el] = color
            color += 1
    else:
        n = deg[0]
        deg.pop()
        adjacent = G[n]  # cохраняем вершины, соединённые с n
        for vertex in adjacent:
            vertex = vertex
            G[vertex].remove(n)
            if len(G[vertex]) == 5:  # обновление deg
                deg.append(vertex)
        del G[n]
        coloring_dict = graph_6_coloring(G, deg_5)
        G[n] = adjacent  # после покраски графа с меньшим количеством вершин возвращаем n
        deg.append(n)
        for vertex in adjacent:
            G[vertex].append(n)
        colors = [coloring_dict[k] for k in adjacent]  # составляем список цветов, в которые покрашены соседи n
        coloring_dict[n] = 0
        while coloring_dict[n] in colors:  # ищем неиспользованный цвет
            coloring_dict[n] += 1
    return coloring_dict


def int_col_to_str_col(col_dict: dict):
    all_colors = ["gray", "green", "blue", "orange", "yellow", "red"]
    string_dict = dict()
    all_nodes = list(col_dict.keys())
    for n in all_nodes:
        string_dict[n] = all_colors[col_dict[n]]
    return string_dict


coloring = graph_6_coloring(graph, deg_5)
print(f'\nCOLORING:')
print(coloring)
str_coloring = int_col_to_str_col(coloring)
print(str_coloring)

# Проверяем:
correct_coloring = True
for key, val in graph.items():
    for v in val:
        if coloring[key] == coloring[v]:
            correct_coloring = False
            break
print("Correct check:", correct_coloring)
