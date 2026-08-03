# Dia 2

No dia anterior, fomos introduzidos a conceitos básicos como pixels, cores e formatos de arquivos de imagem, como PPM. Hoje iremos começar nosso projeto com o repositório onde vamos construir todas as ferramentas de hoje e dos próximos dias.

## Repositório dia 2:

O repositório do dia 2 contém tudo do dia 1 somando novas classes que iremos implementar. Dessa forma, temos no repositório atual novas classes como `Circle`, `Line`, `Object`, `Polyline` e `Polygon`.

Além dessas, temos uma classe `Parser` para nos fornecer uma outra forma de gerar imagens, por meio de arquivo de cenas. A estrutura de um arquivo é a seguinte:

```
<PEinT>
  <canvas filename="filename.extension" size="width height" />
  ...
</PEinT>
```

E para criar linhas, círculos e polígonos podemos usar:
```
<line start="x1 y1" end="x2 y2" />
<circle radius="radius" center="x y"/>
<polygon points="x1 y1 ... xn yn"/>
```

Dessa maneira, aqui temos um arquivo de cena com essas três formas:

```
<PEinT>
  <canvas filename="result.png" size="800 400" />

  <line start="40 360" end="760 360" />

  <circle radius="55" center="400 200"/>

  <polygon points="650 80 674 143 740 146 688 187 706 252 650 215 594 252 612 187 560 146 626 143"/>

</PEinT>
```

Sinta-se a vontade para conhecer nossos novos arquivos!
## Herança em C++

Antes de continuar, vamos estudar um conceito importante que utilizaremos ao longo do minicurso, o conceito de herança.

Linguagens orientadas a objetos implementam herança para permitir uma classe reutilizar e estender componentes de outra. Com esse recurso, uma classe derivada pode estender uma classe base para reaproveitar métodos e atributos, podendo estendê-los com uma nova implementação ou simplesmente criar novos membros que não fazem parte da classe base.

Para nós, esse mecanismo é essencial por oferecer uma interface comum para cada tipo de objeto em nosso rasterizador. A seguir, vamos entender melhor esse mecanismo.

### Como definir uma herança:

A relação de herança define uma relação do tipo **é um**. Suponha que tenhamos a seguinte classe:

```cpp
class Animal {};
```

Temos as seguintes formas de herdar dessa classe:

```cpp
class Cachorro : public Animal {};    // membros public/protected de Animal mantêm seus acessos em Cachorro.
class Gato : protected Animal {};     // membros public de Animal viram protected em Gato.
class Papagaio : private Animal {};   // membros public e protected de Animal viram private em Papagaio.
```

Note que, nos três casos temos que cada classe **é um** `Animal`. Na prática, usaremos herança da primeira forma, com formato `class Derived : public Base {};`.

### Virtual e override:

Com o mecanismo de herança implementado, algumas funcionalidades podem ser usadas. Neste minicurso, focaremos em `Virtual` e `Override`.

#### `Virtual`

A palavra-chave `Virtual` define funções que podem ou não ser sobrescritas, dependendo apenas da classe ser abstrata ou não. Uma classe abstrata é uma classe que não pode ser instanciada e que possui algum método da forma:

```cpp
virtual returnType funcName() = 0;
```

Métodos nesse formato devem, obrigatoriamente, ser sobrescritos em classes derivadas. Por exemplo, suponha que a classe `Animal` tenha um método `falar()`. Cada classe derivada deve implementá-lo:

```cpp
class Animal {
  virtual void falar() = 0;
};

class Cachorro : public Animal {
  void falar() override {
    std::cout << "Woof";
  };
};

class Gato : public Animal {
  void falar() override {
    std::cout << "Meow";
  };
};

class Papagaio : public Animal {
  void falar() override {
    std::cout << "falas de papagaio!";
  };
};
```

