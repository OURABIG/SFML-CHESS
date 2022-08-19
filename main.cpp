#include <SFML/Graphics.hpp>
#include <iostream>
#include <time.h>
using namespace sf;

int size = 56;

Sprite f[32];//figures

int board[8][8]=
    {-1,-2,-3,-4,-5,-3,-2,-1,
     -6,-6,-6,-6,-6,-6,-6,-6,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      6, 6, 6, 6, 6, 6, 6, 6,
      1, 2, 3, 4, 5, 3, 2, 1};

std::string tochessnote(Vector2f p)
{
    std::string s = "";
    s += char(p.x/size+97);
    s += char(7-p.y/size+49);
    return s;
}

Vector2f tocoord(char a,char b)
{
    int x = int(a) - 97;
    int y = 7-int(b)+49;
    return Vector2f(x*size,y*size);
}

void move(std::string str)
{
    Vector2f oldPos = tocoord(str[0],str[1]);
    Vector2f newPos = tocoord(str[2],str[3]);

    for(int i=0;i<32;i++)
    if(f[i].getPosition()==newPos) f[i].setPosition(-100,-100);

    for(int i=0;i<32;i++)
    if (f[i].getPosition()==oldPos) f[i].setPosition(newPos);
    
}

std::string position="";

void loadposition()
{
    int k=0;
    for(int i=0;i<8;i++)
        for(int j=0;j<8;j++)
        {
            int n = board[i][j];
            if(!n) continue;
            int x = abs(n)-1;
            int y = n>0?1:0;
            f[k].setTextureRect(IntRect(size*x,size*y,size,size));
            f[k].setPosition(size*j,size*i);
            k++;
        }
        for (int i=0;i<position.length();i+=5)
            move(position.substr(i,4));
}

int main()
{
    RenderWindow window(VideoMode(454, 455), "the chess!");

    Texture t1,t2;
    t1.loadFromFile("images/figures.png");
    t2.loadFromFile("images/board0.png");

    Sprite s(t1);
    Sprite sBoad(t2);

    for(int i=0;i<32;i++) f[i].setTexture(t1);
    loadposition();
    bool isMove=false;
    float dx=0, dy=0;
    Vector2f oldPos,newPos;
    std::string str;
    int n=0;

    while (window.isOpen())
    {
        Vector2i pos = Mouse::getPosition(window);

        Event e;
        while(window.pollEvent(e))
        {
            if(e.type == Event::Closed)
                window.close();

            //move Back//
            if(e.type == Event::KeyPressed)
                if (e.key.code == Keyboard::BackSpace)
                {position.erase(position.length()-6,5); loadposition();}
                

            //drag and drop//
            if(e.type == Event::MouseButtonPressed)
                if (e.key.code == Mouse::Left)
                {
                    for (int i = 0; i < 32; i++)
                    if(f[i].getGlobalBounds().contains(pos.x,pos.y))
                    {
                        isMove = true; n=i;
                        dx=pos.x-f[i].getPosition().x;
                        dy=pos.y-f[i].getPosition().y;
                        oldPos = f[i].getPosition();
                    }
                }
            
            if(e.type == Event::MouseButtonReleased)
                if(e.key.code == Mouse::Left)
                {
                    isMove=false;
                    Vector2f p = f[n].getPosition() + Vector2f(size/2,size/2);
                    Vector2f newPos = Vector2f(size*int(p.x/size), size*int(p.y/size));
                    str = tochessnote(oldPos) + tochessnote(newPos);
                    move(str); position+=str+"";
                    std::cout <<str<< std::endl;
                    f[n].setPosition(newPos);
                }
                    
                
        }

        //comp move//
        if(Keyboard::isKeyPressed(Keyboard::Space))
        {
            str = "d7d5";
            oldPos = tocoord(str[0],str[1]);
            newPos = tocoord(str[2],str[3]);

            for(int i=0;i<32;i++) if (f[i].getPosition()==oldPos) n=i;

            //animation//
            for(int k=0;k=50;k++)
            {
                Vector2f p = newPos - oldPos;
                f[n].move(p.x/50, p.y/50);
                window.draw(sBoad);
                for(int i=0;i<32;i++) window.draw(f[i]); window.draw(f[n]);
                window.display();
            }
            move(str); position+=str+"";
            f[n].setPosition(newPos);
        }

        if (isMove) f[n].setPosition(pos.x-dx,pos.y-dy);
        //draw//
        window.clear();
        window.draw(sBoad);
        for(int i=0;i<32;i++) window.draw(f[i]);
        window.display();
    }

    return 0;
}