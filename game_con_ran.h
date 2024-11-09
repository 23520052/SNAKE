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

class Food{
private:
    Point food;
    char appearance;
public:
    inline Food(int x, int y, char __appearance){food.setX(x); food.setY(y); appearance = __appearance;}
    inline Point& getFood(){return this->food;}
    inline char getAppearance(){return this->appearance;}
    inline void setAppearance(char app){this->appearance = app;}
};

enum SnakeDirection{
    DirectionLeft,
    DirectionRight,
    DirectionUp,
    DirectionDown ,
};

class Snake{
private:
    Point head;
    std::vector<Point> body;
    SnakeDirection direction;
public:
    inline Point& getHead(){return this->head;}
    inline Point& getTail(){return this->body.back();}
    inline std::vector<Point>& getBody(){return this->body;}
    inline SnakeDirection& getDirection(){return this->direction;}
    inline void setDirection(const SnakeDirection newDir){this->direction = newDir;} // get current snake's direction
    void upsize(int noNodes); // Increase snake's size, remove 'noNodes' nodes from tail
    void downsize(int noNodes); // Decrease snake's size, add more 'noNodes' nodes to tail
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


bool Snake::foodCollision(Food food)
{
    return head == food.getFood();
}






