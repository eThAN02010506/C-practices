
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>



void sort(int items[][2], int length) 
{
    for (int i = 0; i < length - 1; i++) 
    {
        for (int j = 0; j < length - i - 1; j++) 
        {
            double ratio1 = (double)items[j][0] / items[j][1];
            double ratio2 = (double)items[j + 1][0] / items[j + 1][1];
            if (ratio1 < ratio2) 
            {
                int tempValue = items[j][0];
                int tempWeight = items[j][1];
                items[j][0] = items[j + 1][0];
                items[j][1] = items[j + 1][1];
                items[j + 1][0] = tempValue;
                items[j + 1][1] = tempWeight;
            }
        }
    }
}

void backPack_greed(int items[][2], int capacity)
//传入物品，传入背包含量
//二维数组：价值和重量
//首先实现贪心算法
//如果用递归： f(n) = f(n - 1), 背包容量 = 背包容量 - 物品？可以回溯算法？
{
    
    int weight = 0;
    sort(items, sizeof(items)/sizeof(items[0]));
    int i = 0;
    while (i < sizeof(items)/sizeof(items[0]))
    {
        if (weight + items[i][1] <= capacity)
        {
            weight += items[i][1];
        }
        else 
            i++;
    }
 /**    
    01书包：
    for (int i = 0; i < sizeof(items)/sizeof(items[0]); i++)
    {
        if (weight + items[i][1] > capacity)
            break;
        else
            weight += items[i][1];
    }
    无sort情况下：
    while (capacity > weight)
        {
        double most_value = 0;
        int item_weight = 0;
        for (int i = 0; i < sizeof(items) / sizeof(items[0]); i++)
            {
                if (items[i][0] != 0)
                {
                    if ((double)items[i][0] / items[i][1] > most_value)
                    {
                        most_value = items[i][0];
                        item_weight = items[i][1];
                    }
                }
            }
        for (int j = 0; j < sizeof(items) / sizeof(int); j++)
            {
                if (items[j][0] == most_value)
                {
                    items[j][0] = 0;
                    items[j][1] = 0;
                    break;
                }
            }
        weight += item_weight;
    }
}
**/
}
struct bag_s 
{
    int *items_count; //当前重量能放入的每种的物品数量
    int total_value; //当前重量所有放入的物品的总价值
};
typedef struct bag_s bag_t; 
// 遍历多少重量 从1重量开始加 循环至capacity 
void backpack_dynamic(int **items, int length, int capacity) 
{
    //初始化bags
    bag_t *bags = (bag_t*)malloc(sizeof(bag_t)*(capacity + 1));
    //初始化bags的items_count 和 total_value
    for (int i = 0; i < capacity; i++)
    {
        //初始化items_count, 为每个bag[i]位置申请一个int大小的空间
        bags[i].items_count = malloc(sizeof(int) * length);
        bags[i].total_value = 0;
    }
    //循环 重量从一开始每次加一 每次检查能放什么东西并且怎么放的重量/价值比例最高 记录在bag里 然后后面循环 在调用 看怎么搭配加哪个重量最适合
    for (int weight = 1; weight < capacity; weight++)
    {
        int max_index = -1;
        
        //初始化最高价值的物品
        int max_value = 0;
        //创建数组临时存储当前重量下每个物品的最佳数量
        int *most_value_item = malloc(length * sizeof(int));
        //初始化
        for (int j = 0; j < length; j++) 
        {
            //每个先暂时为0
            most_value_item[j] = 0; 
        }
        //遍历整个数组
        for (int k = 0; k < length; k++) 
        {
            //对比用
            int item_weight = items[k][0];
            int item_value = items[k][1];
            //只有当能放下时，开始循环对比
            if (item_weight <= weight) 
            {
                //如果能放下这个物品
                int remaining_weight = weight - item_weight;
                //那么新的价值就是减去该物品重量后的重量的位置的的价值加上该物品的价值
                int new_value = bags[remaining_weight].total_value + item_value;
                //如果这个数大于之前的最高价值的话
                if (new_value > max_value) 
                {
                    //更新最高数值
                    max_index = k;
                    max_value = new_value;
                    //并且循环，将bags每个物品数量暂时存在most_value_item里
                    /**
                    for (int j = 0; j < length; j++) 
                    {
                        //存入
                        most_value_item[j] = bags[remaining_weight].items_count[j];
                    }
                    //该位置的物品数量加一
                    most_value_item[k]++;
                    **/
                }
            }
        }
        if (max_index < 0)
        {
            for (int x = 0; x < length; x++)
            {
                bags[weight].items_count[x] = 0;
            }
            bags[weight].total_value = 0;
        }
        //遍历完成后将bag的weight位置的total_value变为遍历后的最重
        else
        {
            bags[weight].total_value = max_value;
            //循环 将暂存的再回到bags里
           
            for (int e = 0; e < length; e++) 
            {
                bags[weight].items_count[e] = bags[weight - items[max_index][1]].items_count[e];
            }
            bags[weight].items_count[max_index]++;
            //解放占据的空间
            free(most_value_item);
        }
    }


/**
    int bag[capacity + 1];
    malloc(bag, sizeof(bag));

    int weight = 0;
    int value = 0;

    while (weight < capacity) 
    {
        int temp_weight = 0;
        int temp_value = 0;

        for (int i = 0; i < n; i++) 
        {
            if (temp_weight + items[i][1] > capacity) 
            {
                break;
            }
            temp_weight += items[i][1];
            temp_value += items[i][0] / temp_weight;
        }

        if (temp_value > value) 
        {
            value = temp_value;
            weight = temp_weight;
        }

        for (int w = capacity; w >= 0; w--) 
        {
            if (w - temp_weight >= 0) 
            {
                if (bag[w] < bag[w - temp_weight] + temp_value) 
                {
                    bag[w] = bag[w - temp_weight] + temp_value;
                }
            }
        }
    }
**/
}