Outra maneira de definir métodos virtuais são métodos que podem opcionalmente ser derivados. A sintaxe é semelhante, apenas retirando o uso de `= 0;`. Por exemplo, suponha que nossa classe `Animal` quer permitir, mas não obrigar, que o método `falar()` seja sobrescrito. Basta definir como:

```cpp
  virtual void falar();
```

Dessa forma, qualquer classe derivada pode ou não sobrescrever esse método. Mas, supondo que queremos (ou precisamos) sobrescrever um método, como implementar?

#### `Override`

A palavra `Override` define métodos que, após definidos em uma classe base, estão sendo implementados em uma classe derivada. Ela previne que, em tempo de desenvolvimento, cometamos alguns erros como sobrescrever um método que esquecemos de definir. Por exemplo, suponha que uma classe base definiu um método que deve ser sobrescrito. Podemos fazer:

```cpp
class Derivada : public Base {
  returnType funcName() override {}
};
```

Exatamente a sintaxe que usamos anteriormente.

### Callback ao Dia 1

Perceba que se você fez os exercícios do Dia 1, você utilizou esses mesmos conceitos de forma informal. `Background` era uma classe base, `CheckerboardBackground` e `GradientBackground` eram classes derivadas e ambas implementavam `sample()`. Só que, do jeito que declaramos `Background` no Dia 1, isso funcionou meio que por acaso: como não usamos `virtual`, não tínhamos garantia de que uma chamada `sample()` chamaria a versão certa. Faltava dizer ao compilador que esse método deveria ser sobrescrito.

Agora que conhecemos esses conceitos, veja um exemplo de como ficaria a implementação.\
\
`antes (Dia 1)`
```cpp
class Background {
	private:
		RGBColor m_color;

	public:
		Background(RGBColor color) : m_color(color) {};

		RGBColor sample(double u, double v) const;
};
```
```cpp
RGBColor CheckerboardBackground::sample(double u, double v) const {
	/*Código escrito no dia 1*/
}
```
\
`depois`
```cpp
class Background {
	private:
		RGBColor m_color;

	public:
		Background(RGBColor color) : m_color(color) {};

		virtual ~Background() = default; // Vamos guardar Background* apontando para derivadas

		virtual RGBColor sample(double u, double v) const = 0; // Com "= 0" Background vira uma classe abstrata e sample() precisa ser sobrescrito
};
```
```cpp
RGBColor CheckerboardBackground::sample(double u, double v) const {
	/*Código escrito no dia 1*/
}
```



Com tudo isso explicado, vamos iniciar nossa primeira classe base, a `Object`.

## Criando a classe `Object`:

Para iniciar nosso projeto, precisamos de uma interface comum a todos nossos objetos, ou formas. Para isso vamos usar a classe `Object`:

```cpp
    enum class DrawMethod {
      Bresenhan = 0,
      BresenhanMidpoint,
    };

    class Object {
        protected:
            Point2 scale; //> Escala dos valores x e y do objeto
            double thick; //> Grossura das linhas do objeto que será desenhado
        public:

              /**
              * @brief Construtor parametrizado
              * @param scale Escalas x e y do objeto.
              * @param thick Grossura do objeto
              *
              */
            Object(Point2 scale = Point2(1, 1), double thick = 1) : scale(scale), thick(thick) {};

              /**
              * @brief Destrutor Padrão
              */
            virtual ~Object() = default;

              /**
              * @brief Função que desenha o objeto no Canvas
              * @param canvas Tela que será desenhada.
              * @param color Cor do objeto.
              */
            virtual void drawObject(Canvas& canvas, RGBColor color, DrawMethod method = DrawMethod::Bresenhan) = 0;
    };
```

Essa classe vive em `src/objects/object.hpp` e será a **base** de todas as formas que desenharemos. Todos os headers do projeto seguem o mesmo padrão: include guards — `#ifndef`/`#define`/`#endif` — e o namespace `pet`.

Vamos entender alguns detalhes dessa classe:

- **`scale` e `thick`**: atributos que fazem sentido para *qualquer* forma, por isso vivem na base. Eles são `protected` para que as classes derivadas possam acessá-los diretamente. Por enquanto vamos apenas armazená-los.

- **`virtual ~Object() = default;`**: Como vamos guardar formas diferentes através de ponteiros para a base (`Object*`), o destrutor da base **precisa** ser `virtual`. O `= default` apenas pede ao compilador a implementação padrão.

- **`drawObject(...) = 0`**: nosso método virtual **puro**. Ele torna `Object` uma classe abstrata e firma o contrato que *toda forma é obrigada a saber se desenhar em um `Canvas`*.

### O enum `DrawMethod`

Você deve ter notado o parâmetro `method`, de um tipo `DrawMethod` que ainda não definimos. Acontece que uma mesma reta pode ser rasterizada por algoritmos diferentes, com resultados e custos diferentes. Para deixar essa escolha nas mãos de quem chama, declaramos um `enum class` junto da `Object`, em `object.hpp`:

```cpp
enum class DrawMethod {
    Bresenhan = 0,
    BresenhanMidpoint,
};
```

Hoje vamos nos concentrar nos dois primeiros, que são duas formas do mesmo algoritmo clássico. Usamos `enum class` para que os nomes fiquem no escopo `DrawMethod::`.

## Desenhando retas com inteiros

### O problema

Uma reta é um objeto contínuo, mas nossa tela é uma matriz de pixels em posições inteiras. **Rasterizar** uma reta é escolher, entre esses pixels, quais melhor aproximam a reta ideal que vai de `(x0, y0)` até `(x1, y1)`.

Dessa forma, suponha que queremos desenhar uma reta entre `P0` e `P1`:
![P0 and P1](images/p0_and_p1.png "P0 e P1")

Uma boa solução pode ser:
![Line from P0 to P1](images/p0_to_p1_line.png "Linha de P0 a P1")

Mas como desenhá-la?

### Criando a classe `Line`

Inicialmente, vamos à nossa primeira extensão concreta de `Object`. Crie `src/objects/line.hpp`:

```cpp
class Line : public Object {
    private:
        Point2 start; //> Ponto inicial da reta.
        Point2 end;   //> Ponto final da reta.
    public:

        /**
          * @brief Construtor parametrizado
          * @param start Ponto inicial da reta.
          * @param end Ponto final da reta.
          * @param scale Escalas x e y do objeto.
          * @param thick Grossura do objeto
          */
        Line(Point2 start, Point2 end, Point2 scale = Point2(1, 1), double thick = 1) :
        Object(scale, thick), start(start), end(end) {};

        /**
          * @brief Função que desenha uma linha reta no Canvas
          * @param canvas Tela que será desenhada.
          * @param color Cor da reta.
          */
        void drawObject(Canvas& canvas, RGBColor color, DrawMethod method = DrawMethod::Bresenhan) override;
};
```

Repare como os conceitos do começo do dia aparecem aqui: `Line` **é um** `Object`, então herda `scale` e `thick`, repassa-os ao construtor da base e é obrigada (pelo `= 0` da base) a implementar `drawObject`. O `override` garante, em tempo de compilação, que estamos de fato sobrescrevendo o método certo.

#### Implementando o `drawObject` (`src/objects/line.cpp`)

A preparação é comum a todos os métodos de desenho: posição atual, diferenciais e sentidos. Em seguida, um `switch` escolhe o algoritmo pedido:

```cpp
void Line::drawObject(Canvas &canvas, RGBColor color, DrawMethod method) {
    int x = start.x();
    int y = start.y();

    int dx = end.x() - start.x(); //> Diferencial de x
    int dy = end.y() - start.y(); //> Diferencial de y

    int stepX = dx >= 0 ? 1 : -1; //> Sentido de x (1: direita, -1: esquerda)
    int stepY = dy >= 0 ? 1 : -1; //> Sentido de y (1: baixo, -1: cima)

    switch (method) {
        /* ...um case para cada DrawMethod... */
    }
}
```

