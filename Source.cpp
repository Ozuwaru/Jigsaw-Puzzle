#include "codImagen.h"
#include "Archivo.h"

class rompecabezas {
public:
    RenderWindow window;
    Event event;
    Color c1 = Color::Color(115, 79, 154);
    Color c2 = Color::Color(150, 95, 212);
    Archivo a;

    rompecabezas() {
        window.create(VideoMode(1800, 900), "Rompecabezas", Style::Default);

    }

    void linea(float x1, float y1, float x2, float y2) {

        sf::VertexArray line(TriangleStrip, 4);
        line[0].position = Vector2f(x1, y1);
        line[0].color = c1;
        line[1].position = Vector2f(x2, y1);
        line[1].color = c2;
        line[2].position = Vector2f(x1, y2);
        line[2].color = c1;
        line[3].position = Vector2f(x2, y2);
        line[3].color = c2;

        window.draw(line);
    }

    void text( string s, int x, int y, int size, int fontName = 1) {
        Font font;
        string path = "fonts/";
        switch (fontName) {
        case 0: {
            path.append("eva.otf");
            break;
        }case 1: {
            path.append("boton.ttf");
            break;
        }
        }
        if (!font.loadFromFile(path)) {
            cout << "La fuente no fue encontrada";
        }
        else {
            Text text;
            text.setFont(font);
            text.setString(s);
            text.setCharacterSize(size);
            text.setFillColor(Color::White);
            text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
            text.setPosition(x, y);
            window.draw(text);
        }
    }
    void cambiarCB(vector<RectangleShape> botones, vector<Color>& colors) {
        Vector2f pR(event.mouseMove.x, event.mouseMove.y);
        for (int i = 0; i < botones.size(); i++) {
            if (botones[i].getGlobalBounds().contains(pR)) {
                colors[i] = c2;
                break;
            }
            else if (colors[i] != c1) {
                colors[i] = c1;
                break;
            }
        }
    }

    



    int lectorDeEventos( Event& event) {
        switch (event.type)
        {
            case Event::Closed: {
                window.close();
                return 0;
                break;
            }
            //En caso que haya presionado una tecla:
            case Event::KeyPressed: {
                if (event.key.scancode == Keyboard::Scan::Enter) {

                    return 1;
                }else if (event.key.scancode == Keyboard::Scan::Backspace) {
                    return 2;

                }

                break;
            }
            case Event::Resized: {
                window.setSize(window.getSize());
                break;
            }
            case Event::MouseButtonPressed: {
                if (Mouse::isButtonPressed(sf::Mouse::Left)) {
                    return 3;
                }
                break;
            }
            case Event::TextEntered: {
                return 4;
                break;
            }
            case Event::MouseMoved: {
                return 5;
                break;
            }

        }
 }
    void singleP() {
        bool t = true;
        bool enter = false;
        string name;
        vector<Color> colors;
        colors.push_back(c1);
        colors.push_back(c1);
        colors.push_back(c1);
        bool nR = false;;
        while (t) {
            vector<RectangleShape> botones;
            window.clear(Color(3, 7, 8, 255));

            text("rompecabezas", 900, 50, 100, 0);
            linea(200, 120, 1600, 140);

        

            text("Ingrese su nombre!", 900, 200, 40);

            text(name, 900, 250, 40);
            if (enter &&name.size() > 0) {
                text("¿Esta correcto tu nombre?", 900, 300, 40);
                botones.push_back(boton(window,"Si", 450, 350, colors[0]));
                botones.push_back(boton(window, "No", 1350, 350, colors[1]));

                
            }

            if (nR) {
                text("Seleccione una dificultad: ", 900, 300, 40);
                botones.push_back(boton(window, "Facil", 600, 370, colors[0]));
                botones.push_back(boton(window, "Media", 900, 370, colors[1]));
                botones.push_back(boton(window, "Dificil", 1200, 370, colors[2]));
            }

            while (window.pollEvent(event)) {
                int ev = lectorDeEventos(event);

                switch (ev) {
                    case 0: {
                        t = false;
                        break;
                    }
                   case 1: {
                       if (name.size() > 0){
                            enter = true;

                       }
                        break;
      
                    }
                    case 2: {
                        if (!name.empty()) {
                            cout << "Contando evento";
                            name.pop_back();
                            cout << name;
                        }
                        else if(enter){
                            enter = false;
                        }
                        break;
                    }
                    case 3: {
                        if (enter && Mouse::isButtonPressed(sf::Mouse::Left)) {
                            Vector2f pR = window.mapPixelToCoords(Vector2i(event.mouseButton.x, event.mouseButton.y));


                            if (botones[0].getGlobalBounds().contains(pR)) {
                                nR = true;
                                enter = false;
                            }
                            else if (botones[1].getGlobalBounds().contains(pR)) {
                                name = "";
                                enter = false;
                            }

                        
                        }
                        else if(nR && Mouse::isButtonPressed(sf::Mouse::Left)){
                            Vector2f pR = window.mapPixelToCoords(Vector2i(event.mouseButton.x, event.mouseButton.y));
                            for (int c = 0; c < 3; c++) {
                                if (botones[c].getGlobalBounds().contains(pR)) {
                                    /*Imagen i(0, c + 1);
                                    Cronometro tiempo = i.mostrar(window);
                                    int p = (240000-(tiempo.minutos * 60) + tiempo.segundos) + tiempo.milisegundos;
                                    if (p <= 0) {
                                        p = 123;
                                    }*/
                                    //llamamos funcion para guardar archivos
                                    int p = 1500;
                                    a.addPlayer(name, p);
                                    bool presionado = false;
                                    a.guardar();
                                    Archivo b;
                                    while (!presionado) {
                                        window.clear(Color(3, 7, 8, 255));
                                        text("rompecabezas", 900, 50, 100, 0);
                                        linea(200, 120, 1600, 140);

                                        text("Felicidades por resolverlo "+name+", es el jugador #" +to_string(1), 900, 250, 70, 1);
                                        RectangleShape a= boton(window, "continuar", 900, 350);
                                        while (window.pollEvent(event)) {

                                            if (Mouse::isButtonPressed(sf::Mouse::Left)) {
                                                Vector2f pR = window.mapPixelToCoords(Vector2i(event.mouseButton.x, event.mouseButton.y));


                                                if (a.getGlobalBounds().contains(pR)) {
                                                    presionado = true;
                                                }
                                            }
                                        }
                                        window.display();
                                    }
                                    menu();
                                }
                            }

                        }
                        break;

                    }
                    case 4: {
                        
                        if (!enter) {
                            //leemos texto
                            if (event.text.unicode < 128 && event.text.unicode!=8 && event.text.unicode!=0) {
                                name.push_back(event.text.unicode);
                            }
                        }
                        break;
                    }
                    case 5: {
                        cambiarCB(botones, colors);
                        break;

                    }

                }
            }

            window.display();
        }

    }
    void  puzzleStart() {
        bool t = true;
        bool enter = false;
        //1 para 1 jugador y 2 para 2
        vector<Color> colors;
        //0 y 1 para la cantidad de jugadores
        colors.push_back(c1);
        colors.push_back(c1);
        while (t) {
            vector<RectangleShape> botones;
            window.clear(Color(3, 7, 8, 255));
            MostrarFondo(window, 1);
            text("rompecabezas", 900, 50, 100, 0);
            linea(200, 120, 1600, 140);
            text("¿Como desea jugar?", 900, 250, 40);
            botones.push_back(boton(window, "1 jugador", 650, 350, colors[0]));
            botones.push_back(boton(window, "2 jugadores", 1150, 350, colors[1]));
            while (window.pollEvent(event)) {
                int ev = lectorDeEventos(event);

                switch (ev) {
                    case 0: {
                        t = false;
                        break;
                    }
                    case 3: {
                        if (Mouse::isButtonPressed(sf::Mouse::Left)) {
                            Vector2f pR = window.mapPixelToCoords(Vector2i(event.mouseButton.x, event.mouseButton.y));

                            if (botones[0].getGlobalBounds().contains(pR)) {
                                singleP();
                            }
                            else if (botones[1].getGlobalBounds().contains(pR)) {
                                //pM = 2;
                            }
                        }
                        break;

                    }
                    case 5: {
                        cambiarCB(botones, colors);
                        break;

                    }

                }
            }

            window.display();
        }

    }
    void MostrarJ() {

    }

