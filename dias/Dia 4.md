- [x] Introdução às Transformações Geométricas
- [x] Generalizando a translação
- [x] Coordenadas Homogêneas e a Matriz 3x3
- [x] Criando a classe Transform
- [x] Rotação 
- [x] Escala
- [x] Aplicando a Transform sobre um ponto
- [x] Compondo Transformações
- [x] Algoritmos de Preenchimento
- [x] A Classe `Fill`
- [x] O Algoritmo de Flood Fill
- [ ] Extras
- [x] Extra: Scanline Fill
- [ ] Exercícios

## Introdução

Na aula passada, implementamos uma função simples de translação, `translate(Point2 p, Point2 delta)`, que recebia um ponto e um deslocamento, e retornava o ponto já deslocado. Essa função resolve o problema de mover um único ponto, mas ela tem algumas limitações que vamos sentir rapidamente conforme o projeto cresce:

- Ela só sabe transladar. Se quisermos rotacionar ou escalar um objeto, precisaríamos escrever uma função nova para cada uma dessas operações.
- Ela não guarda nenhum "estado". Se quisermos aplicar várias transformações em sequência sobre um mesmo objeto (por exemplo, girar e depois mover), precisamos chamar uma função atrás da outra, manualmente, toda vez.
- Ela não é reaproveitável. Não temos como dizer "essa é a transformação do objeto X" e aplicá-la depois em vários pontos diferentes sem repetir os mesmos parâmetros.

O objetivo de hoje é generalizar essa ideia. Em vez de termos várias funções soltas, cada uma cuidando de uma transformação, vamos criar uma classe `Transform`, capaz de representar todas essas transformações geométricas de forma unificada, podendo guardar o resultado da combinação de várias transformações, e aplicar tudo isso sobre um ponto de uma só vez.

## Transformações Geométricas

Uma Transformação eométrica é uma função que recebe um ponto (ou um conjunto de pontos) e devolve um novo ponto, alterando sua posição, orientação ou tamanho. As três transformações que vamos implementar hoje pertencem a uma categoria chamada **Transformações Afins**, que têm uma propriedade importante: retas continuam retas, e proporções entre pontos colineares são preservadas. Translação, rotação e escala são os três exemplos clássicos de transformação afim.

### Por que não usar apenas funções soltas?

Existe uma alternativa muito mais elegante do que ir empilhando funções: representar cada transformação como uma **matriz**. Dessa forma, aplicar uma transformação em um ponto se torna apenas uma multiplicação entre matriz e vetor. E, o que é ainda mais importante: se quisermos aplicar várias transformações em sequência (por exemplo, primeiro rotacionar, depois transladar), basta multiplicar as matrizes de cada transformação entre si. O resultado é uma única matriz, que já representa a sequência inteira de transformações, pronta para ser aplicada em qualquer ponto do objeto.

### Coordenadas Homogêneas

Só que existe um problema: com uma matriz 2x2, conseguimos representar rotação e escala através de multiplicação, mas **não conseguimos representar translação** dessa forma, já que, como você viu, translação é uma soma, não uma multiplicação.

A solução para isso é usar **coordenadas homogêneas**: em vez de representar um ponto 2D como `(x, y)`, adicionamos uma terceira coordenada `z`, representando o ponto como `(x, y, z)`. Para pontos, usamos `z = 1`. Com essa coordenada extra, a translação também passa a ser expressável como uma multiplicação de matriz, usando agora uma matriz 3x3 para transformações em um espaço 2D.

Dessa forma, qualquer uma das nossas três transformações (translação, rotação, escala) pode ser representada por uma matriz 3x3, e aplicar a transformação em um ponto se torna simplesmente:

```
p' = M * p
```

onde `p` é o ponto em coordenadas homogêneas e `M` é a matriz 3x3 da transformação.

## Criando a classe Transform

Com essa ideia em mente, podemos pensar em como estruturar nossa classe. Ela precisa guardar internamente uma matriz 3x3 (o "estado atual" da transformação) e oferecer métodos para atualizar essa matriz quando quisermos rotacionar, transladar ou escalar.

```cpp
class Transform {
    private:
        Mat3 mat;
    public:
        Transform();
        Transform(const Transform&) = default;
        Transform(const Mat3&);

        void rotate(double degrees, Point2 axis);               /*TODO*/
        void translate(Point2 delta);                           /*TODO*/
        void scale(double deltax, double deltay, Point2 axis);  /*TODO*/

        Point2 operator*(const Point2&) const;                  /*TODO*/
        Transform operator*(const Transform&) const;            /*TODO*/
};
```

### Generalizando a translação

