#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include <ctime>
#include <random>
#include <vector>
#include "Archivo.h"
#define tamImgX 1000
#define tamImgY 600
#define tamvX 1700
#define tamvY 800

using namespace sf;
using namespace std;

RectangleShape boton(RenderWindow &window,string s, int x, int y, Color color = Color::Color(115, 79, 154)) {
    RectangleShape boton(Vector2f(250, 60));
    boton.setFillColor(color);
    boton.setOrigin(boton.getSize().x / 2, boton.getSize().y / 2);
    boton.setPosition(x, y);
    Text texto;
    texto.setString(s);
    Font font;
    font.loadFromFile("fonts/arial.ttf");
    texto.setFont(font);
    texto.setOrigin(texto.getLocalBounds().width / 2, texto.getLocalBounds().height / 2);

    texto.setPosition(x, y - 8);
    window.draw(boton);
    window.draw(texto);
    return boton;
}

void MostrarFondo(RenderWindow& window, int img) {
    sf::Texture texture;
    string path = "bg/";
    switch (img) {
        case 1:{
            path.append("1.jpg");
            break;
        }
        case 2: {
            path.append("2.png");
            break;
        }
    }

    if (!texture.loadFromFile(path))
    {
        cout << "No se encuentra la foto.";
    }

    sf::Sprite sprite(texture);
    sprite.setScale(window.getSize().x / sprite.getLocalBounds().width, window.getSize().y / sprite.getLocalBounds().height);
    window.draw(sprite);


}

class Cronometro {
public: 
    int minutos=0, segundos=0,milisegundos=0;
    bool inicio = false;

    void Iniciar(Clock &c) {
        if (!inicio) {
            inicio = true;
            c.restart();
        }
        
    }
    void mostrarTiempo( int x, int y, int size , RenderWindow &window,Clock &c2) {
        if (!inicio) {
            c2.restart();
        }
        Time time = c2.getElapsedTime();

        if (time.asSeconds() >= 60) {
            this->minutos = time.asSeconds() / 60;

        }
        this->segundos = time.asSeconds() - (this->minutos * 60);
        this->milisegundos = (time.asSeconds() - (this->minutos * 60) - this->segundos) * 10;
        Font font;
        string path = "fonts/time.otf";
        if (!font.loadFromFile(path)) {
            cout << "La fuente no fue encontrada";
        }
        else {
            Text text;
            text.setFont(font);
        
            string s = "time: " + to_string(this->minutos) + ':' + to_string(this->segundos)  + '.' + to_string(this->milisegundos);
            text.setString(s);
            text.setCharacterSize(size);
            text.setFillColor(Color::White);
            text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
            text.setPosition(x, y);
            window.draw(text);
        
        }
    }
};

float random(float min, float max) {
    random_device rd;
    mt19937 gen(rd());

    uniform_real_distribution<float> dis(min, max);

    float random_number = dis(gen);
    return random_number;
}

class Pieza {
public:
    Sprite pieza;
    bool mover = true;
    bool animar = true;
    bool animar2 = true;
    bool animar3 = true;
    bool ocupada = false;
    Vector2f velocidad;
    void animacion(int i, int j, int div, int limY, int player, bool sitio);
    void colision(bool& cond, bool movX, bool movY, int limX, int limY, int limX2, int limY2);
};

void Pieza::animacion(int i, int j, int div, int limY, int player, bool sitio) {
    if (mover) {
        velocidad.x = random(0.4f, 0.8f);
        velocidad.y = random(1.0f, 1.2f);
        mover = false;
    }
    if (animar)
    {
        pieza.move(velocidad);
        if (player == 0)
            colision(animar, true, true, 50, 50, tamvX - 50, tamvY - 50);
        if (player == 1)
            colision(animar, false, true, 25, 50, 660, tamvY - 50);
        if (player == 2)
            colision(animar, false, true, 675, 50, tamvX - 50, tamvY - 50);
    }
    if (sitio) {
        if (!animar && animar2) {
            pieza.move(6.0f, 0.0f);
            if (player == 0)
                colision(animar2, true, false, 50, 50, tamvX - 50, tamvY - 50);
            if (player == 1)
                colision(animar2, true, false, 25, 50, 660, tamvY - 50);
            if (player == 2)
                colision(animar2, true, false, 675, 50, tamvX - 50, tamvY - 50);
        }
        else if (!animar && !animar2 && animar3) {
            pieza.move(0.0f, -6.0f);
            int x = pieza.getPosition().x;
            int y = pieza.getPosition().y;
            int yy = pieza.getGlobalBounds().height;
            if (div == 6) {
                if (y - ((i * yy) / 2) <= limY) {
                    pieza.setPosition(x - (150 * j), (i * (yy / 2)) + limY);
                    animar3 = false;
                }
            }
            if (div == 7) {
                if (y - ((i * yy) / 2) <= limY) {
                    pieza.setPosition(x - (150 * j), (i * (yy / 2)) + limY);
                    animar3 = false;
                }
            }
            if (div == 8) {
                if (y - ((i * yy) / 2) <= limY) {
                    pieza.setPosition(x - (150 * j), (i * (yy / 2)) + limY);
                    animar3 = false;
                }
            }
        }
    }
}

