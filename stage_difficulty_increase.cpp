#include "stage_difficulty_increase.h"
#include "game_logic.h"

// Linked list functions
// Initialize a node
Node *createNode(Block block)
{
    Node *p = new Node;
    p->block = block;
    p->next = NULL;

    return p;
}

// Initialize a list
List createList(Node *p)
{
    List l;
    l.head = l.tail = p;
    return l;
}

// Add tail to the list
void addTail(List &l, Block block)
{
    Node *p = createNode(block);

    if (l.head == NULL) l.head = l.tail = p;
    
    else
    {
        l.tail->next = p;
        l.tail = p;
    }
}

// Remove the head node from the list
void removeHead(List &l)
{
    if (l.head == NULL) return;

    if (l.head == l.tail)
    {
        Node *p = l.head;
        l.head = l.tail = NULL;
        delete p;
    }
    else
    {
        Node *p = l.head;
        l.head = l.head->next;
        delete p;
    }
}

// Remove the tail node from the list
void removeTail(List &l)
{
    if (l.head == NULL) return;

    if (l.head == l.tail)
    {
        Node *p = l.head;
        l.head = l.tail = NULL;
        delete p;
    }
    else
    {
        Node *p = l.head;
        while (p->next->next) p = p->next;

        delete l.tail;
        l.tail = p;
        l.tail->next = NULL;
    }
}

// Initialize board
void initBoard(List &board, int size)
{    
    int pool_size = size * size;
    int *pool = new int[pool_size]; // Create a pool of values to fill Board
    
    genRandoms(pool, pool_size);    // Get randoms values
    shuffleRandoms(pool, pool_size);    // Shuffle the pool

    for (int i = 0; i < pool_size; i++)
    {
        Block block;
        char value = pool[i] + 'A';
        initBlock(block, i % size, i / size, value); // Initialize each block in the board.

        addTail(board, block);
    }

    delete[] pool;
}

// Find wanted Node
Node *gotoNode(Node *head, int x, int y)
{
    if (head == NULL) return NULL;

    Node *p = head;
    while (p)
    {
        if (p->block.x == x && p->block.y == y) 
            return p;
        p = p->next;
    }

    return NULL;
}

// Pattern checking
bool check_I_Node(Node *head, int x1, int y1, int x2, int y2)
{
    bool check_vertical = true;
    bool check_horizontal = true;

    // Check vertically
    if (x1 != x2)
        check_vertical = false;
    else
    {
        // Check if there's any other block in the way.
        int start = std::min(y1, y2); 
        int end = std::max(y1, y2);

        // If there's a block that is not empty between first and second block, 
        // set check_vertical to false
        for (int row = start + 1; row < end; row++)
        {
            if (gotoNode(head, x1, row) != NULL)
            {
                check_vertical = false;
                break;
            }
        }
    }

    // Check horizontally
    if (y1 != y2)
        check_horizontal = false;
    else
    {
        // Check if there's any other block in the way.
        int start = std::min(x1, x2); 
        int end = std::max(x1, x2);

        // If there's a block that is not empty between first and second block, 
        // set check_horizontal to false
        for (int col = start + 1; col < end; col++)
        {
            if (gotoNode(head, col, y1) != NULL)
            {
                check_horizontal = false;
                break;
            }
        }
    }

    return check_vertical || check_horizontal;
}

bool check_L_Node(Node *head, int x1, int y1, int x2, int y2) 
{
    bool flag1 = false;
    bool flag2 = false;

    // Find the position of the other two corners of the L shape 
    Node *temp1 = gotoNode(head, x1, y2);
    Node *temp2 = gotoNode(head, x2, y1);
    
    if (temp1 == NULL)
        // Check L match by verifying both of its edges
        flag1 = check_I_Node(head, x1, y1, x1, y2) && check_I_Node(head, x2, y2, x1, y2);
    
    if (temp2 == NULL)
        // Likewise, check L match by verifying the edges.
        flag2 = check_I_Node(head, x1, y1, x2, y1) && check_I_Node(head, x2, y2, x2, y1);
    
    return (flag1 || flag2);
}