Lembre-se da função da aula passada, `translate(Point2 p, Point2 delta)`. Ela recebia um deslocamento `(dx, dy)` e devolvia o ponto `(x + dx, y + dy)`. Agora, em vez de aplicar isso diretamente sobre um ponto, queremos guardar esse deslocamento como uma matriz dentro da nossa `Transform`, para que ele possa ser combinado com outras transformações antes de ser aplicado.

Usando coordenadas homogêneas, essa operação pode ser escrita como a seguinte matriz 3x3:

```
| 1  0  dx |   | x |
| 0  1  dy | * | y |
| 0  0  1  |   | 1 |
```

Multiplique essa matriz pelo ponto `(x, y, 1)` no papel e verifique que o resultado é exatamente `(x + dx, y + dy, 1)`, ou seja, o mesmo resultado que a função da aula passada já calculava. A partir disso, você terá a matriz que deve ser usada dentro do método `translate` da classe `Transform`.

### Rotação

A fórmula de rotação de um ponto em torno da **origem** por um ângulo `θ` (em radianos) utiliza seno e cosseno:

```
x' = x*cos(θ) - y*sin(θ)
y' = x*sin(θ) + y*cos(θ)
```

Assim como a translação, essa fórmula também pode ser escrita como uma matriz 3x3 usando coordenadas homogêneas.

Só que, no nosso projeto, o método `rotate` recebe um parâmetro extra chamado `axis`, do tipo `Point2`. Isso acontece porque, na prática, quase nunca queremos rotacionar um objeto em torno da origem `(0, 0)` — normalmente queremos rotacioná-lo em torno de algum ponto de referência (seu centro, por exemplo). Esse ponto de referência é chamado de **pivô**.

Como a fórmula de rotação só produz o resultado esperado quando o pivô é a origem, usamos a seguinte técnica:

1. Mover o objeto de forma que o pivô coincida com a origem;
2. Aplicar a rotação normalmente (em torno da origem);
3. Retornar o objeto de volta para a posição original do pivô.

Ou seja, a rotação em torno de um pivô arbitrário é a **composição** de três transformações mais simples. Pense em como você organizaria essas três matrizes (translação para a origem, rotação, translação de volta) e em que ordem elas devem ser multiplicadas para produzir o efeito correto.

### Escala

A escala funciona de forma parecida com a rotação. Escalar um ponto em torno da origem por fatores `sx` e `sy` (um para cada eixo) é simples:

```
x' = x * sx
y' = y * sy
```

E, assim como na rotação, isso também pode ser escrito como uma matriz 3x3. E, assim como na rotação, o método `scale` também recebe um `axis`, pois normalmente queremos escalar um objeto em torno de um ponto de referência (por exemplo, seu centro), e não em torno da origem.

A mesma técnica usada para resolver o problema do pivô na rotação deve ser reaproveitada aqui, trocando apenas a matriz do meio (de rotação para escala).

## Aplicando a Transform sobre um ponto

Depois de configurar a matriz interna da nossa `Transform` (seja ela uma translação, rotação, escala, ou uma composição dessas), precisamos de uma forma de efetivamente aplicá-la sobre um ponto do nosso desenho. É para isso que existe o operador `operator*(const Point2&)`.

Lembre-se de que nossos pontos são armazenados como `Point2` (duas coordenadas), mas a matriz é 3x3 e trabalha com coordenadas homogêneas. Ou seja, antes de multiplicar, é preciso converter o `Point2` para um `Point3` com a terceira coordenada valendo `1`, multiplicar pela matriz, e depois converter o resultado de volta para `Point2`, descartando a terceira coordenada.

## Compondo Transformações

Um dos pontos mais importantes de representar transformações como matrizes é que, ao aplicarmos uma nova transformação sobre um objeto que já tinha sido transformado antes, **não precisamos refazer o trabalho do zero**. Basta multiplicar a nova matriz de transformação pela matriz que já estava guardada na nossa `Transform`.
 
Isso é o que permite, por exemplo, que você chame `rotate` e depois `translate` no mesmo objeto `Transform`, e o resultado final já leve em conta as duas operações em sequência, sem precisar guardar uma lista de transformações separadas — resolvendo exatamente as limitações que apontamos na introdução sobre a função da aula passada.

Para isso, faz sentido adicionar o operador `*`, mas dessa vez recebendo outra `Transform` como parâmetro:

```cpp
Transform operator*(const Transform&) const;
``` 

## Algoritmos de Preenchimento

Até agora, nossas formas geométricas são apenas contornos. Nós já conseguimos desenhar e transformar (mover, rotacionar, escalar) a borda de um círculo ou os lados de um polígono, mas o interior deles continua vazio, exibindo apenas a cor do plano de fundo (`Background`). 

