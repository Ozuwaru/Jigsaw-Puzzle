#include <glfw/glfw3.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <ctime>
#include <random>
#include <vector>
#include <algorithm>

#define tamImgX 800.0f
#define tamImgY 500.0f
#define tamvX 1366
#define tamvY 700

using namespace sf;
using namespace std;

float random(float min, float max){
    random_device rd;
    mt19937 gen(rd());

    uniform_real_distribution<float> dis(min, max);

    float random_number = dis(gen);
    return random_number;
}

class Imagen:public Pieza{
protected:
    string jpg="Imagenes/img_";
    int indice_Pieza = -1;
    int indice_Pos = -1;
    int n_Div;
    bool seleccionada=false;
    int cant_P, player;
    vector<Pieza> img_div;
    vector<Pieza> posicion;
    vector<int> impresion;
    public:
        Imagen(int jugador, int dif);
		Cronometro mostrar(RenderWindow& window);
        void mostrar();
        void acomodarTam(Sprite &sprite, Texture texture, float escalaX, float escalaY);
        void div_Img(Texture &texture, RenderWindow& window);
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
                if (it == impresion.end() && random<tamMax)
                    impresion.push_back(random);
            }
        }
};

Imagen::Imagen(int jugador, int dif){
    srand(time(NULL));
    int p;
    do {
        p = rand() % 6;
    } while (p < 1 || p > 5);
    string aux = to_string(p);
    jpg+=(aux+".jpg");
    
    switch (dif) {
        case 1: {
            n_Div = 6;
            break;
        }
        case 2: {
            n_Div = 7;
            break;
        }
        case 3: {
            n_Div = 8;
            break;
        }
    }
    if (jugador == 0){
        cant_P = 1;
        jugador = 0;
    }
    else
    {
        cant_P = 2;
        player = jugador;
    }
}

void Imagen::mostrar() {
    RenderWindow window(VideoMode(tamvX, tamvY), "Mi ventana");
    Texture imagen;
    imagen.loadFromFile(jpg);
    Sprite sprite(imagen);
    if (cant_P == 1){
        acomodarTam(sprite, imagen, tamImgX, tamImgY);
    }
    div_Img(imagen, window);
    Vector2i actual;
    printAleatorio();
    bool aux = true;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space) {
                if (verificar())
                    cout << "Completo";
                else
                    cout << "Incompleto";
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
            if (cant_P == 1) {
                img_div[indice_Pieza].colision(aux, true, true,50,50,tamvX-50,tamvY-50);
            }
            if (cant_P == 2 && player == 1) {
                img_div[indice_Pieza].colision(aux, true, true, 50, 50, 660, tamvY - 50);
            }
            if (cant_P == 2 && player == 2) {
                img_div[indice_Pieza].colision(aux, true, true, 680, 50, tamvX-50, tamvY - 50);
            }
        }
        window.clear();
        int j = 0, k = 0;
        show_Div_Img(window);
        for (int i = 0; i < img_div.size(); i++) {
            j += 1;
            if (cant_P == 1)
                img_div[impresion[i]].animacion(j, k, n_Div, 50, player, true);
            if (cant_P == 2 && i < img_div.size() / 2)
                img_div[impresion[i]].animacion(j, k, n_Div, 50, player, true);
            else if(cant_P == 2 && i >= img_div.size() / 2)
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

void Imagen::acomodarTam(Sprite& sprite, Texture texture, float escalaX,float escalaY){
    float x = escalaX / texture.getSize().x;
    float y = escalaY / texture.getSize().y;
    sprite.setScale(x, y);
}

void Imagen::div_Img(Texture &texture, RenderWindow& window){
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
                aux.pieza.setPosition((x * aux.pieza.getGlobalBounds().width) + 250, (y * aux.pieza.getGlobalBounds().height) + aux.pieza.getGlobalBounds().height/2 + 70);
            }
            if (cant_P == 2&& player == 1) {
                acomodarTam(aux.pieza, texture, 500, 400);
                aux.pieza.setPosition((x * aux.pieza.getGlobalBounds().width) + (aux.pieza.getGlobalBounds().width / 2) + 50, (y * aux.pieza.getGlobalBounds().height) + aux.pieza.getGlobalBounds().height / 2 + 70);
            }
            if (cant_P == 2&& player == 2) {
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

void Imagen::show_Div_Img(RenderWindow &window) {
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

int main()
{
    cout << "1.Un jugador 2.Dos jugadores";
    int aux;
    cin >> aux;
    cout << "\nIngrese la dificultad: 1.6x6 2.7x7 3.8x8 \nOp:";
    int op;
    cin >> op;
    if (aux == 1) {
        Imagen g(0 , op);
        g.mostrar();
    }
    if (aux == 2) {
        int aux2;
        cout << "Jugador 1 o 2: ";
        cin >> aux2;
        Imagen g(aux2, op);
        g.mostrar();
    }
    system("pause");
    return 0;
}