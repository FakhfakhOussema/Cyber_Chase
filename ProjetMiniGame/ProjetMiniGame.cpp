#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <random>

using namespace std;
using namespace sf;
int hackercount = 2;
int xtaille = 600;
int ytaille = 600;
bool game = true;
int score = 0;
int nbrCollecte = 0;

struct User
{
    //x:abcisse , y:ordonnee , dx:deplacement selon x , dy:deplacement selon y 
    int x, y, dx, dy;
    User()
    {
        x = 70;
        y = 20;
        dx = 0;
        dy = 0;
    }
    void move()
    {
        x = x + dx;
        y = y + dy;
        //l'image qui represente User est de dimensions 34 50
        if (x < 0 || x>(xtaille - 34)) x = x - dx;
        if (y < 0 || y>(ytaille - 50)) y = y - dy;
    }

};

struct Pc
{
    //x:abcisse , y:ordonnee , dx:deplacement selon x , dy:deplacement selon y 
    int x, y, dx, dy;
    Pc()
    {
        x = 100;
        y = 100;
        dx = 0;
        dy = 0;
    }
    void move()
    {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(0, 520);
        dx = dis(gen);
        dy = dis(gen);
        x = dx;
        y = dy;
        //l'image qui represente PC et de dimensions 50 41
        if (x < 20 || x>(xtaille - 50))
        {
            dx = -dx;
            x = x + dx;
        }
        if (y < 20 || y>(ytaille - 41))
        {
            dy = -dy;
            y = y + dy;
        }
    }
};

struct Hacker
{
    //x:abcisse , y:ordonnee , dx:deplacement selon x , dy:deplacement selon y 
    int x, y, dx, dy;

    Hacker()
    {
        x = 300;
        y = 300;
        dx = 4 - rand() % 8; // un nombre alea entre -3 et 4 
        dy = 4 - rand() % 8;
    }
    void move()
    {
        x = x + dx;
        y = y + dy;
        //l'image qui represente Hacker et de dimensions 33 50
        if (x < 0 || x>(xtaille - 33))
        {
            dx = -dx;
            x = x + dx;
        }
        if (y < 0 || y>(ytaille - 50))
        {
            dy = -dy;
            y = y + dy;
        }
    }

};

int main()
{
    RenderWindow window(VideoMode(xtaille, ytaille), "Cyber Chase");
    window.setFramerateLimit(80); // ralentir le fenetre

    //importer les images 
    Texture t1, t2, t3, t4, t5;
    t1.loadFromFile("images/background.png");
    t2.loadFromFile("images/user.png");
    t3.loadFromFile("images/hackers.png");
    t4.loadFromFile("images/pc.png");
    t5.loadFromFile("images/gameover.png");
    Sprite sBackground(t1);
    Sprite sUser(t2);
    Sprite sHackers(t3);
    Sprite sPc(t4);
    Sprite sGameover(t5);

    //importer les musiques 
    Music music, music1;
    music.openFromFile("musique/background.ogg");
    music1.openFromFile("musique/end.ogg");


    //importer fonts
    Font font;
    font.loadFromFile("fonts/Glitch.ttf");
    Text scoreText;
    scoreText.setFont(font);

    //activer la musique
    music.play();
    music.setVolume(15);
    music.setLoop(true);
    window.clear();

    //instanciation
    User user;
    Pc pc;
    Hacker hacker[20];

    while (window.isOpen())
    {
        Event event;
        //les evenements
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)window.close();
            if (Keyboard::isKeyPressed(Keyboard::Left)) { user.dx = -2; user.dy = 0; };
            if (Keyboard::isKeyPressed(Keyboard::Right)) { user.dx = 2; user.dy = 0; };
            if (Keyboard::isKeyPressed(Keyboard::Up)) { user.dx = 0; user.dy = -2; };
            if (Keyboard::isKeyPressed(Keyboard::Down)) { user.dx = 0; user.dy = 2; };
        }

        if (game == false)continue;

        window.draw(sBackground);

        // deplacement de joueur
        RectangleShape rect_user(Vector2f(34, 50));
        rect_user.setFillColor(Color::Red);
        user.move();
        rect_user.setPosition(user.x, user.y);
        sUser.setPosition(user.x, user.y);
        window.draw(sUser);

        //deplacement du pc 
        sPc.setPosition(pc.x, pc.y);
        RectangleShape rect_pc(Vector2f(50, 41));
        rect_pc.setFillColor(Color::Red);
        rect_pc.setPosition(pc.x, pc.y);
        //collision entre pc et user
        if (rect_user.getGlobalBounds().intersects(rect_pc.getGlobalBounds()))
        {
            // Collision détectée !
            pc.move();
            score = score + 10;
            nbrCollecte++;
        }
        window.draw(sPc);

        //deplacement du hackers
        for (int i = 0; i < hackercount; i++)
        {
            RectangleShape rect_hacker(Vector2f(25, 40));
            rect_hacker.setFillColor(Color::Red);
            rect_hacker.setPosition(hacker[i].x + 3, hacker[i].y + 3);
            sHackers.setPosition(hacker[i].x, hacker[i].y);
            hacker[i].move();
            window.draw(sHackers);
            //collision entre hacker et user
            if (rect_user.getGlobalBounds().intersects(rect_hacker.getGlobalBounds()))
            {
                game = false;
            }
        }

        if (hackercount <= 19)
        {
            if (nbrCollecte == 5)
            {
                hackercount++;
                nbrCollecte = 0;
            }
        }

        if (game == false)
        {
            window.draw(sGameover);
            music.stop();
            music1.play();
            music1.setVolume(15);
            scoreText.setCharacterSize(60);
            scoreText.setPosition(150, 500);
            scoreText.setString("Score final : " + to_string(score));
            window.draw(scoreText);
        }

        window.display();

    }
    return 0;
}