int isOk(int *board, int position)
{
    int n = position / 8;
    int i = n * 8;
    for (; i < n + 8; i++)
    {
        if (i != position)
        {
            if (board[i] == 1)
            {
                return 0;
            }
        }
    }
    i = position % 8;
    
    for (; i < 64; i += 8)
    {
        if (i != position)
        {
            if (board[i] == 1)
            {
                return 0;
            }
        }
    }
    n = position;
    int temp = n % 8;
    for (; n >= 0; n -= 9)
    {
        if (temp < n % 8)
            break;
        temp = n % 8;
        if (n != position)
        {
            if (board[n] == 1)
            {
                return 0;
            }
        }
    }

    n = position;
    temp = n % 8;
    for (; n < 64; n += 9)
    {
        if (temp > n % 8)
            break;
        temp = n % 8;
        if (n != position)
        {
            if (board[n] == 1)
            {
                return 0;
            }
        }
    }

    n = position;
    temp = n % 8;
    for (; n > 0; n -= 7)
    {
        if (temp > n % 8)
            break;
        temp = n % 8;
        if (n != position)
        {
            if (board[n] == 1)
            {
                return 0;
            }
        }
    }
    
    n = position;
    temp = n % 8;
    for (; n < 64; n += 7)
    {
        if (temp > n % 8)
            break;
        temp = n % 8;
        if (n != position)
        {
            if (board[n] == 1)
            {
                return 0;
            }
        }
    }
    //printf("ok %d %d %d\r\n", position, position / 8, position % 8);
    return 1;
}

