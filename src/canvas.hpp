/**
 * @file canvas.hpp
 * @brief Implementação da classe que descreve a tela do programa
 * @author Heitor Campos() 
 * @author Arthur Skeete()
 * @author Gabriel Carvalho()
 * @details Esta classe guarda as informações da imagem e permite a exportação da mesma.
 * @date 25/05/2026()
 */

#ifndef CANVAS_HPP
#define CANVAS_HPP

#include "common.hpp"
#include "lodepng/lodepng.h"
#include "export_img.hpp"
#include <string>
#include <vector>


namespace pet {
    enum class ImgType { //> Tipo da imagem exportada
            PPM3=0,
            PNG,
        };

            /**
              * @class Canvas
              * @brief Classe responsável por guardar as informações principais sobre a imagem 
              * e exportá-la para os tipos PNG e PPM3
              */
    class Canvas {
        private:

        /* atributos */

        public:
        
            /**
              * @brief Construtor parametrizado
              * @param w Largura da imagem.
              * @param h Altura da imagem.
              * @param fn Nome do arquivo de exportação.
              *
              */

            /* TODO
            constructors
            destructors
            getters
            */

            /**
              * @brief Função que desenha um pixel na imagem
              * @param pixel Posição do pixel desenhado.
              * @param color Nova cor do pixel.
              */
        void add(const Pixel& pixel, const RGBColor& color){
            /*TODO*/
            /*Dica: Pixel é apenas um vetor de 2 posições (x,y)*/
	    };
        
            /**
              * @brief Função que exporta a imagem armazenada
              */
        bool export_img() const {
            /*TODO*/   
        };
	};
};

#endif
