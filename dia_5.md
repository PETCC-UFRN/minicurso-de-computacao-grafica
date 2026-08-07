# Dia 5

## Introdução


Agora que terminamos a implementação de formas para o nosso rasterizador, vamos nos distanciar um pouquinho desse assunto e partir para outra área: filtros. Ainda que seja muito mais utilizada no processamento de imagens, essas técnicas que veremos também são utilizadas, por exemplo, na redução ruídos em rasterizadores baseados em Ray Tracers

Você já deve ter usado filtros de imagens alguma vez na sua vida. E enquanto alguns, como o filtro sépia, parecem bem intuitivos e de implementação relativamente simples, (podemos imaginar uma multiplicação de cada canal R, G e B de cada cor por uma matriz 3x1 que intensifique o vermelho e verde e neutralize o azul) o nosso foco aqui é outro: filtros de convolulções. Esses filtros são usados para diversas tarefas, desde borrões até detecção de bordas em imagens. Essa parte usa uma matemática mais pesada, sobretudo no que tange à otimização das operações que veremos aqui. No entanto, tentamos focar nas partes mais claras e evidentes.

Mas antes de chegarmos nas definições formais, vamos pensar no básico de filtros




## Filtros

É claro que existem diversos tipos de filtros. Mas não estamos interessados em filtros "sépia", "retrô" ou "cinemáticos" - queremos filtros que nos deixem tirar informações das nossas imagens! (como identificar bordas, tirar diversos tipos de ruídos, etc.).

De maneira geral, filtros funcionam a partir da avaliação de cada pixel da imagem inicial para gerarmos uma imagem final. Assim, para cada pixel da figura inicial, estaremos fazendo alguma operação para gerarmos o seu valor equivalente. Mas, como queremos extrair algum tipo de informação das nossas imagens durante esse processo, concorda que precisamos estar levando em consideração, de certa forma, os "arredores" de cada ponto? Por exemplo: digamos que você está fazendo um filtro de identificação de bordas, e busca resultados como o abaixo

<img src="./assets/dia_5/sobel.png" alt="imagem com filtro de sobel" width="600" style="display: block; margin: 0 auto">


Para que identifiquemos alguma relação do pixel com os seus arredores, precisamos considerar algum tipo de "vizinhança", para que possamos entender o papel desse pixel em seus arredores.

Em geral, então, os filtros aplicam algum tipo de função nos arredores daquele pixel para encontrar o valor resultante no local em específico. Mas existem várias formas de fazermos isso! Aqui, dividiremos os filtros a serem implementados entre *não lineares* e *lineares*, e analisaremos as diferentes maneiras que eles retratam as relações entre vizinhanças

Essa é a cara do _filters.hpp_ que está disponível para vocês.


```cpp

class Filter {
	protected:
		int kernel_size;
		virtual RGBColor filterFunction(Pixel& p, Canvas& canvas) = 0;
	public:
		Filter(int size) : kernel_size(size) {};
		virtual ~Filter() = default;
		virtual void applyFilter(Canvas& canvas);
};
```
Note que cada filtro tem um atributo chamado _kernel size_, referente ao tamanho do lado da nossa vizinhança (que será um quadrado). Além disso, podemos verificar 2 funções a serem implementadas: _applyFilter_ e _filterFunction_; Elas representam as duas etapas que precisaremos implementar para cada filtro: 1 - a função que vai iterar por todos os pixels da imagem e 2 - a função que irá ser chamada para cada pixel da imagem e calcular o valor resultante que deve ser aplicado ali.

Observe que _filterFunction_, não será mais alterada porque implementará apenas o esqueleto geral de um filtro: passar por todos os pixels da imagem, chamando a função que aplicará o filtro para cada um deles (o que é é comum a todos os filtros).

Por outro lado, a segunda função deve ser diferente para cada tipo de filtro, e todos os filtros que herdarem da classe deverão ter sua própria versão de _filterFunction_, como indica a keyword _virtual_.

(É claro que essa estrutura é apenas uma sugestão sa nossa parte e vocês podem seguir o que for mais intuitivo para vocês!)




## Filtros Não Lineares