void Pieza::colision(bool& cond, bool movX, bool movY, int limX, int limY, int limX2, int limY2) {
    int x = pieza.getPosition().x;
    int xx = pieza.getGlobalBounds().width;
    int y = pieza.getPosition().y;
    int yy = pieza.getGlobalBounds().height;

    if (movX) {
        if (x - (xx / 2) <= limX) {
            pieza.setPosition((xx / 2) + limX, y);
            cond = false;
        }
        if (x + (xx / 2) >= limX2) {
            pieza.setPosition(limX2 - (xx / 2), y);
            cond = false;
        }
    }
    if (movY) {
        if (y - (yy / 2) <= limY) {
            pieza.setPosition(x, (yy / 2) + limY);
            cond = false;
        }
        if (y + (yy / 2) >= limY2) {
            pieza.setPosition(x, limY2 - (yy / 2));
            cond = false;
        }
    }
    if (x + (xx / 2) >= limX2 && y + (yy / 2) >= limY2) {
        pieza.setPosition(limX2 - (xx / 2), limY2 - (yy / 2));
        cond = false;
    }
    if (x - (xx / 2) <= limX && y + (yy / 2) >= limY2) {
        pieza.setPosition((xx / 2) + limX, limY2 - (yy / 2));
        cond = false;
    }
    if (x + (xx / 2) >= limX2 && y - (yy / 2) <= limY) {
        pieza.setPosition(limX2 - (xx / 2), (yy / 2) + limY);
        cond = false;
    }
    if (x - (xx / 2) <= limX && y - (yy / 2) <= limY) {
        pieza.setPosition((xx / 2) + limX, (yy / 2) + limY);
        cond = false;
    }
}

class Imagen :public Pieza {
protected:
    string jpg = "Imagenes/img_";
    int comodines;
    int indice_Pieza = -1;
    int indice_Pos = -1;
    int n_Div;
    bool seleccionada = false;
    int cant_P, player;
    vector<Pieza> img_div;
    vector<Pieza> posicion;
    vector<int> impresion;
public:
    Imagen(int jugador, int dif);
    Cronometro mostrar(RenderWindow& window);
    void MostrarImagenNormal(RenderWindow& window);
    void acomodarTam(Sprite& sprite, Texture texture, float escalaX, float escalaY);
    void div_Img(Texture& texture, RenderWindow& window);
    void show_Div_Img(RenderWindow& window);
    void clickPieza(Vector2i& pos);
    void soltarPieza(Vector2i& pos);
    bool verificar();
    void printAleatorio() {
        srand(time(NULL));
        int tamMax = n_Div * n_Div;
        int random;

        while (impresion.size() < tamMax) {
            random = rand() % tamMax;
            auto it = find(impresion.begin(), impresion.end(), random);
            if (it == impresion.end() && random < tamMax)
                impresion.push_back(random);
        }
    }
};