Relembrando o caso anterior, o `dx` e `dy` podem ser vistos como:

![Dx Dy](images/dydx.png "Variações em ambos os eixos")



#### `DrawMethod::Bresenhan`

Usaremos como forma padrão o `DrawMethod::Bresenhan`.


```cpp
case DrawMethod::Bresenhan: {
    dx = std::abs(dx);
    dy = -std::abs(dy); //> Repare no sinal negativo!

    int d = dx + dy; //> Acumulador de erro

    canvas.add(Pixel(x, y), color);

    while (x != end.x() || y != end.y()) {
        int d2 = 2 * d;

        if (d2 >= dy) { //> Hora de avançar em x
            x += stepX;
            d += dy;
        }

        if (d2 <= dx) { //> Hora de avançar em y
            y += stepY;
            d += dx;
        }

        canvas.add(Pixel(x, y), color);
    }
    break;
}
```

O que está acontecendo aqui:

- `dy` entra **negativo** de propósito: assim um único número `d` consegue carregar a "dívida" dos dois eixos ao mesmo tempo — cada passo em x soma `dy` (puxa `d` para baixo), cada passo em y soma `dx` (puxa para cima), e o laço vive equilibrando os dois lados;
- os dois `if` são **independentes** (não é um `if/else`): quando ambos disparam na mesma iteração, o passo é diagonal;
- cada comparação (`d2 >= dy` e `d2 <= dx`) é o critério do ponto médio de um dos dois ramos da versão anterior — só que aqui os dois ramos convivem no mesmo laço, e por isso esse código funciona em **qualquer octante** sem nenhum caso especial;
- o laço roda até `(x, y)` alcançar `end` nos dois eixos, então o último pixel também é pintado.

Provar formalmente que as duas versões escolhem os mesmos pixels é um ótimo exercício — se tiver curiosidade, chame um petiano para rabiscar isso no quadro!

## Criando a classe `Circle`

Nossa segunda forma é a circunferência, definida por um centro e um raio. Crie `src/objects/circle.hpp`:

```cpp
class Circle : public Object {
    protected:
        double radius; //> Raio da circunferência.
        Point2 center; //> Centro da circunferência.
    public:

        /**
          * @brief Construtor parametrizado
          * @param radius Raio da circunferência.
          * @param center Centro da circunferência.
          * @param scale Escalas x e y do objeto.
          * @param thick Grossura do objeto
          */
        Circle(double radius, Point2 center, Point2 scale = Point2(1, 1), double thick = 1) :
        Object(scale, thick), radius(radius), center(center) {};

        /**
          * @brief Função que desenha uma circunferência no Canvas
          * @param canvas Tela que será desenhada.
          * @param color Cor da circunferência.
          */
        void drawObject(Canvas& canvas, RGBColor color, DrawMethod method = DrawMethod::Bresenhan) override;
};
```

### A matemática: simetria de 8 pontos + ponto médio

A mesma estratégia da reta funciona aqui — só trocamos a função implícita. Para uma circunferência de raio `r` centrada na origem:

```
F(x, y) = x² + y² - r²
```

- `F < 0`: ponto **dentro** da circunferência;
- `F = 0`: ponto **sobre** ela;
- `F > 0`: ponto **fora**.

E ganhamos um bônus enorme: a circunferência tem **simetria de 8 pontos**. Se `(x, y)` pertence a ela, então `(±x, ±y)` e `(±y, ±x)` também pertencem. Ou seja, basta calcular **um oitavo** do contorno (um octante) e refletir cada ponto calculado para os outros sete — somando o centro `(cx, cy)` na hora de pintar:

```
(cx + x, cy + y)   (cx - x, cy + y)   (cx + x, cy - y)   (cx - x, cy - y)
(cx + y, cy + x)   (cx - y, cy + x)   (cx + y, cy - x)   (cx - y, cy - x)
```

