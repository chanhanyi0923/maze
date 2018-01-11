#include "graph.h"

const char Graph_road = 0;
const char Graph_obstacle = 1;
const char Graph_destination = 2;
const char Graph_outset = 3;

static struct Point Graph_move[4];

/* 建构一个新图 */
struct Graph*
Graph_new(int size)
{
    struct Graph *G = (struct Graph*)malloc(sizeof(struct Graph));
    G->size = size;
    G->path = Stack_new();
    G->solved = FALSE;
    return G;
}

/* 选取随机生成迷宫的算法 */
void
Graph_randomlyGenerateGraph(struct Graph *G,
                            mode_type     mode)
{
    switch(mode) {
        case MODE_KRUSKAL:
            Graph_randomlyGenerateGraphKruskal(G);
            break;
        case MODE_NAIVE:
            Graph_randomlyGenerateGraphNaive(G);
            break;
    }
    int i, j;

    /* 将最外围的点全部定为墙壁 */
    for (i = 0; i < G->size; i ++) {
        G->data[i][0] = Graph_obstacle;
        G->data[i][G->size - 1] = Graph_obstacle;
        G->data[0][i] = Graph_obstacle;
        G->data[G->size - 1][i] = Graph_obstacle;
    }

    /* 设置起点与终点 */
    G->outset = Point_makePoint(1, 1);
    G->destination = Point_makePoint(G->size - 2, G->size - 2);

    G->data[1][1] = Graph_outset;
    G->data[G->size - 2][G->size - 2] = Graph_destination;
}

/* Kruskal算法 */
void
Graph_randomlyGenerateGraphKruskal(struct Graph *G)
{
    struct DisjointSet *DSU = DisjointSet_new(G->size * G->size);

    /* 墙壁数量最多不会超过 (N ^ 2) / 2 */
    const int wall_list_max_size = (G->size * G->size) >> 1;
    int wall_list_size = 0;
    struct Point *wall_list = (struct Point*)
        malloc(wall_list_max_size * sizeof(struct Point));

    int i, j;
    /* 将所有可以拆的墙壁放入数组wall_list中 */
    for (i = 1; i < G->size - 1; i ++) {
        for (j = 1; j < G->size - 1; j ++) {
            /* 判断一开始是墙壁还是道路 */
            G->data[i][j] = ((i & 1) && (j & 1)) ? Graph_road : Graph_obstacle;
            
            /* 可拆墙壁（与小房间相邻）的条件为: (i + j)是奇数 */
            if ((i + j) & 1) {
                wall_list[wall_list_size ++] = Point_makePoint(i, j);
            }
        }
    }

    /* 在数组内任意交换两个元素，使得数组变成乱序 */
    for (i = 0; i < wall_list_size; i ++) {
        int idx[2];
        for (j = 0; j < 2; j ++) {
            /* 0 <= idx[j] <= wall_list_size - 1 */
            idx[j] = (int)((wall_list_size - 1) * (1.0 * rand() / RAND_MAX));
        }
        struct Point tmp = wall_list[idx[0]];
        wall_list[idx[0]] = wall_list[idx[1]];
        wall_list[idx[1]] = tmp;
    }

    /* 开始拆墙 */
    for (i = 0; i < wall_list_size; i ++) {
        struct Point pos = wall_list[i];

        int a, b;
        if (pos.x & 1) {
            /* 连接y方向的两个房间 */
            a = pos.x * G->size + (pos.y + 1);
            b = pos.x * G->size + (pos.y - 1);
        } else if (pos.y & 1) {
            /* 连接x方向的两个房间 */
            a = (pos.x + 1) * G->size + pos.y;
            b = (pos.x - 1) * G->size + pos.y;
        }

        /* 若不再同一个集合，則拆墙 */
        if (!DisjointSet_sameSet(DSU, a, b)) {
            G->data[pos.x][pos.y] = Graph_road;
            DisjointSet_merge(DSU, a, b);
        }
    }

    /* 释放内存 */
    free(wall_list);
    free(DSU);
}

