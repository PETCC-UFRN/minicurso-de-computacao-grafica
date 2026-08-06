/**
 * @file object.hpp
 * @brief Classh abstrata que descreve um objeto genérico.
 * @author Heitor Campos()
 * @author Arthur Skeete()
 * @author Gabriel Carvalho()
 * @details Esta classe define um objeto genérico que será desenhado na cena
 * @date 25/05/2026()
 */
#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "canvas.hpp"
#include "common.hpp"
#include "transform.hpp"

namespace pet{
			enum class DrawMethod {
				Bresenhan = 0,
        		BresenhanMidpoint,
				XiaolinWu,
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
              * @brief Função que desenha o objetp no Canvas
              * @param canvas Tela que será desenhada.
              * @param color Cor do objeto.
              */
            virtual void drawObject(Canvas& canvas, RGBColor color, DrawMethod method = DrawMethod::Bresenhan) = 0;

            /**
              * @brief Indica se o objeto é uma forma fechada (tem interior preenchível).
              * @details Query separada da ação (fillObject): permite ao chamador
              * validar antes de mutar estado. Default: forma aberta.
              */
            virtual bool hasInterior() const { return false; }

            /**
              * @brief Pinta o interior do objeto no Canvas.
              * @details No-op por padrão; formas fechadas sobrescrevem com o
              * preenchimento geométrico (spans por scanline).
              * @param canvas Tela alvo.
              * @param color Cor de preenchimento.
              */
            virtual void fillObject(Canvas&, const RGBColor&) {}

            /**
              * @brief Função que aplica uma matriz de rotação no objeto
              * @param transformation Transformação que será aplicada
              */
            virtual void transform(const Transform& transformation) = 0;

            /**
              * @brief Função que retorna o eixo de transformação do objeto
              */
            virtual Point2 getAxis() const = 0;
    };
};

#endif