Em sua definição formal, filtros não lineares são aqueles tais que, dados dois inputs de sinal _r_ e _s_ e suas saídas equivalentes _R_ e _S_, $\alpha \dot R + \beta \dot S $ não é sempre o output equivalente quando a entrada é uma combinação linear $\alpha \dot r + \beta \dot s $

(pra testar quem realmente pagou álgebra linear!)

Massss não vamos nos prender tanto a isso. O que isso significa de uma maneira mais prática é que esse tipo de filtro não pode ser implementado através de uma convolução padrão. Isso _também_ não nos ajuda muito por enquanto, já que só vamos definir convoluções mais à frente (mas mantenham isso em mente!)

Nessa categoria, vamos implementar apenas um filtro de mediana, mas existem vários outros!

### Filtro de Mediana

Antes da implementação, vamos pensar no seu funcionamento. Muitas vezes, durante o processo de aquisição de imagens ou transmissão desses sinais, os resultados são distintos do que queremos e apresentam ruídos. Alguns dos tipos de ruídos mais comuns são conhecidos como "sal", "pimenta" e "sal e pimenta"

<img src="./assets/dia_5/saltnpepa.png" alt="imagem com ruído sal e pimenta" width="600" style="display: block; margin: 0 auto">


Como é bem perceptível, esse tipo de ruído trata-se de pontos brancos e/ou pretos que aparecem ao longo da imagem de maneira aleatória. Esse tipo de _noise_ aparece, muitas vezes, por causa de sensores defeituosos, problemas de hardware ou na transmissão de dados (perda de dados). Para casos como esse, aplicamos um filtro de mediana! Ele é um dos filtros mais comuns (e efetivos) para a remoção desse tipo de ruído, e agora vamos entender o porquê.

Primeiro, imaginemos uma imagem em preto e branco, com valores que variam de 0 a 127, por exemplo. Note que ruídos de sal e pimenta terão, respectivamente, os valores máximo (127) e mínimo (0) como seus valores de intensidade.

Em seguida, como foi estabelecido anteriormente, iremos passar pixel por pixel e avaliar toda a região em seus arredores, para que possamos decidir qual cor vai tomar o lugar que estamos julgando. Nesse filtro, como pode ser esperado, ordenamos todos os pixels de acordo com seus valores de intensidade (como só temos um canal em imagens em preto e branco, ordenamos por esse valor!). Como estamos ordenando esses valores, as intensidades mais extremas (possivelmente sal e pimenta) serão jogadas para as pontas, e não permanecerão na imagem final.

Assim, o pixel passará a tomar o valor da mediana dentre todos os pixels da sua vizinhança. Isso pode resultar, é claro, em alterações indesejadas nas imagens - sobretudo, objetos podem ficar ligeiramente (ou significativamente, dependendo do tamanho do _kernel_ escolhido) mais finos, devido à substituição de pixels em suas bordas (pois sempre definimos o seu valor como a mediana!)

Mas isso é sobre uma imagem preto e branca. como comparaíamos pixels com 3 canais ao invés de 1? Bom, isso vai da implementação. A implementada por nõs usa uma função de comparação que apenas soma os valores dos canais R, G e B e usa esse valor resultante para a comparação! Note que essa função ainda cumpre o nosso objetivo de isolar pontos pretos e brancos nas extremidades do vetor ordenado, então é uma possível solução para a remoção desse ruído. Agora, para a implementação!

#### Implementando nosso primeiro filtro de verdade

A partir de agora, vamos começar a criar novas classes (que herdam de Filter) e codar as suas funções _filterFunction_. O _hpp_ que vocês têm disponível é mais ou menos assim:

```cpp
#ifndef MEDIAN_FILTER_HPP
#define MEDIAN_FILTER_HPP

#include "canvas.hpp"
#include "common.hpp"
#include "filter.hpp"

namespace pet {
    class MedianFilter : public Filter {
        public:
            MedianFilter(int size) : Filter(size) {};
            virtual RGBColor filterFunction(Pixel& p, Canvas& canvas) override;
    };
}


#endif
```

Então, nos falta criar um _medianFilter.cpp_ e codar nossa função!