/**
void queens()
{
    int start = 0;
    int board[64];
    for (int i = 0; i < 64; i++)
    {
        board[i] = 0;
    }
    int count = 0;
    for (int j = 0; j < 8; j++)
        {   
            board[j] = 1;
            count = 1;
            for (int a = 8; a < 16; a++)
            {
                if (isOk(board, a))
                {
                    board[a] = 1; 
                    count++;
                }   
                for (int b = 16; b < 24; b++)
                {
                    if (isOk(board, b))
                    {
                        board[b] = 1; 
                        count++;
                    }    
                    for (int c = 24; c < 32; c++)
                    {
                        if (isOk(board, c))
                        {
                            board[c] = 1; 
                            count++;
                        }    
                        for (int d = 32; d < 40; d++)
                        {
                            if(isOk(board, d))
                            {
                                board[d] = 1; 
                                count++; 
                            }
                            for (int e = 40; e < 48; e++)
                            {
                                if(isOk(board, e))
                                {
                                    board[e] = 1; 
                                    count++; 
                                }
                                for (int f = 48; f < 56; f++)
                                {
                                    if(isOk(board, f))
                                    {
                                        board[f] = 1; 
                                        count++; 
                                    }
                                    if (count < 8)
                                        break;
                                    else 
                                        {
                                            for (int z = 0; z < 64; z++)
                                                {
                                                    printf("%d", board[z]);
                                                    if (z == 7 || (z - 7) % 8 == 0)
                                                        {
                                                            printf("\n");
                                                        }

                                                }
                                        }
                                        
                                }
                                if (count < 7)
                                        break;
                            }
                            if (count < 6)
                                break;
                        }
                        if (count < 5)
                            break;
                    }
                    if (count < 4)
                        break;
                }
                if (count < 3)
                    break;
            }
            if (count < 2)
                 break;
        }


}

main ()
{
    queens();
}
**/
void printBoard(int *board, int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            int position = i * 8 + j;
            printf(" %d", board[position]);
            
        }
        printf("\r\n");
    }
    printf("\r\n");
}
//递归
int queens(int *board, int row, int size)
{
    if (row == size)
    {
        printBoard(board, size);
        return 1;
    }
    for (int col = 0; col < size; col++) 
    {
        int position = row * 8 + col;
        if (isOk(board, position)) 
        {
            board[position] = 1;
            queens(board, row + 1, size);  
            board[position] = 0;      
        }
    }
}

//非递归
void queen(int *board, int size)
{
    int row = 0;
    int col = 0;
    int pos[size];
    while (row >= 0) 
    {
        if (col < size) 
        {
            int position = row * size + col;
            if (isOk(board, position)) 
            {
                board[position] = 1;
                pos[row] = position;
                col = 0;
                row++;
                if (row == size) 
                {
                    printBoard(board, size);
                    row--;
                    board[pos[row]] = 0;
                    col = (pos[row] % size) + 1;
                }
            } 
            else 
            {
                col++;
            }
        } 
        else 
        {
            row--; 
            if (row >= 0) 
            {
                col = (pos[row] % size) + 1;
                board[pos[row]] = 0;
            }
        }
    }
}


void solveQueens()
{
    int board[64] = {0};
    for (int i = 0; i < 64; i++)
    {
        board[i] = 0;
    }
    queen(board, 8);


}

//用链表实现栈和队列
struct LinkNode 
{
    int val;               
    struct LinkNode *next; 
};

typedef struct LinkNode StackNode;
typedef struct LinkNode *Stack;
typedef struct LinkNode *List;
typedef struct LinkNode LinkNode;

void sortedlist_add(List *list, int n)
{
    LinkNode *ln;
    LinkNode *prev = NULL;
    LinkNode *node = malloc(sizeof(LinkNode));
    node->val = n;
    for (ln = *list; ln != NULL; ln = ln->next)
    {
        if (ln->val >= n)
        {
            break;
        }
        else 
            prev = ln; 
    }
    if(prev == NULL)
    {
        node->next = *list;
        *list = node;
    }
    else
    {
        prev->next = node;
        node->next = ln;
    }
}

int sortedlist_delete(List *list, int n)
{
    LinkNode *ln;
    LinkNode *prev = NULL;
    for (ln = *list; ln != NULL; ln = ln->next)
    {
        if (ln->val == n)
        {
            if (prev != NULL)
                prev->next = ln->next;
            else
                *list = ln->next;
            free(ln);
            return 1;
        }
        else if (ln->val > n)
            return 0;
        prev = ln; 
    }
    return 0;
}

#define LIST_FOREACH(list,node) for (node = list; node != NULL; node = node->next)
void sortedlist_print(List *list)
{
    List l;
    for (l = *list; l != NULL; l = l->next)
    {
        printf("%d\r\n", l->val);
    }
}
//栈ß
// 定义的时候* == 指针，使用的时候* == 指针的内容，所以当函数内部赋值指针指向的存储单元的内容会修改该内容
// 取变量的存储单元的地址 == &
void stack_push(Stack *stack, int val)
{
    StackNode *p = malloc(sizeof(StackNode));
    p->val = val;
    p->next = *stack;
    *stack = p;
}