Imagen::Imagen(int jugador, int dif) {
    srand(time(NULL));
    int p;
    do {
        p = rand() % 6;
    } while (p < 1 || p > 5);
    string aux = to_string(p);
    jpg += (aux + ".jpg");

    switch (dif) {
    case 1: {
        n_Div = 6;
        comodines = 4;
        break;
    }
    case 2: {
        n_Div = 7;
        comodines = 2;
        break;
    }
    case 3: {
        n_Div = 8;
        comodines = 1;
        break;
    }
    }
    if (jugador == 0) {
        cant_P = 1;
        jugador = 0;
    }
    else
    {
        cant_P = 2;
        player = jugador;
    }
}
void Imagen::MostrarImagenNormal(RenderWindow& window) {
    Clock c;
    Time t;
    while (t.asSeconds() < 12) {
        cout << "Mostrando imagen"<<endl;
        t = c.getElapsedTime();
        window.clear(Color(3, 7, 8, 255));

        sf::Texture texture;

        if (!texture.loadFromFile(jpg))
        {
            cout << "No se encuentra la foto.";
        }

        sf::Sprite sprite(texture);
        sprite.setScale(tamImgX / sprite.getLocalBounds().width, tamImgY / sprite.getLocalBounds().height);
        sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);
        sprite.setPosition(700, 400);
        window.draw(sprite);
        window.display();
    
    }

}
Cronometro Imagen::mostrar(RenderWindow& window) {

    Texture imagen;
    imagen.loadFromFile(jpg);
    Sprite sprite(imagen);
    if (cant_P == 1){
        acomodarTam(sprite, imagen, tamImgX, tamImgY);
    }

    div_Img(imagen, window);
    RectangleShape a;
    Vector2i actual;
    printAleatorio();
    bool aux = true;
    Clock c;
    Cronometro crono;

    while (window.isOpen()) {
        Event event;

        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
            else if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space) {
                if (verificar())
                    return crono;
                else
                    cout << "Incompleto";
            }
            else if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                crono.Iniciar(c);

                actual = Mouse::getPosition(window);
                if (a.getGlobalBounds().contains(static_cast<float>(actual.x), static_cast<float>(actual.y))) {
                    if (comodines > 0) {
                        MostrarImagenNormal(window);
                        comodines--;
                    }
                }
                clickPieza(actual);
                soltarPieza(actual);
                if (indice_Pos >= 0) {

                    if (posicion[indice_Pos].ocupada == true) {
                        posicion[indice_Pos].ocupada = false;
                        indice_Pos = -1;
                    }
                }
            }
            else if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
                actual = Mouse::getPosition(window);
                if (seleccionada == true) {
                    soltarPieza(actual);
                    if (indice_Pos >= 0) {
                        if (posicion[indice_Pos].ocupada == false) {
                            img_div[indice_Pieza].pieza.setPosition(posicion[indice_Pos].pieza.getPosition());
                            posicion[indice_Pos].ocupada = true;
                        }
                    }
                }
                indice_Pos = -1;
                indice_Pieza = -1;
                seleccionada = false;
            }
        }
        if (seleccionada && indice_Pieza != -1) {
            Vector2i pos = Mouse::getPosition(window);
            img_div[indice_Pieza].pieza.setPosition(static_cast<float>(pos.x), static_cast<float>(pos.y));
            if (cant_P == 1) {
                img_div[indice_Pieza].colision(aux, true, true, 50, 50, tamvX - 50, tamvY - 50);
            }
            if (cant_P == 2 && player == 1) {
                img_div[indice_Pieza].colision(aux, true, true, 50, 50, 660, tamvY - 50);
            }
            if (cant_P == 2 && player == 2) {
                img_div[indice_Pieza].colision(aux, true, true, 680, 50, tamvX - 50, tamvY - 50);
            }
        }
        window.clear(Color(3, 7, 8, 255));
        int j = 0, k = 0;
        show_Div_Img(window);
        for (int i = 0; i < img_div.size(); i++) {
            j += 1;
            if (cant_P == 1)
                img_div[impresion[i]].animacion(j, k, n_Div, 50, player, true);
            if (cant_P == 2 && i < img_div.size() / 2)
                img_div[impresion[i]].animacion(j, k, n_Div, 50, player, true);
            else if (cant_P == 2 && i >= img_div.size() / 2)
                img_div[impresion[i]].animacion(j, k, n_Div, 50, player, false);
            if (n_Div == 6) {
                if (cant_P == 1) {
                    if (j % 13 == 0) {
                        j = 0;
                        k += 1;
                    }
                }
                if (cant_P == 2) {
                    if (j % 17 == 0) {
                        j = 0;
                        k += 1;
                    }
                }
            }
            if (n_Div == 7) {
                if (cant_P == 1) {
                    if (j % 16 == 0) {
                        j = 0;
                        k += 1;
                    }
                }
                if (cant_P == 2) {
                    if (j % 20 == 0) {
                        j = 0;
                        k += 1;
                    }
                }
            }
            if (n_Div == 8) {
                if (cant_P == 1) {
                    if (j % 18 == 0) {
                        j = 0;
                        k += 1;
                    }
                }
                if (cant_P == 2) {
                    if (j % 22 == 0) {
                        j = 0;
                        k += 1;
                    }
                }
            }
        }
       crono.mostrarTiempo(700, 850, 50,window,c);
        a = boton(window, "Mostrar Imagen", 700, 800);
        window.display();
    }
    
    





}