bool check_Z_Node(Node *head, int x1, int x2, int y1, int y2) {

    bool check_vertical = false;
    bool check_horizontal = false;

    // Check vertically
  
    int start = std::min(y1, y2);
    int end = std::max(y1, y2);

    for (int row = start + 1; row < end; row++) {

        Node* temp1 = gotoNode(head, x1, row);
        Node* temp2 = gotoNode(head, x2, row);

        if (temp1 == NULL && temp2 == NULL) 
        {
            // Check Z match by verifying three of its edges
            if (check_I_Node(head, x1, y1, x1, row) &&
                check_I_Node(head, x1, row, x2, row) &&
                check_I_Node(head, x2, y2, x2, row))
            {
                check_vertical = true;
                break;
            }
        }
    }


    // Check horizontally

    start = std::min(x1, x2);
    end = std::max(x1, x2);

    for (int col = start + 1; col < end; col++)
    {
        Node* temp1 = gotoNode(head, col, y1);
        Node* temp2 = gotoNode(head, col, y2);

        if (temp1 == NULL && temp2 == NULL)
        {
            // Check Z match by verifying three of its edges
            if (check_I_Node(head, x1, y1, col, y1) &&
                check_I_Node(head, col, y1, col, y2) &&
                check_I_Node(head, x2, y2, col, y2))
            {
                check_horizontal = true;
                break;
            }
        }
    }
    return check_vertical || check_horizontal;
}

bool check_U_Node(Node *head, int x1, int y1, int x2, int y2, int width, int height)
{   
    // First check the case where two blocks are the edge of the board.
    if ((y1 == y2) && (y1 == 0 || y1 == height - 1))
        return true;

    if ((x1 == x2) && (x1 == 0 || x1 == width - 1))
        return true;

    Node *tmp1, *tmp2;
    // Iterate from first row to last row.
    for (int i = 0; i < height; i++)
    {
        tmp1 = gotoNode(head, x1, i);
        tmp2 = gotoNode(head, x2, i);

        // For the first and last row, check for clear path between first block and tmp1 block
        // and check for clear path between second block and tmp2 block.
        if ((i == 0 || i == height - 1) && check_I_Node(head, x1, y1, x1, i) && check_I_Node(head, x2, y2, x2, i))
        {
            // If the first block or the second block is on the first or last row
            if ((i == y1 || i == y2))
            {
                // If the tmp1 block and the first block are on the same row
                // and if the tmp1 block is empty, return true
                if (i != y1 && tmp1 == NULL)
                    return true;
                // If the tmp2 block and the second block are on the same row
                // and if the tmp2 block is empty, return true
                if (i != y2 && tmp2 == NULL)
                    return true;
            }
            else
            {
                // If the first or the second block not on first or last row
                // check to see if both tmp blocks are empty
                if (tmp1 == NULL && tmp2 == NULL)
                    return true;
            }

        }
        else
        {
            // For other rows, check for clear path between tmp1 and tmp2 block,
            // as well as clear path between first and tmp1 and between second and tmp2.
            if (check_I_Node(head, x1, y1, x1, i) && check_I_Node(head, x2, y2, x2, i)
                && check_I_Node(head, x1, i, x2, i)
                && (tmp1 == NULL && tmp2 == NULL))
                    return true;
        }
    }

    // Iterate from first column to last column.
    for (int i = 0; i < width; i++)
    {
        tmp1 = gotoNode(head, i, y1);
        tmp2 = gotoNode(head, i, y2);

        // For the first and last column, check for clear path between first block and tmp1 block
        // and check for clear path between second block and tmp2 block.
        if ((i == 0 || i == width - 1) && check_I_Node(head, x1, y1, i, y1) && check_I_Node(head, x2, y2, i, y2))
        {
            // If the first block or the second block is on the first or last column
            if ((i == x1 || i == x2))
            {
                // If the tmp1 block and the first block are on the same column
                // and if the tmp1 block is empty, return true
                if (i != x1 && tmp1 == NULL)
                    return true;
                // If the tmp2 block and the second block are on the same column
                // and if the tmp2 block is empty, return true
                if (i != x2 && tmp2 == NULL)
                    return true;
            }
            else
            {
                // If the first or the second block not on first or last column
                // check to see if both tmp blocks are empty
                if (tmp1 == NULL && tmp2 == NULL)
                    return true;
            }

        }
        else
        {
            // For other rows, check for clear path between tmp1 and tmp2 block,
            // as well as clear path between first and tmp1 and between second and tmp2.
            if (check_I_Node(head, x1, x2, i, y1) && check_I_Node(head, x2, y2, i, y2)
                && check_I_Node(head, i, y1, i, y2)
                && (tmp1 == NULL && tmp2 == NULL))
                    return true;
        }
    }
    return false;
}