//指针的指针
int stack_pop(Stack *stack, int *val)
{
    //指针的指针的指
    StackNode *p = *stack;
    if (p == NULL)
        return 0;
    *stack = p->next;
    *val = p->val;
    free(p);
    return 1;
}

void stack_free(Stack *stack)
{
    int value = 0;
    while(stack_pop(stack, &value));
}

//queue
struct Queue
{
    struct LinkNode *head;
    struct LinkNode *tail;
};
typedef struct Queue Queue;

void enqueue(Queue *q, int value)
{
    struct LinkNode *p = malloc(sizeof(struct LinkNode)); 
    p->val = value;
    p->next = NULL;
    if (q->tail != NULL)
    {
        q->tail->next = p;
        q->tail = p;
    }
    else
    {
        q->head = p;
        q->tail = p;
    }
}

int dequeue(Queue * q, int *value)
{
    struct LinkNode *p = q->head;
    if (p == NULL)
        return 0;
    q->head = p->next;
    *value = p->val;
    if (q->head == NULL)
    {
        q->tail = NULL;
    }
    free(p);
    return 1;
}
//double linknode
typedef struct DoubleLinkNode
{
    int value;
    struct DoubleLinkNode *next;
    struct DoubleLinkNode *prev;
} DoubleLinkNode;

typedef struct DoubleLink
{
    struct DoubleLinkNode *head;
    struct DoubleLinkNode *tail;
} DoubleLink;


void dlinkAdd(DoubleLink *dlink, DoubleLinkNode *nodeAfter, DoubleLinkNode *node)
{
    if (nodeAfter == NULL) 
    {
        node->next = dlink->head;
        node->prev = NULL;
        if (dlink->head != NULL) 
        {
            dlink->head->prev = node;
        }
        dlink->head = node;
        if (dlink->tail == NULL) 
        {
            dlink->tail = node;
        }
    } 
    else 
    {
        node->next = nodeAfter->next;
        node->prev = nodeAfter;
        if (nodeAfter->next != NULL) 
        {
            nodeAfter->next->prev = node;
        } 
        else 
        {
            dlink->tail = node;
        }
        nodeAfter->next = node;
    }
}



int dlinkDelete(DoubleLink *dlink, DoubleLinkNode *node)
{
    if (dlink->head == NULL)
    {
        return 0;
    }
    if (node->prev != NULL)
    {
        node->prev->next = node->next;
    }
    else 
        dlink->head = node->next;
    if (node->next != NULL)
    {
        node->next->prev = node->prev;
    }
    else 
        dlink->tail = node->prev;
    free(node);
    return 1;
}
//if in order 
void dlink_addsort(DoubleLink *dlink,  int n)
{
    DoubleLinkNode *dl = dlink->tail;
    DoubleLinkNode *dln = malloc(sizeof(DoubleLinkNode));
    dln->value = n;
    while (dl != NULL)
    {
        if (dl->value > n)
        {
            dl = dl->prev;
        } 
        else
        {
            break;
        }
    }
    dlinkAdd(dlink, dl, dln);
}

int dlink_delete(DoubleLink *dlink, int n)
{
    DoubleLinkNode *dl = dlink->head;

    while (dl != NULL)
    {
        if (dl->value != n)
        {
            dl = dl->next;
        } 
        else
        {
            dlinkDelete(dlink, dl);
            return 1;
        }
    }
    return 0;
}

//Basic calculation, +-*/
int priority(char n) {
    if (n == '+' || n == '-')
        return 1;
    else if (n == '*' || n == '/')
        return 2;
    else if (n == '(' || n == ')')
        return 0; 
    return -1;
}

int calculate(int a, char symbol, int b) {
    if (symbol == '+')
        return a + b;
    else if (symbol == '-')
        return a - b;
    else if (symbol == '*')
        return a * b;
    else if (symbol == '/')
        return a / b;
    else
        return 0;
}