Vamos percorrer o octante que começa em `(0, r)`. Nele, a cada iteração fazemos um de dois movimentos: andamos para o lado (passo **E**, `x + 1`) ou descemos em direção ao eixo x (passo **S**, `y - 1`). A decisão vem, de novo, do sinal de uma variável `d` que acompanha `F` de forma **incremental**:

- Passo E:  `F(x+1, y) - F(x, y) = 2x + 1`
- Passo S:  `F(x, y-1) - F(x, y) = -2y + 1`

(confira as duas contas expandindo os quadrados!)

A regra fica: **enquanto `d < 0`, ainda estamos dentro da circunferência e podemos continuar andando com E; quando `d ≥ 0`, alcançamos ou passamos do arco, e é hora de descer com S.**

E o valor inicial? Avaliamos `F` no primeiro ponto médio, `(1, r - 1/2)`:

```
F(1, r - 1/2) = 1 + r² - r + 1/4 - r² = 5/4 - r
```

Como todas as atualizações (`2x + 1`, `-2y + 1`) são inteiras, a fração `1/4` nunca muda o resultado de uma comparação com zero — então podemos descartá-la e usar simplesmente:

```
d0 = 1 - r
```

#### Implementando o `drawObject` (`src/objects/circle.cpp`)

```cpp
void Circle::drawObject(Canvas& canvas, RGBColor color, DrawMethod) {
    int x = 0;
    int y = radius;
    int d = /*TODO: valor inicial da decisão*/;

    while (y > x) { //> Percorremos um único octante — a simetria faz o resto!
        for (int i{-1}; i <= 1; i += 2) {
            for (int j{-1}; j <= 1; j += 2) {
                canvas.add(Pixel(center.x() + x * i, center.y() + y * j), color);
                canvas.add(Pixel(center.x() + y * i, center.y() + x * j), color);
            }
        }

        if (/*TODO: ainda estamos dentro?*/) {
            /*TODO: passo E — atualize d e avance com x*/
        }
        else {
            /*TODO: passo S — atualize d e desça com y*/
        }
    }
}
```

Dois detalhes de implementação:

- Os `for` de `i` e `j` percorrem `{-1, +1}` e, junto com a troca `(x, y) → (y, x)` da segunda chamada a `add`, geram exatamente as 8 reflexões da tabela acima;
- O parâmetro `DrawMethod` fica **sem nome** na definição: a circunferência (por enquanto) tem um único algoritmo de desenho, e omitir o nome de um parâmetro não usado evita o warning do `-Wall` que o projeto compila.

## Criando `Polyline` e `Polygon`

### `Polyline`:

Uma **polirreta** é uma sequência de pontos conectados por segmentos de reta. Não precisamos de nenhum algoritmo novo, cada par de pontos consecutivos é uma `Line`. Crie `src/objects/polyline.hpp`:

```cpp
class Polyline : public Object {
    protected:
        vector<Point2> points; //> Lista de pontos da polirreta
    public:

        /**
          * @brief Construtor parametrizado
          * @param points Lista de pontos da polirreta.
          * @param scale Escalas x e y do objeto.
          * @param thick Grossura do objeto
          */
        Polyline(vector<Point2> points, Point2 scale = Point2(1, 1), double thick = 1)
        : Object(scale, thick), points(points) {};

        /**
          * @brief Função que desenha uma polirreta no Canvas
          * @param canvas Tela que será desenhada.
          * @param color Cor da polirreta.
          */
        void drawObject(Canvas& canvas, RGBColor color, DrawMethod method = DrawMethod::Bresenhan) override;
};
```

E o `drawObject` (em `src/objects/polyline.cpp`) é apenas um laço que delega o trabalho:

```cpp
void Polyline::drawObject(Canvas& canvas, RGBColor color, DrawMethod method) {
    for (size_t i{1}; i < points.size(); ++i) { //> Percorre os pares consecutivos
        /*TODO: crie uma Line de points[i - 1] até points[i]
                (repassando scale, thick e method) e desenhe-a*/
    }
}
```