// Remove a block from the board
void removeBlock(List &board, int x, int y)
{
    if(board.head == NULL) return;

    Node *p = gotoNode(board.head, x, y); // Move to the coordinate of (x,y)

    if (p == NULL) return; // If not found, return

    if (p == board.head) removeHead(board); // If it's head, remove the head
    else if (p == board.tail) removeTail(board); // If it's tail, remove the tail
    else
    {
        Node *q = p->next;
        *p = *q;
        if (q == board.tail) removeTail(board); // Check if p is the second-last node.
        else delete q;
    } 
}

// Remove all remaining blocks in the board
void cleanBoard(List &board)
{
    while (board.head) removeHead(board);
}

// Visual representation of the board
void drawBoard(List board)
{
    for (Node *p = board.head; p; p = p->next)
    {
        Block block = p->block;
        if (block.mode == LOCKED)
            drawBlock(block, GREEN, LIGHT_AQUA);
        else if (block.isSelected == true)
            drawBlock(block, WHITE, BLACK);
        else
            drawBlock(block, BLACK, WHITE);
    }
}

// Stage difficulty increase mode's functionalities
// Find the height (maximum row)
int findHeight(Node *head)
{
    if (head == NULL) return 0;

    int height = 0;
    for (Node *p = head; p; p = p->next)
        if (p->block.y > height)
            height = p->block.y;
    
    return height + 1;
}

// Find the width (maximum column)
int findWidth(Node *head)
{
    if (head == NULL) return 0;

    int width = 0;
    for (Node *p = head; p; p = p->next)
        if (p->block.x > width)
            width = p->block.x;
    
    return width + 1;
}

// If the cursor moves out of the board, bring it back
void resetOutofBound(int &x, int &y, int width, int height)
{
    if (x < 0) x = 0;

    if (y < 0) y = 0;

    if (x >= width) x = width - 1;

    if (y >= height) y = height - 1;
}

// Scoring
bool scoreIMatch(int &score, List board, int x1, int y1, int x2, int y2)
{
    // Check if the two values are the same and it's a I matching pattern.
    if (check_I_Node(board.head, x1, y1, x2, y2))
    {
        score++;
        return true;
    }

    return false;
}

bool scoreLMatch(int &score, List board, int x1, int y1, int x2, int y2)
{
    // Check if it's a L matching pattern.
    if (check_L_Node(board.head, x1, y1, x2, y2))
    {
        score += 2;
        return true;
    }

    return false;
}

bool scoreZMatch(int &score, List board, int x1, int y1, int x2, int y2)
{
    // Check if it's a Z matching pattern.
    if (check_Z_Node(board.head, x1, y1, x2, y2))
    {
        score += 3;
        return true;
    }

    return false;
}

bool scoreUMatch(int &score, List board, int x1, int y1, int x2, int y2, int width, int height)
{
    // Check if it's a U matching pattern.
    if (check_U_Node(board.head, x1, y1, x2, y2, width, height))
    {
        score += 3;
        return true;
    }

    return false;
}

// Check end game
bool checkEmptyBoard(List board)
{
    if (board.head == NULL)
        return true;

    return false;
}

bool checkRemainPairs(List board, int width, int height)
{
    if (board.head == NULL) return false;
    
    for (Node *p = board.head; p->next; p = p->next)
    {
        for (Node *q = p->next; q; q = q->next)
        {
            Block first = p->block;
            Block second = q->block;

            if ((check_I_Node(board.head, first.x, first.y, second.x, second.y) 
                || check_L_Node(board.head, first.x, first.y, second.x, second.y) 
                || check_Z_Node(board.head, first.x, first.y, second.x, second.y)
                || check_U_Node(board.head, first.x, first.y, second.x, second.y, width, height))
                && first.value == second.value)

                return true; 
        }
    }

    return false;
}

void moveSuggestion(List board, int width, int height, Block &found1, Block &found2)
{
    for (Node *p = board.head; p->next; p = p->next)
    {
        for (Node *q = p->next; q; q = q->next)
        {
            Block first = p->block;
            Block second = q->block;

            if ((check_I_Node(board.head, first.x, first.y, second.x, second.y) 
                || check_L_Node(board.head, first.x, first.y, second.x, second.y) 
                || check_Z_Node(board.head, first.x, first.y, second.x, second.y)
                || check_U_Node(board.head, first.x, first.y, second.x, second.y, width, height))
                && first.value == second.value)
            {
                found1 = first;
                found2 = second;
                return; 
            }
        }
    }
}

