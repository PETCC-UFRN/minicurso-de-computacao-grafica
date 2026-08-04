/**
 * @file vec2.hpp
 * @brief Implementação da classe de vetores de 2 dimensões.
 * @author Heitor Campos()
 * @details Esta classe contém as definições e operações matemáticas para vetores 2D,
 *          servindo tanto para posições cartesianas quanto para o cálculo de vetores.
 * @see https://raytracing.github.io/ 
 * @par Créditos:
 *      A lógica fundamental, sobrecarga de operadores e design da classe `vec2` 
 *      são baseados nos ensinamentos de Peter Shirley no seu livro 
 *      "Ray Tracing in One Weekend".
 * @date 15/04/2026()
 */

#pragma once

#ifndef SSUGARY_MATH_VECTOR_2_HPP
#define SSUGARY_MATH_VECTOR_2_HPP

#include <cmath>
#include <cstdint>
#include <fstream>

using std::size_t;

/**
  *
  * @class vec2
  * @brief Representa um vetor matemático de 2 componentes.
  * @tparam T O tipo de dado numérico armazenado.
  * @details Esta classe é fundamental para cálculos geométrico no espaço.
  */
template<typename T>
    class vec2 {  
        private:
            T e[2];
        public:
            /**
              * @brief Construtor padrão. Inicializa como um vetor nulo.
              * @details Utiliza static_cast<T> para garantir compatibilidade com qualquer 
              *          tipo numérico, evitando avisos de conversão implícita.
              */
            vec2(){e[0] = static_cast<T>(0); e[1] = static_cast<T>(0);};
            /**
              * @brief Construtor parametrizado.
              * @param e0 Valor da componente X .
              * @param e1 Valor da componente Y .
              */
            
            constexpr vec2(T e0, T e1) : e{e0, e1}{};
            constexpr vec2(const vec2<T>& vec) : e{vec[0], vec[1]} {};

            /* Acesso de Coordenadas Cartesianas */
            constexpr T x() const {return e[0];};
            constexpr T y() const {return e[1];};


            /** @brief Retorna o próprio vetor */
            constexpr const vec2<T>& operator+() const {return *this;};

            /** @brief Retorna o vetor invertido */
            constexpr vec2<T> operator-() const {return vec2(-e[0], -e[1]);};

            /**
              * @brief Operador desigualdade
              * @param rhs Representa o outro elemento, right hand side
              * @return Retorna se um ponto está mais a esquerda do que outro
              */
			bool operator<(const vec2& rhs) const {
				if (this->x() != rhs.x())
					return this->x() < rhs.x();
				return this->y() < rhs.y();
			}

            /**
              * @brief Operador de acesso constante aos elementos do vetor.
              * @param index Representa a posição acessada.
              * @return Retorna o elemento que está na posição indicada pelo parâmetro.
              */
            constexpr T operator[](size_t index) const {return e[index];};

            /**
              * @brief Operador de acesso aos elementos do vetor.
              * @param index Representa a posição acessada.
              * @return Retorna o elemento que está na posição indicada pelo parâmetro.
              */
            constexpr T& operator[](size_t index){return e[index];};

            /**
              * @brief Soma dois vetores termo a termo.
              * @param v2 O vetor do lado direito da soma
              * @return vec2<T> Novo vetor resultante.
              */
            constexpr vec2<T> operator+(const vec2<T> &v2) const {return vec2<T>(v2.e[0] + e[0], v2.e[1] + e[1]);};

            /**
              * @brief Subtrai dois vetores termo a termo.
              * @param v2 O vetor do lado direito da subtração
              * @return vec2<T> Novo vetor resultante.
              */
            constexpr vec2<T> operator-(const vec2<T> &v2) const {return vec2<T>(e[0] - v2.e[0], e[1] - v2.e[1]);};

            /**
              * @brief Multiplica um vetor por um escalar t.
              * @param t Escalar que está multiplicando o vetor.
              * @return vec2<T> Novo vetor resultante.
              */
            constexpr vec2<T> operator*(const T t) const {return vec2<T>(e[0] * t, e[1] * t);};

            constexpr vec2<T> operator*(const vec2<T> &v2)const{return vec2<T>(e[0] * v2[0], e[1] * v2[1]);};
            /**
              * @brief Divide um vetor por um escalar t.
              * @param t Escalar que está dividindo o vetor.
              * @return vec2<T> Novo vetor resultante.
              */
            constexpr vec2<T> operator/(const T t) const {return vec2<T>(e[0]/t, e[1]/t);};

            /**
              * @brief Calcula o Produto Escalar entre este vetor 2D e outro.
              * @param v2 O vetor secundário.
              * @return O valor do produto escalar.
              */
            constexpr T dot(vec2<T> v2) const {
                return e[0] * v2.e[0] + e[1] * v2.e[1];
            }

            /**
              * @brief Calcula o Produto Vetorial entre este vetor 2D e outro.
              * @param v2 O vetor secundário.
              * @return O valor da Z-Coordenada do Vetor perpendicular
              */
            constexpr T cross(vec2<T> &v2) const {
                return dynamic_cast<T>(e[0] * v2.e[1] - v2.e[0] * e[1]);
            }

            /**
              * @brief Iguala os valores de um vetor a outro.
              * @param v2 Vetor que envia os valores que serão copiados.
              * @return vec2<T> O vetor atual após se igualar ao segundo vetor.
              */
            constexpr vec2<T>& operator=(const vec2<T> &v2){
                e[0] = v2.e[0];
                e[1] = v2.e[1];
                return *this;
            }

            /**
              * @brief Soma os valores de um vetor a outro.
              * @param v2 Vetor que envia os valores que serão somados.
              * @return vec2<T> O vetor atual após a soma do segundo vetor.
              */
            constexpr vec2<T>& operator+=(const vec2<T> &v2){
                e[0] += v2.e[0];
                e[1] += v2.e[1];
                return *this;
            };

            /**
              * @brief Subtrai os valores de um vetor a outro.
              * @param v2 Vetor que envia os valores que serão subtraídos.
              * @return vec2<T> O vetor atual após a subtração do segundo vetor.
              */
            constexpr vec2<T>& operator-=(const vec2<T> &v2){
                e[0] -= v2.e[0];
                e[1] -= v2.e[1];
                return *this;
            };

            /**
              * @brief Multiplica o vetor com um escalar.
              * @param t Escalar que irá multiplicar o vetor.
              * @return vec2<T> O vetor atual após a multiplicação.
              */
            constexpr vec2<T>& operator*=(const float t){
                e[0] *= t;
                e[1] *= t;
                return *this;
            };

             /**
              * @brief Divide o vetor com um escalar.
              * @param t Escalar que irá dividir o vetor.
              * @return vec2<T> O vetor atual após a divisão.
              */
            constexpr vec2<T>& operator/=(const float t){
                e[0] /= t;
                e[1] /= t;
                return *this;
            };



            /** @brief Retorna o comprimento (magnitude) real do vetor 2D. */
            inline T length() const {return std::sqrt(e[0] * e[0] + e[1] * e[1]);};

            /** @brief Retorna o comprimento ao quadrado do vetor 2D. */
            constexpr T sqr_length() const {return e[0] * e[0] + e[1] * e[1];};

            /** @brief Normaliza o próprio vetor in-place*/
            inline void mk_unit_vec(){
                float k = 1.0 / this->length();
                e[0] *= k; e[1] *= k;
            }

            
        };
        /** @brief Retorna uma cópia normalizada de um vetor 2D fornecido. */
            template<typename T>
            inline vec2<T> unit_vec(vec2<T> v){return v / v.length();};
            /**
              * @brief Multiplica um vetor por um escalar t.
              * @param t Escalar que está multiplicando o vetor.
              * @param v Vetor que está sendo multiplicado.
              * @return vec2<T> Novo vetor resultante.
              */
            template<typename T>
            constexpr vec2<T> operator*(const T t, const vec2<T> &v) {
                return v * t; 
            }

            /** @brief Permite a leitura do vetor a partir de streams de input. */
            template<typename T>
            inline std::istream& operator>>(std::istream &is, vec2<T> &v){
                is >> v[0] >> v[1];
                return is;
            }

            /** @brief Permite a impressão do vetor em streams de output. */
            template<typename T>
            inline std::ostream& operator<<(std::ostream &os, const vec2<T> &v){
                os << v[0] << ' ' << v[1];
                return os;
            }

            /**
              * @brief Calcula o Produto Vetorial entre dois vetores 2D.
              * @param v1 O primeiro vetor.
              * @param v2 O segundo vetor.
              * @return O valor da Z coordenada do vetor ortogonal a v1 e v2
              */
            template<typename T>
            constexpr T cross(const vec2<T> &v1, const vec2<T> &v2){
                return dynamic_cast<T>(v1[0] * v2[1] - v1[1] * v2[0]);
            }

            /**
              * @brief Calcula o Produto Escalar dois vetores 2D.
              * @param v1 O primeiro vetor
              * @param v2 O segundo vetor.
              * @return O valor do produto escalar entre v1 e v2.
              */
            template <typename T>
            constexpr T dot(vec2<T> v1, vec2<T> v2){
                return v1[0] * v2[0] + v1[1] * v2[1];
            }
#endif // SSUGARY_MATH_VECTOR_2_HPP
