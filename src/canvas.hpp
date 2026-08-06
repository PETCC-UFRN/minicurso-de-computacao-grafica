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

        std::string filename; //< Nome do Arquivo que será criado
        std::vector<RGBColor> buffer; //< Buffer de pixels 
        int img_width; //< Largura da imagem gerada
        int img_height; //< Altura da imagem gerada
        ImgType type; //< Tipo da imagem gerada (PPM3 ou PNG)
        bool gamma_correction; //< Flag para aplicar a correção gamma na cor da imagem

        public:
        
            /**
              * @brief Construtor parametrizado
              * @param w Largura da imagem.
              * @param h Altura da imagem.
              * @param fn Nome do arquivo de exportação.
              * @param gc Flag da correção gamma.
              * @param imgType Tipo da imagem exportada.
              *
              */
        Canvas(const int &w, const int &h, const std::string &fn, bool gc = false, ImgType imgType = ImgType::PPM3 ) : filename(fn), img_width(w), img_height(h), type(imgType), gamma_correction(gc)  {
            buffer.resize(w * h);
        }

            /**
              * @brief Função que desenha um pixel na imagem
              * @param pixel Posição do pixel desenhado.
              * @param color Nova cor do pixel.
              */
        void add(const Pixel& pixel, const RGBColor& color) {
            if(pixel.x() < 0 || pixel.x() >= img_width || pixel.y() < 0 || pixel.y() >= img_height)return;
            buffer[pixel.y() * img_width + pixel.x()] = color;
        };
        
            /**
              * @brief Função que exporta a imagem armazenada
              * @details Utilizando o parâmetro imgType passado pelo construtor, ela define qual o formato da imagem.
              */
        bool export_img() const {

            if(type == ImgType::PNG){
                return ExportImg::exportPng(filename, buffer, img_width, img_height, gamma_correction);
            }
            else{
                return ExportImg::exportPPM3(filename, buffer, img_width, img_height, gamma_correction);
            }
                
        };

        int width() const {return img_width;}  //> Getter da largura da imagem.
        int height() const {return img_height;}//> Getter da altura da imagem.
        const std::vector<RGBColor>& data() const {return buffer;}//> Getter da image;

	  std::optional<RGBColor> pixel(Pixel p) {//> Getter do pixel;
		if (p.x() < 0 || p.x() >= img_width || p.y() < 0 || p.y() >= img_height) {
		  return std::nullopt;
		}
		return buffer[p.y() * img_width + p.x()];
	  };
		};
	};

#endif