/* 随机决定要生成墙壁或道路 */
char
Graph_makeElement()
{
    if (1.0 * rand() / RAND_MAX < OBSTACLE_WEIGHT) {
        return Graph_obstacle;
    } else {
        return Graph_road;
    }
}

/* 直观算法生成图 */
void
Graph_randomlyGenerateGraphNaive(struct Graph *G)
{
    int i, j;

    /* 将每一个元素随机给定为墙壁或道路 */
    for (i = 1; i < G->size - 1; i ++) {
        for (j = 1; j < G->size - 1; j ++) {
            G->data[i][j] = Graph_makeElement();
        }
    }
}

/* 更新在这一次DFS最深的点的标签 */
void
Graph_dlsUpdateTag(struct Graph *G,
                   struct Point  pos,
                   int           depth)
{
    int i;
    for (i = 0; i < 4; i ++) {
        /* 对于上一次走到的最远的点，更新他相邻的点为这次走到的最远的点 */
        struct Point next_pos = Point_add(pos, Graph_move[i]);
        if (G->data[next_pos.x][next_pos.y] != Graph_obstacle) {
            G->tag[next_pos.x][next_pos.y] =
                G->tag[next_pos.x][next_pos.y] < (depth + 1) ?
                G->tag[next_pos.x][next_pos.y] : depth + 1;
            /* 若该点已经走过，则不更新 */
        }
    }
}

gboolean
Graph_dls(struct Graph *G,
          struct Point  pos,
          int           depth)
{
    if (G->data[pos.x][pos.y] == Graph_obstacle) {
        return FALSE;
    }

    if (G->tag[pos.x][pos.y] == depth) {
        Graph_dlsUpdateTag(G, pos, depth);
        return (gboolean)(G->data[pos.x][pos.y] == Graph_destination);
    }

    if (G->visited[pos.x][pos.y]) {
        return FALSE;
    }
    G->visited[pos.x][pos.y] = TRUE;

    int i;
    for (i = 0; i < 4; i ++) {
        struct Point next_point = Point_add(pos, Graph_move[i]);
        if (Graph_dls(G, next_point, depth)) {
            return TRUE;
        }
    }

    return FALSE;
}

void Graph_initVisited(struct Graph *G)
{
    int i, j;
    for (i = 0; i < G->size; i ++) {
        for (j = 0; j < G->size; j ++) {
            G->visited[i][j] = FALSE;
        }
    }
}

gboolean
Graph_iddfs(struct Graph *G,
            struct Point  pos)
{
    int i, j;

    for (i = 0; i < G->size; i ++) {
        for (j = 0; j < G->size; j ++) {
            G->tag[i][j] = G->size * G->size + 1;
        }
    }
    G->tag[pos.x][pos.y] = 0;

    for (i = 0; i < G->size * G->size; i ++) {
        Graph_initVisited(G);
        if (Graph_dls(G, pos, i)) {
            return TRUE;
        }
    }
    return FALSE;
}

gboolean
Graph_solved(struct Graph *G)
{
    return G->solved;
}

gboolean
Graph_findDestination(struct Graph *G)
{
    /* 设定移动方向 */
    Graph_move[0] = Point_makePoint(0, 1);
    Graph_move[1] = Point_makePoint(1, 0);
    Graph_move[2] = Point_makePoint(0, -1);
    Graph_move[3] = Point_makePoint(-1, 0);

    /* 用IDDFS找路 */
    G->solved = Graph_iddfs(G, G->outset);

    if (G->solved) {
        /* 从终点寻找路径走回原路 */
        struct Point pos = G->destination;
        while (G->tag[pos.x][pos.y] > 0) {
            int i;
            for (i = 0; i < 4; i ++) {
                struct Point next_pos = Point_add(pos, Graph_move[i]);
                if (G->tag[pos.x][pos.y] - G->tag[next_pos.x][next_pos.y] == 1) {
                    Stack_push(G->path, next_pos);
                    pos = next_pos;
                    break;
                }
            }
        }

        /* 将第一个点去除（终点） */
        Stack_pop(G->path);
    }

    return G->solved;
}