Para criar imagens mais complexas e sólidas, precisamos de uma forma de preencher essas áreas delimitadas com uma cor, simulando o famoso "baldinho de tinta" de softwares de edição de imagem.

Existem vários algoritmos para resolver esse problema. Hoje, vamos implementar uma versão iterativa do **Flood Fill** e criar a estrutura para que possamos plugar algoritmos ainda mais eficientes no futuro.

## A Classe `Fill` 

Assim como fizemos com as formas geométricas (herdando da classe abstrata `Object`), vamos usar o polimorfismo em C++ para definir uma interface base para qualquer algoritmo de preenchimento.

O algoritmo precisará saber em qual `Canvas` ele vai desenhar, qual será o pixel de partida e qual a cor que queremos usar. Além disso, devido a arredondamentos matemáticos, duas cores visualmente iguais podem ter pequenas diferenças nos seus valores, então precisaremos de uma função que verifique se as cores combinam com uma pequena margem de tolerância.

```cpp
#ifndef FILL_HPP
#define FILL_HPP

#include "../canvas.hpp"
#include "../common.hpp"

namespace pet {
    /**
     * @class Fill
     * @brief Abstract base class for fill algorithms
     * @details Defines the interface for different fill algorithms used to fill
     * closed regions in a canvas with a specific color.
     */
    class Fill {
    public:
        virtual ~Fill() = default;
        
        /**
         * @brief Fill a region starting from a seed point
         * @param canvas The canvas to fill
         * @param seed Starting pixel coordinates
         * @param fillColor Color to fill with
         */
        virtual void fill(Canvas& canvas, const Pixel& seed, const RGBColor& fillColor) = 0;

    protected:
        /**
          * @brief Function that checks if a pixel has a color similar to that of another 
          * @param color1 Color to be compared
          * @param color2 Color to be compared
          * @param tolerance Error rate
          */
        virtual bool colorsMatch(const RGBColor& color1, const RGBColor& color2, double tolerance = 0.0) const;
    };
}

#endif
```

## O Algoritmo de Preenchimento Flood Fill

A abordagem clássica do Flood Fill tenta olhar para os vizinhos de um pixel usando recursão. O problema é que, em telas grandes, a recursão vai tão fundo que causa um erro chamado *Stack Overflow*.

Para resolver isso, usaremos um **Flood Fill Iterativo**. Em vez de usar recursão, nós mesmos vamos controlar uma estrutura de dados de Pilha (`std::stack`) da biblioteca padrão do C++ para guardar a lista de pixels que ainda precisamos visitar.

### Criando o `floodFill.hpp`

Criaremos uma nova classe chamada `FloodFill` que herde de `Fill`.

```cpp
#ifndef FLOOD_FILL_HPP
#define FLOOD_FILL_HPP

#include "fill.hpp"
#include <stack>

namespace pet {
    /**
      * @class FloodFill
      * @brief Implements a FloodFill algorithm
      */
    class FloodFill : public Fill {
        public:
            /**
            * @brief Fill a region using the floodFill algorithm
            * @param canvas The canvas to fill
            * @param seed Starting pixel coordinates
            * @param fillColor Color to fill with
            */
            void fill(Canvas& canvas, const Pixel& seed, const RGBColor& fillColor) override;
    };
}
#endif
```

Nesse sentido, seguiremos essa lógica para a implementação da função `fill`:

1. Pegamos a cor original do pixel "seed".
2. Se a cor original já for igual à cor que queremos pintar, ignoramos para evitar um loop infinito.
3. Colocamos o pixel seed na nossa pilha.
4. Enquanto a pilha não estiver vazia, retiramos o pixel do topo e olhamos para os seus 4 vizinhos (cima, baixo, direita, esquerda).
5. Se o vizinho ainda possuir a cor original, nós pintamos esse vizinho e o colocamos na pilha para analisarmos os vizinhos dele depois.

## Desafio Extra: Scanline Fill

Embora o algoritmo de Pilha resolva o nosso problema de recursão, existe um algoritmo mais profissional que pinta blocos horizontais inteiros de pixels de uma vez só: o **Scanline Seed Fill**.

O Scanline caminha para a esquerda e para a direita do pixel inicial, encontrando um segmento contínuo da mesma cor. Ele pinta a linha horizontal inteira e só procura novas sementes nas linhas exatamente acima e abaixo desse segmento.

Se você já terminou a implementação básica do `FloodFill`, recomendamos que tente criar a classe `ScanlineFill` baseada na mesma interface. Caso necessário, discuta com um Petiano sobre a lógica da implementação deste algoritmo.

## Referências

- SHIRLEY, Peter et al. **Fundamentals of Computer Graphics**, third edition.