```cpp
#include "medianFilter.hpp"
#include "canvas.hpp"
#include "common.hpp"

namespace pet {

    RGBColor MedianFilter::filterFunction(Pixel& p, Canvas& canvas){
        //TO DO
    }
};
```

Agora é com vocês! Mas lembrem-se do seguinte: vamos avaliar toda a vizinhança em um quadrado de tamanho _kernelsize_ x _kernelsize_ de cada pixel. Mas temos que prestar atenção nas nossas bordas! Não podemos acessar pixels de índices menores que 0 ou maiores que o tamanho do canvas para adicionarmos em um vetor e depois ordená-lo.

## Filtros lineares

Parabéns, você implementou o seu primeiro filtro!

Agora as coisas vão mudar um pouquinho. Como o esperado, a definição de filtro linear é a negação da que vimos anteriormente. Ou seja, dados dois inputs de sinal _r_ e _s_ e suas saídas equivalentes _R_ e _S_, $\alpha \dot R + \beta \dot S $ é sempre o output equivalente quando a entrada é uma combinação linear $\alpha \dot r + \beta \dot s $. Ou, filtros que podem ser implementados via convolução.

No entanto, ainda não sabemos o que é uma convolução e nem como ou porque ela nos afeta. Vamos ver isso a seguir, tentando seguir um _approach_ intuitivo por meio da probabilidade.



### Convoluções intuitivamente: Probabilidade

Antes de pensarmos em imagens, vamos tentar visualizar a operação com a ajuda da probabilidade.

Imagine que você está jogando algum jogo (como Catan!) e quer calcular a probabilidade de a soma dos dados ser algum número x. O _approach_ que seguimos muitas vezes é o de montar uma tabela com todas as faces de cada dado - e, claramente, isso funciona. Mas e se pensarmos um pouquinho diferente?

Vamos tentar visualizar, então, todas as possíveis faces de um dado enfileiradas (como temos 2 dados, teremos 2 fileiras!).

<img src="./assets/dia_5/dice.png" alt="dados enfileirados" width="600" style="display: block; margin: 0 auto">


Note que, ao girar uma dessas fileiras 180 graus e alinhando as duas fileiras de acordo com alguma face, estaremos gerando todas as possibilidades de somar algum resultado x!!

<img src="./assets/dia_5/dice2.png" alt="dados enfileirados" width="600" style="display: block; margin: 0 auto">

É claro que, com o uso de um dado não viciado, os resultados não vão nos trazer nada tão interessante (só aquela típica escadinha até atingirmos o ápice da função resultante, na probabilidade 7). Mas é interessante perceber o comportamento dessa função resultante com base em diferentes probabilidades de cada face de um dado.

<img src="./assets/dia_5/conv-func-1.gif" alt="convolução entre duas funçoes" width="600" style="display: block; margin: 0 auto">
<img src="./assets/dia_5/conv-func-2.gif" alt="convolução entre duas funçoes" width="600" style="display: block; margin: 0 auto">


Note que, na situação em que um dos dados é não viciado, a função resultante tem valores que assemelham a uma _"moving average"_, ou "média que se movimenta". No fim das contas, como isso é um dado, - e as probabilidades sempre somarão 1 - a função final será uma _"moving average"_, mas, dessa vez, uma média ponderada. (Vamos retomar essa observação mais à frente)

Isso é uma convolução: Uma forma de combinar funções um pouquinho diferente!

Claramente estamos exemplificando (e vamos focar durante o resto do dia) com um caso discreto, mas essa ideia de "convolucionar" (_convolve_, em inglês) também é presente para funções contínuas.


### Definindo Convoluções de Maneira Formal

Representamos as convoluções entre duas funções _f_ e _g_ como _f * g_, ou "f estrela g". Num espaço contínuo, _f * g_ pode ser definido como a integral do produto das funções após a reflexão de uma delas no eixo y e o decorrido deslocamento.

<img src="./assets/dia_5/conv-cont.png" alt="definição formal de uma convolução contínua" width="600" style="display: block; margin: 0 auto">

Masss aqui queremos dar atenção ao caso discreto (as imagens que estamos trabalhando com são discretas!!), que pode ser representado da seguinte forma:

<img src="./assets/dia_5/conv-disc.png" alt="definição formal de uma convolução contínua" width="600" style="display: block; margin: 0 auto">

E isso pode parecer um pouco assustador, mas é exatamente aquilo que vimos na seção anterior: a soma de uma multiplicação ponto-a-ponto entre funções, com uma delas sendo cada vez mais deslocada. Daqui em diante, tentaremos imaginar a nossa primeira função como o nosso canvas principal, enquanto a função deslocada será referida como _kernel_. 

Até agora, eu só representei as convoluções de uma dimensão a vocês, mas é evidente que precisaremos estar munidos de algo mais completo pra fazer operações sobre a nossa imagem - afinal, ela, por definição, é 2D! Então ao invés de uma função deslizada sobre a outra apenas horizontalmente, podemos imaginar um kernel quadrado sendo deslizado horizontalmente ao longo de cada linha (dessa forma, todos os pixels serão "processados"!)


### No processamento de imagens

Agora que temos a definição formal, podemos imaginar como seria a aplicação de algumas convoluções sobre uma imagem qualquer. E se o nosso _kernel_ tivesse probabilidades todas iguais, como um dado não viciado? Vimos anteriormente que o resultado representa uma média móvel. Então, no mundo das imagens, estaremos, no fim das contas, substituindo o pixel que estamos avaliando pela média de todos os pixels que estão sendo considerados (que fazem parte da _"neighborhood"_ ou "vizinhança"). Ou seja, todos os pixels ao redor daquele têm uma influência igual no resultado que está por vir. E aí, consegue imaginar o resultado?

### Filtro Box Blur

Quando olhamos a imagem resultante, conseguimos fazer entender o que vemos:

<img src="./assets/dia_5/box-blur.jpg" alt="filtro de média / box blur" width="600" style="display: block; margin: 0 auto">

A imagem aparenta estar mais "borrada", de certa forma. E isso faz muito sentido! Estamos atualizando o valor do nosso pixel pra um valor que também considere todos os pixels em seus arredores - ou seja, uma média de todos os pixels ao seu redor. No fim das contas, estamos "suavizando" a influência do píxel em questão. Além disso, detalhes específicos serão, de certa forma, "apagados" ou "suavizados" se existir uma região relativamente homogênea na sua vizinhança.

<img src="./assets/dia_5/box-blur-homogeneo.png" alt="visualização de um box blur sobre uma região homogênea" width="600" style="display: block; margin: 0 auto">

<img src="./assets/dia_5/box-blur-nhomogeneo.png" alt="visualização de um box blur sobre uma região não homogênea" width="600" style="display: block; margin: 0 auto">

Isso é o que chamamos de _"box blur"_. Note que o resultado final aparenta bem pixelado para nós! (decorrente dos pesos iguais)

Preparados para codar esse filtro? Lembre-se de criar outro arquivo referente à classe averageFilter que herda de Filter!

```cpp
    RGBColor AverageFilter::filterFunction(Pixel& p, Canvas& canvas){
        //TO DO
    }

```


### Filtro Customizado

Por último, vamos implementar um filtro que possibilite ao usuário escolher os pesos de cada pixel! Isso abre várias possibilidades para filtros que falarei por cima daqui a pouco.

Nesse caso, precisaremos definir um construtor que receba o nosso kernel por completo, não só o tamanho (já que o usuário estará definindo os valores de cada ponto do kernel!).

A função _filterFunction_, por fim, será bem similar à definida na seção anterior (já que também representa uma média), mas com os pesos multiplicados e uma divisão ao fim referente à soma de todos os pesos (para que não tênhamos resultados que extrapolem os limites de cor estabelecidos por nós!)

## Sobre Outros Filtros

Vamos explorar muito brevemente alguns outros filtros e suas funcionalidades.

Tente usar esse kernel para um customFilter:

```cpp
-1 -2 -1
 0  0  0
 1  2  1
```

 Consegue notar quais diferenças?
 
 Isso, na verdade, é um filtro de sobel! Ele é capaz de identificar bordas (especificamente as da direção em que ele é especificado)
 Esse filtro, na verdade, é o produto entre um kernel de média e um kernel de diferenciação:

 ```cpp
+1
 0   *  [1  2  1]
-1
```
Intuitivamente, esse kernel também faz sentido para nós. Note que ele busca a diferença (assim como a derivada!) entre os extremos do núcleo. E é exatamente isso que queremos! A _mudança_ de cores ou intensidades.