void Imagen::clickPieza(Vector2i& pos) {
    for (int i = 0; i < img_div.size(); i++) {
        if (img_div[i].pieza.getGlobalBounds().contains(static_cast<float>(pos.x), static_cast<float>(pos.y))) {
            indice_Pieza = i;
            seleccionada = true;
            break;
        }
    }
}

void Imagen::soltarPieza(Vector2i& pos) {
    for (int i = 0; i < posicion.size(); i++) {
        if (posicion[i].pieza.getGlobalBounds().contains(static_cast<float>(pos.x), static_cast<float>(pos.y))) {
            indice_Pos = i;
            break;
        }
    }
}

void Imagen::acomodarTam(Sprite& sprite, Texture texture, float escalaX, float escalaY) {
    float x = escalaX / texture.getSize().x;
    float y = escalaY / texture.getSize().y;
    sprite.setScale(x, y);
}

void Imagen::div_Img(Texture& texture, RenderWindow& window) {
    int ancho = texture.getSize().x / n_Div;
    int alto = texture.getSize().y / n_Div;


    for (int y = 0; y < n_Div; y++) {
        for (int x = 0; x < n_Div; x++) {
            IntRect rect(x * ancho, y * alto, ancho, alto);
            Pieza aux;
            aux.pieza.setTexture(texture);
            aux.pieza.setTextureRect(rect);
            if (cant_P == 1) {
                acomodarTam(aux.pieza, texture, tamImgX, tamImgY);
                aux.pieza.setPosition((x * aux.pieza.getGlobalBounds().width) + 250, (y * aux.pieza.getGlobalBounds().height) + aux.pieza.getGlobalBounds().height / 2 + 70);
            }
            if (cant_P == 2 && player == 1) {
                acomodarTam(aux.pieza, texture, 500, 400);
                aux.pieza.setPosition((x * aux.pieza.getGlobalBounds().width) + (aux.pieza.getGlobalBounds().width / 2) + 50, (y * aux.pieza.getGlobalBounds().height) + aux.pieza.getGlobalBounds().height / 2 + 70);
            }
            if (cant_P == 2 && player == 2) {
                acomodarTam(aux.pieza, texture, 500, 400);
                aux.pieza.setPosition((x * aux.pieza.getGlobalBounds().width) + (aux.pieza.getGlobalBounds().width / 2) + 680, (y * aux.pieza.getGlobalBounds().height) + aux.pieza.getGlobalBounds().height / 2 + 70);
            }
            aux.pieza.setOrigin(ancho / 2, alto / 2);
            img_div.push_back(aux);

            Texture texture2;
            texture2.loadFromFile("Imagenes/imgMatriz.jpg");
            int ancho2 = texture2.getSize().x / n_Div;
            int alto2 = texture2.getSize().y / n_Div;
            IntRect rect2(x * ancho2, y * alto2, ancho2, alto2);
            Pieza fondo;
            fondo.pieza.setTexture(texture2);
            fondo.pieza.setTextureRect(rect2);
            if (cant_P == 1) {
                acomodarTam(fondo.pieza, texture2, 500, 400);
            }
            if (cant_P == 2) {
                acomodarTam(fondo.pieza, texture2, 300, 200);
            }
            fondo.pieza.setPosition(aux.pieza.getPosition());
            fondo.pieza.setOrigin(ancho2 / 2, alto2 / 2);
            posicion.push_back(fondo);
        }
    }
}

void Imagen::show_Div_Img(RenderWindow& window) {
    for (int i = 0; i < posicion.size(); i++) {
        window.draw(posicion[i].pieza);
    }
    for (int i = 0; i < img_div.size(); i++) {
        window.draw(img_div[impresion[i]].pieza);
    }
}

bool Imagen::verificar() {
    for (int i = 0; i < (n_Div * n_Div); i++) {
        if (img_div[i].pieza.getPosition() != posicion[i].pieza.getPosition()) {
            return false;
        }
    }
    return true;
}