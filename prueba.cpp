#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <ctime>
#include <random>
#include <vector>

#define tamImgX 700.0f
#define tamImgY 500.0f
#define tamvX 1100
#define tamvY 600

using namespace sf;
using namespace std;

float random(float min, float max) {
    random_device rd;
    mt19937 gen(rd());

    uniform_real_distribution<float> dis(min, max);

    float random_number = dis(gen);
    return random_number;
}

class Pieza {
protected:
public:
    Sprite pieza;
    bool mover = true, animar = true, ocupada = false;
    Vector2f velocidad;
    void animacion();
};

void Pieza::animacion() {
    if (mover) {
        velocidad.x = random(-0.6f, 0.6f);
        velocidad.y = random(1.0f, 1.5f);
        mover = false;
    }
    if (animar)
    {
        int x = pieza.getPosition().x;
        int y = pieza.getPosition().y;
        int xx = pieza.getGlobalBounds().width;
        int yy = pieza.getGlobalBounds().height;
        pieza.move(velocidad);
        if (x - (xx / 2) <= 0) {
            pieza.setPosition((xx / 2), y);
            animar = false;
        }
        if (y - (yy / 2) <= 0) {
            pieza.setPosition(x, (yy / 2));
            animar = false;
        }
        if (x + (xx / 2) >= tamvX) {
            pieza.setPosition(tamvX - (xx / 2), y);
            animar = false;
        }
        if (y + (yy / 2) >= tamvY) {
            pieza.setPosition(x, tamvY - (yy / 2));
            animar = false;
        }
    }
}

class Imagen :public Pieza {
protected:
    string jpg = "";
    int indice_Pieza = -1;
    int indice_Pos = -1;
    bool seleccionada = false;
    vector<Pieza> img_div;
    vector<Pieza> posicion;
public:
    Imagen();
    void mostrar();
    void acomodarTam(Sprite& sprite, Texture texture, float escalaX, float escalaY);
    void div_Img(Texture& texture, int n_Div, RenderWindow& window);
    void show_Div_Img(RenderWindow& window);
    void clickPieza(Vector2i& pos);
    void soltarPieza(Vector2i& pos);
};

Imagen::Imagen() {
    srand(time(NULL));
    /*int p;
    do {
        p = rand() % 6;
    } while (p == 0);
    switch (p) {
    case 1: {
        jpg = "1217604.jpg";
        break;
    }
    case 2: {
        jpg = "1107549.jpg";
        break;
    }
    case 3: {
        jpg = "698414.jpg";
        break;
    }
    case 4: {
        jpg = "759633.png";
        break;
    }
    case 5: {
        jpg = "1217886.jpg";
        break;
    }
    }*/

    jpg = "imagenes/img.jpg";
}

void Imagen::mostrar() {
    RenderWindow window(VideoMode(tamvX, tamvY), "Mi ventana");
    Texture imagen;
    imagen.loadFromFile(jpg);
    Sprite sprite(imagen);
    acomodarTam(sprite, imagen, tamImgX, tamImgY);
    div_Img(imagen, 8, window);
    Vector2i actual;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
                window.close();
            }
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                actual = Mouse::getPosition(window);
                clickPieza(actual);
                soltarPieza(actual);
                if (indice_Pos >= 0) {
                    if (posicion[indice_Pos].ocupada == true) {
                        posicion[indice_Pos].ocupada = false;
                        indice_Pos = -1;
                    }
                }
            }
            if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
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
        }
        window.clear();
        for (auto& s : img_div) {
            s.animacion();
        }
        show_Div_Img(window);
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

void Imagen::div_Img(Texture& texture, int n_Div, RenderWindow& window) {
    int ancho = texture.getSize().x / n_Div;
    int alto = texture.getSize().y / n_Div;

    Texture texture2;
    texture2.loadFromFile("negro.jpg");
    int ancho2 = texture2.getSize().x / n_Div;
    int alto2 = texture2.getSize().y / n_Div;

    for (int y = 0; y < n_Div; y++) {
        for (int x = 0; x < n_Div; x++) {
            IntRect rect(x * ancho, y * alto, ancho, alto);
            Pieza aux;
            aux.pieza.setTexture(texture);
            aux.pieza.setTextureRect(rect);
            acomodarTam(aux.pieza, texture, tamImgX, tamImgY);
            aux.pieza.setPosition((x * aux.pieza.getGlobalBounds().width) + aux.pieza.getGlobalBounds().width + 150, (y * aux.pieza.getGlobalBounds().height) + aux.pieza.getGlobalBounds().height);
            aux.pieza.setOrigin(ancho / 2, alto / 2);
            img_div.push_back(aux);

            IntRect rect2(x * ancho2, y * alto2, ancho2, alto2);
            Pieza fondo;
            fondo.pieza.setTexture(texture2);
            fondo.pieza.setTextureRect(rect2);
            acomodarTam(fondo.pieza, texture2, tamImgX, tamImgY);
            fondo.pieza.setPosition((x * fondo.pieza.getGlobalBounds().width) + fondo.pieza.getGlobalBounds().width + 150, (y * fondo.pieza.getGlobalBounds().height) + fondo.pieza.getGlobalBounds().height);
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
        window.draw(img_div[i].pieza);
    }
}

int main()
{
    Imagen g;
    g.mostrar();
    system("pause");
    return 0;
}