Por último, vamos tentar mais um kernel:

```cpp
 0 -1  0
-1  5 -1
 0 -1  0

```

Esse é conhecido como um filtro de sharpening, ou _aguçamento_. Note que ele traz a ideia de "distanciar" o pixel central dos pixels ao seu redor! Por isso, conseguimos notar que as características ficaram mais distintas e destacadas!


# Projeto Final

Chegamos ao fim do conteúdo desse curso. Ao longo desses 5 dias entendemos e implementamos várias coisas legais. Agora, a proposta consiste em usar a sua criatividade para fazer um vídeo(ou um **gif**) legal usando o projeto. Algo tipo assim:

```{image} assets/dia_5/output.gif
:alt: Exemplo do rasterizador desenvolvido durante o curso
:width: 600px
:align: center
```

Note que esse **GIF** é um mero compilado feito a partir de 3 imagens diferentes, e vocês **com certeza** conseguem fazer muito melhor que isso!

Para gerar essas imagens vocês vão precisar:
- Pensar em algo legal que vocês queiram animar
- "Quebrar" essa cena em vários frames
- Descrever esses frames através de um arquivo `.xml`(consultem os exemplos disponíveis em `/scenes`)
- Adaptar o `script.sh` para renderizar a sua cena

## Bash

A cena mostrada como exemplo consiste em 3 arquivos `.xml`: 
- Boneco normal
- Boneco sorrindo acenando para um lado
- Boneco sorrindo acenando para o outro lado. 

Mas a forma como eles são dispostos segue a seguinte ordem:

- `scenes/boneco_1.xml` (normal)
- `scenes/boneco_2.xml` (sorrindo acenando para um lado)
- `scenes/boneco_3.xml` (sorrindo acenando pro outro lado)
- `scenes/boneco_2.xml`
- `scenes/boneco_3.xml`

Tá, mas como esses 3 "estados" viram aquela pequena animação? Bom através de um script de `Bash`😎. Se você usa **Linux** você muito possivelmente já escutou esse nome, pois se trata do **shell** que é usado no **kernel**! Ou seja, o responsável por interpretar e executar os comandos que você digita. E algo mais legal ainda sobre isso é que o **Bash** proporciona basicamente uma linguagem na qual podemos fazer scripting! Então temos: Variáveis, IFs, Loops, Listas, além de todos os comandos já disponíveis para usar.

Certo, mas indo direto ao ponto, o script usado para gerar aquela animação foi esse aqui:

```bash
#!/bin/bash

mkdir -p frames

cycles=2

scenes=(
    "scenes/boneco_1.xml"
    "scenes/boneco_2.xml"
    "scenes/boneco_3.xml"
    "scenes/boneco_2.xml"
    "scenes/boneco_3.xml"
)

frame_count=0

for ((i=0; i<cycles; i++)); do
    for ((j=0; j<${#scenes[@]}; j++)); do
        ./build/PEinT "${scenes[$j]}"
        mv boneco.png "frames/frame_$(printf "%03d" "$frame_count").png"
        ((frame_count++))
    done
done

frame_rate=3

ffmpeg -framerate $frame_rate \
    -i frames/frame_%03d.png \
    -filter_complex "split[s0][s1];[s0]palettegen[p];[s1][p]paletteuse" \
    output.gif
```

Para utiliza-lo vocês precisarão baixar o **FFmpeg**.
```bash
sudo apt install ffmpeg
```

E dar permissão para que esse script seja executável.
```bash
chmod +x script.sh
```

## Agora é com vocês!

Não se esqueçam de [**avaliar**](https://forms.gle/LRTigAKq2RJa3VsE7) o curso! Nos ajuda demais e é uma pesquisa de satisfação rápida e anônima, então não tem desculpa para não responder 😉.

E não custa avisar que:
- É necessário 75% de presença para emitir o certificado
- Toda aula teve 2 listas de chamada
- A segunda presença do dia 5 consiste na apresentação do projeto final

