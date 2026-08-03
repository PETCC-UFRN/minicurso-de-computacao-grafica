/**
 * @file export_img.hpp
 * @brief Implementação das funções que exportam a imagem armazenada em Canvas
 * @author Heitor Campos() 
 * @author Arthur Skeete()
 * @author Gabriel Carvalho()
 * @details Esta classe implementa as funções que exportam as imagens armazenadas na classe Canvas para os formatos
 * PPM3 e PNG
 * @date 25/05/2026()
 */

#ifndef EXPORT_IMG_HPP
#define EXPORT_IMG_HPP

#include "common.hpp"
#include "lodepng/lodepng.h"
#include <cstdint>
#include <vector>
#include <string>

namespace pet {

    /**
      * @class ExportImg
      * @brief Classe responsável pela implementação das funções que exportam a imagem armazenada em Canvas
      */
    class ExportImg {
        private:
        /**
          * @brief Função auxiliar que aplica a correção gamma em um determinado valor.
          * @param value Valor a ser corrigido.
          * @param gamma Fator de correção gamma.
          *
          */
        static double apply_gamma(const double value, const double gamma = 2.2) {
            if (value <= 0.0) return 0.0;
            if (value >= 1.0) return 1.0;
            return std::pow(value, 1.0 / gamma);
        }
        public:

        /**
          * @brief Função que exporta a imagem armazenada em Canvas para o formato PNG
          * @param filename Nome do arquivo gerado
          * @param buffer Buffer que armazena as cores dos pixels.
          * @param img_width Largura da imagem.
          * @param img_height Altura da imagem.
          * @param gamma_correction Flag de correção gamma.
          *
          */
        static bool exportPng(const std::string& filename,
                              const std::vector<RGBColor>& buffer,
                              const int& img_width,
                              const int& img_height,
                              const bool& gamma_correction) {

            vector<uint64_t> img;
            img.reserve(img_width * img_height * 4);

            for (auto &color : buffer) {
                double r = std::clamp(color./*TODO*/, 0.0, 1.0);
                double g = std::clamp(color./*TODO*/, 0.0, 1.0);
                double b = std::clamp(color./*TODO*/, 0.0, 1.0);
                
                if(gamma_correction){
                r = apply_gamma(r);
                g = apply_gamma(g);
                b = apply_gamma(b);
                }

                img.push_back(static_cast<uint64_t>(r * 255.0));
                img.push_back(static_cast<uint64_t>(g * 255.0));
                img.push_back(static_cast<uint64_t>(b * 255.0));
                img.push_back(255); // blk = 255.
            }

            vector<unsigned char> chars(img.begin(), img.end());

            unsigned error = lodepng::encode(filename, chars, img_width, img_height);
            if (error) {
                std::cout << "encoder error " << error << ": " << lodepng_error_text(error)
                        << "\n";
                return false;
            } else {
                std::cout << "image generated in: " << filename << "\n";
            }
            return true; // STUB
        }
        
        /**
          * @brief Função que exporta a imagem armazenada em Canvas para o formato PPM3
          * @param filename Nome do arquivo gerado
          * @param buffer Buffer que armazena as cores dos pixels.
          * @param img_width Largura da imagem.
          * @param img_height Altura da imagem.
          * @param gamma_correction Flag de correção gamma.
          *
          */
        static bool exportPPM3(const std::string& filename,
                              const std::vector<RGBColor>& buffer,
                              const int& img_width,
                              const int& img_height,
                              const bool& gamma_correction){

            std::ofstream ofs(filename);

            if (!ofs.is_open()) {
                std::cerr << "Error on create the file " + filename << '\n';
                return false;
            }
            ofs << "P3\n" << img_width << ' ' << img_height << "\n255\n";

            /*
            TODO
            */

            ofs.close();
            return true; // STUB
        }
    };

};

#endif