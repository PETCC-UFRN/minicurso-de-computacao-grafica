#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "common.hpp"

namespace pet {
    /**
      * @class Transform
      * @brief Classe que aplica o conceito de Transformação Geométrica em objetos que extendem a classe Object
      *
      */
    class Transform {
        public:
            Mat3 mat; //< Matriz de transformação
            Transform();                           //< Construtor Padrão
            Transform(const Transform&) = default; //< Construtor de Cópia
            Transform(const Mat3&);                //< Construtor Parametrizado

            /**
              * @brief Função que adiciona o fator rotação na matriz de transformação
              * @param angle Ângulo de rotação
              * @param axis Eixo de rotação
              */
            void rotate(double angle, Point2 axis);

            /**
              * @brief Função que adiciona o fator translação na matriz de transformação
              * @param delta Valor que será adicionado na translação do objeto
              */
            void translate(Point2 delta);

            /**
              * @brief Função que adiciona o fator Escala na matriz de transformação
              * @param deltax Variação do valor de X
              * @param deltay Variação do valor de Y
              * @param axis Eixo de escala do objeto
              */
            void scale(double deltax, double deltay, Point2 axis);
            
            /**
              * @brief Função que adiciona o fator de Cisalhamento na matriz de transformação
              * @param s Variação do cisalhamento
              * @param axis Eixo de escala do objeto
              * @details Essa função não precisa ser implementada, é um >EXTRA<
              */
            void shear(double s, Point2 axis);

            /**
              * @brief Função que adiciona o fator Reflexão no eixo X na matriz de transformação
              * @details Essa função não precisa ser implementada, é um >EXTRA<
              */
            void reflectX();

            /**
              * @brief Função que adiciona o fator Reflexão no eixo Y na matriz de transformação
              * @details Essa função não precisa ser implementada, é um >EXTRA<
              */
            void reflectY();

            /**
              * @brief Função que adiciona o fator Reflexão com base em um eixo na matriz de transformação
              * @param axis Eixo de reflexão do objeto
              *
              * @details Essa função não precisa ser implementada, é um >EXTRA<
              */
            void reflect(Point2 axis);

            /**
              * @brief Operador que aplica a transformação em um ponto
              * @param point Ponto a ser Transformado
              */
            Point2 operator*(const Point2& point) const;

            /**
              * @brief Operador que compõe duas transformações
              * @param t Transformação que será composta com a atual
              */
            Transform operator*(const Transform& t) const;
    };
};


#endif