### `Polygon`:

Um **polígono** é uma polirreta em que o último ponto se conecta de volta ao primeiro. Em outras palavras, um `Polygon` **é uma** `Polyline` fechada. Crie `src/objects/polygon.hpp`:

```cpp
class Polygon : public Polyline {
    public:

        /**
          * @brief Construtor parametrizado
          * @param points Lista de pontos do polígono.
          * @param scale Escalas x e y do objeto.
          * @param thick Grossura do objeto
          */
        Polygon(vector<Point2> points, Point2 scale = Point2(1, 1), double thick = 1)
        : Polyline(points, scale, thick)
        {
            /*TODO: uma única linha de código "fecha" a polirreta. Qual?*/
        };
};
```

A classe inteira herda o `drawObject` **pronto** de `Polyline` e só precisa garantir, no construtor, que o contorno se feche.


## Juntando tudo: nossa primeira cena polimórfica

Como todas as formas **são** `Object`s, podemos guardá-las juntas e desenhá-las num único laço:

```cpp
#include "common.hpp"
#include "canvas.hpp"
#include "background.hpp"
#include "line.hpp"
#include "circle.hpp"
#include "polygon.hpp"

using namespace pet;

int main() {
    Canvas canvas(800, 400, "result.ppm");

    //> Pintando o fundo, como fizemos no dia 1
    Background bkg(RGBColor(30, 30, 46, "rgb"));
    for (int j{0}; j < canvas.height(); ++j)
        for (int i{0}; i < canvas.width(); ++i)
            canvas.add(Pixel(i, j), bkg.sample(double(i) / canvas.width(), double(j) / canvas.height()));

    //> Nossas formas do dia
    Line    line(Point2(50, 350), Point2(750, 50));
    Circle  circle(80, Point2(400, 200));
    Polygon triangle({Point2(100, 300), Point2(300, 300), Point2(200, 150)});

    //> O polimorfismo em ação: um vetor de Object* desenha tudo
    std::vector<Object*> scene = { &line, &circle, &triangle };

    for (auto* obj : scene) {
        obj->drawObject(canvas, RGBColor(255, 255, 255, "rgb"));
    }

    canvas.export_img();
    return 0;
}
```

Cada chamada `obj->drawObject(...)` decide **em tempo de execução** qual implementação rodar. Se amanhã criarmos uma forma nova, esse laço não muda uma linha sequer, bastando a nova classe herdar de `Object` e implementar seus métodos.

## Exercícios

Com as quatro formas em mãos, é hora de desenhar de verdade! Algumas coisinhas novas podem aparecer, mas ajudaremos com qualquer dúvida.

#### Boneco palito

Crie uma cena com um boneco palito usando as formas de hoje:

- a cabeça é um `Circle`;
- tronco, braços e pernas podem ser `Line`s.

Dica: rascunhe as coordenadas num papel quadriculado antes de codar. Um canvas de 400×400 é um bom tamanho para começar.

#### Uma casa em um dia de sol

Agora uma cena que use **todas** as classes do dia:

- as paredes da casa são um `Polygon` retangular;
- o telhado, um `Polygon` triangular;
- porta e janelas ficam por sua conta (`Polygon`s? `Line`s?);
- o sol é um `Circle`.

## Checklist:
- [x] Apresentação do repositório do trabalho (parte 2).
- [x] Criação de classes com herança
- [x] Criando a classe Object (Polimorfismo)
- [x] Embasamento matemático de algum algoritmo de desenhar Retas.
- [x] Criando extensões da classe Object (Line e Circle)
- [x] Criando polygon e polyline
- [x] Juntando tudo: nossa primeira cena polimórfica
- [x] Criar um boneco palito
- [x] Criar uma casa em um dia de sol
- [x] Desafio: a classe Ellipse

## Referências

- SHIRLEY, Peter et al. **Fundamentals of Computer Graphics**, third edition.