    void menu()
    {   
        vector<Color> colors;
        colors.push_back(c1);
        colors.push_back(c1);
        colors.push_back(c1);
        //main loop
        while (window.isOpen()) {
            vector<RectangleShape> botones;
            window.clear(Color(3, 7, 8, 255));


            VertexArray triangle(Triangles, 3);

            triangle[0].position = Vector2f(850, 150);
            triangle[0].color =Color(139, 212, 80);

            triangle[1].color = Color(139, 212, 80);
            triangle[1].position = Vector2f(950, 150);

            triangle[2].position = Vector2f(900, 50);
            triangle[2].color =Color(139, 212, 80);

            window.draw(triangle);
            linea(200, 220, 500, 265);
            text("rompecabezas", 900, 200, 150, 0);
            linea(1600, 220, 1300, 265);

            linea(600, 340, 790, 385);
            text( "v.31", 900, 310, 150, 0);
            linea(1200, 340, 1015, 385);
            botones.push_back(boton(window, "START", 900, 510,colors[0]));
            botones.push_back(boton(window, "Mostrar top 10", 900, 610,colors[1]));

            botones.push_back(boton(window, "Salir", 900, 710,colors[2]));

            while (window.pollEvent(event)) {
                int ev = lectorDeEventos(event);
                switch (ev) {
                    case 0: {
                        window.close();
                        break;
                    }
                    case 3: {
                        if (Mouse::isButtonPressed(sf::Mouse::Left)) {
                            Vector2f pR = window.mapPixelToCoords(Vector2i(event.mouseButton.x, event.mouseButton.y));
                            if (botones[0].getGlobalBounds().contains(pR)) {

                                puzzleStart();
                            }
                            else if (botones[1].getGlobalBounds().contains(pR)) {
                                window.clear(Color(3, 7, 8, 255));
                                text("Mejores 10 jugadores", 900, 100, 80, 0);

                                if (a.jugadores.size() > 0) {
                                    for (int i = 0; i < a.jugadores.size(); i++) {
                                        text("Jugador #" + to_string(i + 1) + ' ' + a.jugadores[i].d.nombre, 900, 120 * (i + 2), 70);
                                        if (i == 9) {
                                            break;
                                        }
                                    }
                                }
                                else {
                                    text("Actualmente no hay jugadores registrados", 900, 100 * (1 + 2), 70);

                                }
                                a.guardar();
                                window.display();
                                system("pause");
                            }
                            else if (botones[2].getGlobalBounds().contains(pR)) {
                                window.close();
                            }
                        }
                        break;
                    }
                    case 5:{
                        cambiarCB(botones,colors);
                    }
                }
            }

            window.display();


        }
    }
};



int main()
{
    rompecabezas r;
    r.menu();
    return 0;
} 