int math(const char n[]) 
{
    int len = strlen(n);
    // 运算符号栈
    char op_stack[len];
    // 数字栈
    int num_stack[len];
    // 运算符号的栈顶
    int op_top = -1;
    // 数字的栈顶
    int num_top = -1;

    for (int i = 0; i < len; i++) 
    {
        // 如果是数字
        if (n[i] >= '0' && n[i] <= '9') 
        {
            int val = 0;
            // 处理连续数字
            while (i < len && n[i] >= '0' && n[i] <= '9') 
            {
                val = val * 10 + (n[i] - '0');
                i++;
            }
            i--;
            // 数字入栈
            num_stack[++num_top] = val;
        }
        // 如果是括号或运算符
        else 
        {
            // 左括号直接入栈
            if (n[i] == '(') 
            {
                op_stack[++op_top] = n[i];
            }
            // 右括号处理
            else if (n[i] == ')') 
            {
                // 直到遇到左括号
                while (op_top != -1 && op_stack[op_top] != '(') 
                {
                    int b = num_stack[num_top--];
                    int a = num_stack[num_top--];
                    char op = op_stack[op_top--];
                    num_stack[++num_top] = calculate(a, op, b);
                }
                // 移除左括号
                op_top--;
            }
            // 处理运算符
            else 
            {
                // 如果栈内有运算符且优先级大于等于当前运算符
                while (op_top != -1 && priority(op_stack[op_top]) >= priority(n[i])) 
                {
                    int b = num_stack[num_top--];
                    int a = num_stack[num_top--];
                    char op = op_stack[op_top--];
                    num_stack[++num_top] = calculate(a, op, b);
                }
                // 当前运算符入栈
                op_stack[++op_top] = n[i];
            }
        }
    }

    // 处理栈内剩余的运算符
    while (op_top != -1) 
    {
        int b = num_stack[num_top--];
        int a = num_stack[num_top--];
        char op = op_stack[op_top--];
        num_stack[++num_top] = calculate(a, op, b);
    }

    // 最终结果
    return num_stack[num_top];
}
const char* find2(const char *original, const char *search)
{
    const char *p, *q;
    for (p = original; *p != '\0'; p++ )
    {
        for(q = search; *q != '\0'; q++)
        {
            if (*(p + (q - search))  != *q) 
                break;
        }
        if (*q == '\0')
            return p;
    }
    return NULL;
}


int main(int argc, char *arrgv[])
{
    int value = 0;
    Stack stack = NULL;
    Queue queue = {0};
    printf("Hello World\r\n");
    //solveQueens();
    /*
    stack_push(&stack, 1);
    stack_push(&stack, 2);
    stack_push(&stack, 3);
    while(stack_pop(&stack, &value))
    {
        printf("%d %p \r\n", value, stack);
    }
    enqueue(&queue, 1);
    enqueue(&queue, 2);
    enqueue(&queue, 3);
    while (dequeue(&queue, &value))
    {
        printf("%d %p \r\n", value, queue.head);
    }
    
// test Dlink
    DoubleLink dlink = {0};
    dlink_addsort(&dlink, 2);
    dlink_addsort(&dlink, 1);
    dlink_addsort(&dlink, 3);
    DoubleLinkNode *dln;
    for (dln = dlink.head; dln != NULL; dln = dln->next)
    {
        printf("%d\r\n", dln->value);
    }
    printf("\r\n");
    dlink_delete(&dlink, 2);
    for (dln = dlink.head; dln != NULL; dln = dln->next)
    {
        printf("%d\r\n", dln->value);
    }
    printf("\r\n");
    dlink_delete(&dlink, 1);
    for (dln = dlink.head; dln != NULL; dln = dln->next)
    {
        printf("%d\r\n", dln->value);
    }
    printf("\r\n");
    dlink_delete(&dlink, 3);
    for (dln = dlink.head; dln != NULL; dln = dln->next)
    {
        printf("%d\r\n", dln->value);
    }
    return 0;
    List list = NULL;
    sortedlist_add(&list, 4);
    sortedlist_add(&list, 2);
    sortedlist_add(&list, 3);
    
    LinkNode *node;
    LIST_FOREACH(list, node)
    {
        printf("%d\r\n", node->val);
    }

    sortedlist_print(&list);

    sortedlist_delete(&list, 3);
    sortedlist_delete(&list, 2);
    sortedlist_delete(&list, 4);
*/

    char c[20] = "1+(2*3-1)-4+(12/2)";
    printf("%d\r\n", math(c));
    return 0;
}