#include <vector>

class Point{
private:   
    int x, y;
public: 
    inline Point(int __x = 0, int __y = 0){this->x = __x; this->y = __y;}
    inline ~Point(){};
    inline int getX(){return this->x;}
    inline int getY(){return this->y;}
    inline void setX(int newX){this->x = newX;}
    inline void setY(int newY){this->y = newY;}
    Point& operator=(const Point &p);
    bool operator==(Point p) {return this->x == p.getX() && this->y == p.getY();}
};

Point& Point::operator=(const Point &p)
{
    if(this != &p) {
        this->x = p.x;
        this->y = p.y;
    }
    return *this; 
}

class Node{
private:
    Point coordinates;
    char appearance;
public:
    inline Node(int coordinateX = 0, int coordinateY = 0){coordinates.setX(coordinateX); coordinates.setY(coordinateY); appearance =  ' ';}
    inline Point Coordinates(){return this->coordinates;}
    inline char getAppearance(){return this->appearance;}
    inline void setAppearance(char app){this->appearance = app;}
    inline ~Node(){}
};

class Wall{
private:
    std::vector<Node> blocks;
    char appearance;
public:
    Wall(int x, int y, int width, int height, char appearance);
    inline std::vector<Node>& Blocks(){return this->blocks;}
    inline char getAppearance(){return this->appearance;} // return current appearance of wall
    inline void setAppearance(char app); // set all of node's appearance in blocks vector
};

Wall::Wall(int x, int y, int width, int height, char appearance) : appearance(appearance){
    for(int i = 0; i < height; ++i)
    {
        for(int j = 0; j < width; ++j)
        {
            Node block(x + j, y + i);
            block.setAppearance(appearance);
            blocks.push_back(block);
        }
    }
}

void Wall::setAppearance(char app)
{
    this->appearance = app;
    for (auto& block : blocks){ block.setAppearance(app);}
}

class Food{
private:
    Node food;
    char appearance;
public:
    inline Food(int x, int y, char __appearance){food.Coordinates().setX(x); food.Coordinates().setY(y); appearance = __appearance;}
    inline Node& getFood(){return this->food;}
    inline char getAppearance(){return this->appearance;}
    inline void setAppearance(char app){this->appearance = app; food.setAppearance(appearance);}
};

enum SnakeDirection{
    DirectionLeft,
    DirectionRight,
    DirectionUp,
    DirectionDown ,
};

class Snake{
private:
    Node head;
    std::vector<Node> body;
    SnakeDirection direction;
public:
    inline Node& getHead(){return this->head;}
    inline Node& getTail(){return this->body.back();}
    inline std::vector<Node>& getBody(){return this->body;}
    inline SnakeDirection& getDirection(){return this->direction;}
    inline void setDirection(const SnakeDirection newDir){this->direction = newDir;} // get current snake's direction
    void upsize(int noNodes); // Increase snake's size, remove 'noNodes' nodes from tail
    void downsize(int noNodes); // Decrease snake's size, add more 'noNodes' nodes to tail
    bool wallCollision(Wall wall); //check if the snake and a specific wall overlap each other;
    bool foodCollision(Food food); // check if the snake and a specific peice of food overlap each other
};

void Snake::upsize(int noNodes)
{
    for (int i = 0; i < noNodes; ++i){body.push_back(head);} // thêm đầu vào thân     
}

void Snake::downsize(int noNodes)
{
    for(int i = 0; i < noNodes && !body.empty(); ++i){ body.pop_back();} // loại bỏ phần đuôi     
}

bool Snake::wallCollision(Wall wall)
{
    for (Node block : wall.Blocks())
    {
        if (head.Coordinates() == block.Coordinates()) return true;
    }
    return false;
}

bool Snake::foodCollision(Food food)
{
    return head.Coordinates() == food.getFood().Coordinates();
}