// Sliding effect
void slidingLeft(List &board, int width, int height)
{
    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++)
            // Check if this node is deleted
            if (gotoNode(board.head, x, y) == NULL)
                for (int i = x + 1; i < width; i++)
                {
                    // If the next node after is not deleted yet
                    // perform sliding 
                    Node *q = gotoNode(board.head, i, y);
                    if (q)
                    {
                        q->block.x--;   
                    }
                }
}

// Stage difficulty increase mode's logic
void loadChallengeMode(GameInfo &game, int duration)
{
    // Seed for random
    srand(time(0));

    // Initialize board
    List board = createList(NULL);
    initBoard(board, 6);

    // Initialize game information
    int score = game.score;
    game.gameFinished = false;

    // Initialize time
    std::thread timer(countDownTimer, &duration);
    stopTimer = false;

    // Initialize cursor
    int cursor_x = 0, cursor_y = 0;
    Node *first = nullptr, *second = nullptr;
    Node *current;
    int nLocks = 0;
    int c;

    // Main game loop
    while (true)
    {        
        int width = findWidth(board.head);
        int height = findHeight(board.head);

        // Reset the cursor
        resetOutofBound(cursor_x, cursor_y, width, height);        
        current = gotoNode(board.head, cursor_x, cursor_y);
        if (current == NULL) current = board.head; // If the cursor is on deleted node, bring it back to the head.
        current->block.isSelected = true;

        // Drawing
        system("cls");
        drawBoard(board);
        drawScoreBoard(score);

        c = getch();

        if (c == 0 || c == 224)
            moveCursor(c, cursor_x, cursor_y, &current->block);

        else if (c == SPACE_KEY)
        {
            if (current != NULL)
                lockCursor(nLocks, &current->block);

            if (nLocks == 1)
                first = current;
            else if (nLocks == 2)
                second = current;

        }
        
        // Unlocking selected blocks
        else if (c == 'u')
            unlockCursor(nLocks, &current->block);

        // Confirm blocks
        else if (c == '\r')
        {
            if (first != nullptr && second != nullptr)
            {
                if (first->block.value == second->block.value)
                {
                    int x1 = first->block.x, x2 = second->block.x;
                    int y1 = first->block.y, y2 = second->block.y;
                    //Start scoring after confirming
                    bool isScored = false;
                    if (scoreIMatch(score, board, x1, y1, x2, y2))
                            isScored = true;
                    else if (scoreLMatch(score, board, x1, y1, x2, y2))
                            isScored = true;
                    else if (scoreZMatch(score, board, x1, y1, x2, y2))
                            isScored = true;
                    else if (scoreUMatch(score, board, x1, y1, x2, y2, width, height))
                            isScored = true;
                    
                    else
                    {
                        // If the selected blocks do not match, unlock both of them.
                        unlockCursor(nLocks, &first->block);
                        unlockCursor(nLocks, &second->block);
                        first = second = nullptr;
                    }

                    if (isScored)
                    {
                        removeBlock(board, first->block.x, first->block.y);
                        removeBlock(board, second->block.x, second->block.y);
                        nLocks = 0;
                    }
                }
                Sleep(500);
            }
        }
        
        // Move suggestion
        else if (c == 'h')
        {
            Block f1, f2;
            moveSuggestion(board, width, height, f1, f2);
            // Highlighting the suggested blocks.
            drawBlock(f1, PURPLE, WHITE);
            drawBlock(f2, PURPLE, WHITE);
            Sleep(1500); // Delay for 1,5 seconds for the player to see.
        }

        // Quiting the game
        else if (c == ESC)
        {
            cleanBoard(board);
            game.score = score;
            stopTimer = true;
            saveGame("Quiting game!");
            break;
        }

        if (checkEmptyBoard(board))
        {
            cleanBoard(board);
            game.score = score;
            game.gameFinished = true;
            stopTimer = true;
            saveGame("Board is empty!");
            break;
        }

        if (!checkRemainPairs(board, width, height))
        {
            cleanBoard(board);
            game.score = score;
            game.gameFinished = true;
            stopTimer = true;
            saveGame("No valid pairs remain!");
            break;
        }

        if (duration <= 0)
        {
            cleanBoard(board);
            game.score = score;
            saveGame("Time up! You lost!");
            break;
        }

        slidingLeft(board, width, height);
    }

    timer.join();
}