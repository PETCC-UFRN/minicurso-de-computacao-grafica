/**
 * @file mat2.hpp
 * @brief Implementação da classe de matrizes de 2x2 dimensões.
 * @author Heitor Campos()
 * @details Esta classe contém as definições e operações matemáticas para matrizes 2x2,
 *          servindo tanto para posições cartesianas quanto para cálculos de espaços vetoriais.
 * @date 15/04/2026()
 */

#pragma once

#ifndef SSUGARY_MATH_MATRIX_2X2_HPP
#define SSUGARY_MATH_MATRIX_2X2_HPP
 
#include <algorithm>
#include <array>
#include <cstdint>
#include <initializer_list>
#include <utility>

#include "vec2.hpp"

using size_t = std::size_t;
#ifndef MATRIX_LAYOUT
#define MATRIX_LAYOUT
/**
 * @brief Define a forma como a matriz é populada a partir de vetores.
 */
enum MatrixLayout{
    ROWS=0, ///< Os vetores fornecidos formam as linhas da matriz
    COLUMNS ///< Os vetores fornecidos formam as colunas da matriz.
};
#endif
template<typename T>
class mat2 {
        private:
            std::array<T, 4> mat;
        public:
            /**
              * @brief Construtor padrão que inicializa uma Matriz Identidade 2X2.
              * @details Utiliza static_cast<T> para garantir compatibilidade com qualquer 
              *          tipo numérico, evitando avisos de conversão implícita (narrowing).
              */
            constexpr mat2() {
                T zero = static_cast<T>(0);
                T one  = static_cast<T>(1);

                mat = {
                    one,  zero,
                    zero, one,  
                };
            };

            /**
              * @brief Constrói uma matriz 2X2 a partir de elementos individuais.
              * @details Os elementos seguem a ordem de linha (Row-Major), onde o primeiro 
              *          dígito indica a linha e o segundo a coluna (ex: e12 é linha 1, coluna 2).
              */
            constexpr mat2(T e00, T e01, 
                           T e10, T e11) 
                           : mat{e00, e01, e10, e11}{}

            /**
              * @brief Constrói uma matriz 2X2 a partir de três vetores vec2.
              * @param v1 O primeiro vetor (linha 0 ou coluna 0).
              * @param v2 O segundo vetor  (linha 1 ou coluna 1).
              * @param flag Define se os vetores representam linhas (ROWS) ou colunas (COLUMNS). Padrão é ROWS.
              */
            constexpr mat2(const vec2<T>& v1, const vec2<T>& v2, MatrixLayout flag = ROWS){
                switch(flag){
                    case ROWS:
                        mat[0] = v1[0]; mat[1] = v1[1];
                        mat[2] = v2[0]; mat[3] = v2[1];
                    break;
                    case COLUMNS:
                        mat[0] = v1[0]; mat[1] = v2[0]; 
                        mat[2] = v1[1]; mat[3] = v2[1]; 
                    break;
                }
            }
            /**
              * @brief Constrói uma matriz 2X2 a partir de outra matriz 2X2.
              * @param m2 Matriz que servirá de base para a construção.
              */
            constexpr mat2(const mat2 &m2){mat = m2.mat;};

            /**
              * @brief Constrói uma matriz 2X2 a partir de uma initializer list.
              * @param list Lista genérica que irá construir a matriz.
              * @details Os elementos seguem a ordem de linha (Row-Major), onde o primeiro 
              *          dígito indica a linha e o segundo a coluna (ex: e12 é linha 1, coluna 2).
              */
            constexpr mat2(std::initializer_list<T> list){
                mat.fill(static_cast<T>(0)); 
    
                size_t count = std::min(static_cast<size_t>(4), list.size());
                std::copy(list.begin(), list.begin() + count, mat.begin());
            
            };


            /** @brief Retorna a própria matriz. */
            inline const mat2& operator+() const {return *this;};

            /** @brief Retorna a matriz invertida. */
            inline mat2 operator-() const {return mat2( -mat[0], -mat[1], 
                                                        -mat[2],-mat[3]);};
            /**
              * @brief Operador de acesso aos elementos da matriz
              * @param row Representa a linha que será acessada
              * @param col Representa a coluna que será acessaad
              * @return Retorna o elemento que está na posição indicada pelos parâmetros 
              *         (ex: (0, 1) retorna o elemento e01) 
              */
            constexpr T& operator()(size_t row, size_t col){return mat[row * 2 + col];}

            /**
              * @brief Operador de acesso constante aos elementos da matriz
              * @param row Representa a linha que será acessada
              * @param col Representa a coluna que será acessaad
              * @return Retorna o elemento de que está na posição indicada pelos parâmetros 
              *         (ex: (0, 1) retorna o elemento e01) 
              */
            constexpr T  operator()(size_t row, size_t col) const {return mat[row * 2 + col];}

            /**
              * @brief Soma duas matrizes termo a termo.
              * @param m2 A matriz do lado direito da soma.
              * @return mat2<T> A nova matriz resultante.
              */
            constexpr mat2 operator+(const mat2 &m2) const {return mat2(mat[0] + m2.mat[0], mat[1] + m2.mat[1],
                                                                        mat[2] + m2.mat[2], mat[3] + m2.mat[3]);};
            /**
              * @brief Subtrai duas matrizes termo a termo.
              * @param m2 A matriz do lado direito da subtração.
              * @return mat2<T> A nova matriz resultante.
              */
            constexpr mat2 operator-(const mat2 &m2) const {return mat2(mat[0] - m2.mat[0], mat[1] - m2.mat[1],
                                                                        mat[2] - m2.mat[2], mat[3] - m2.mat[3]);};
            /**
              * @brief Multiplica duas matrizes 2X2.
              * @details Utiliza o método de "linha por coluna". Combina transformações.
              * @param m2 A matriz do lado direito da multiplicação.
              * @return mat2<T> A nova matriz resultante.
              */
            constexpr mat2 operator*(const mat2& m2) const {
                return mat2(
                    mat[0] * m2.mat[0] + mat[1] * m2.mat[2],
                    mat[0] * m2.mat[1] + mat[1] * m2.mat[3],
                    mat[2] * m2.mat[0] + mat[3] * m2.mat[2],
                    mat[2] * m2.mat[1] + mat[3] * m2.mat[3]
                );
            }

            /**
              * @brief Multiplica uma matriz por uma constante.
              * @param t Constante que está multiplicando a matriz.
              * @return mat2<T> A nova matriz resultante.
              */
            constexpr mat2 operator*(const T t ) const {return mat2(mat[0] * t, mat[1] * t,
                                                                    mat[2] * t, mat[3] * t);};

            /**
              * @brief Divide uma matriz por uma constante.
              * @param t Constante que está dividindo a matriz.
              * @return mat2<T> A nova matriz resultante.
              */
            constexpr mat2 operator/(const T t ) const {return mat2(mat[0] / t, mat[1] / t,
                                                                    mat[2] / t, mat[3] / t);};
            
            /**
              * @brief Iguala os valores de uma matriz a outra.
              * @param m2 Matriz que envia os valores que serão copiados.
              * @return mat2<T> A matriz atual após se igualar a segunda matriz.
              */
            constexpr mat2& operator=(const mat2 &m2){
                for(size_t i{0}; i < 4; i++){
                    mat[i] = m2.mat[i];
                }
                return *this;
            };

            /**
              * @brief Soma os valores de uma matriz a outra.
              * @param m2 Matriz que envia os valores que serão somados.
              * @return mat2<T> A matriz atual após a soma da segunda matriz.
              */
            constexpr mat2& operator+=(const mat2 &m2){
                for(size_t i{0}; i < 4; i++){
                    mat[i] += m2.mat[i];
                }
                return *this;
            };

            /**
              * @brief Subtrai os valores de uma matriz a outra.
              * @param m2 Matriz que envia os valores que serão subtraídos.
              * @return mat2<T> A matriz atual após a subtração da segunda matriz.
              */
            constexpr mat2& operator-=(const mat2 &m2){
                for(size_t i{0}; i < 4; i++){
                    mat[i] -= m2.mat[i];
                }
                return *this;
            };

            /**
              * @brief Multiplica duas matrizes 2X2.
              * @details Utiliza o método de "linha por coluna". Combina transformações.
              * @param m2 A matriz do lado direito da multiplicação.
              * @return mat2<T> A nova matriz resultante.
              */
            constexpr mat2& operator*=(const mat2& m2) {
                *this = *this * m2;
                return *this;
            }

            /**
              * @brief Multiplica a matriz com um escalar.
              * @param t Escalar que irá multiplicar a matriz.
              * @return mat2<T> A matriz atual após a multiplicação.
              */
            constexpr mat2& operator*=(const T t){
                for(size_t i{0}; i < 4; i++){
                    mat[i] *= t;
                }
                return *this;
            };
            
            /**
              * @brief Divide a matriz com um escalar.
              * @param t Escalar que irá Dividir a matriz.
              * @return mat2<T> A matriz atual após a multiplicação.
              */
            constexpr mat2& operator /=(const T t){
                for(size_t i{0}; i < 4; i++){
                    mat[i] /= t;
                }
                return *this;
            }

            /**
              * @brief Multiplica a matriz com um vetor de mesma largura.
              * @param v Vetor que irá ser multiplicado pela matriz.
              * @return vec2<T> Vetor obtido após a multiplicação.
              */
            constexpr vec2<T> operator*(const vec2<T>& v) const {
                return vec2<T>(
                    mat[0] * v[0] + mat[1] * v[1],
                    mat[2] * v[0] + mat[3] * v[1]
                );
            }

            /**
              * @brief Transforma a matriz em sua forma transposta.
              * @return mat2<T> Matriz transposta gerada a partir da matriz original.
              */
            inline mat2& transpose(){
                std::swap(mat[1], mat[2]);
                return *this;
            }

            /**
              * @brief Retorna a representação da matriz em vetores de vetores
              * @param flag Define se os vetores representam linhas (ROWS) ou colunas (COLUMNS). Padrão é ROWS.
              * @return vec2<vec2<T>> Representação da matriz original em vetor de vetor.
              */
            constexpr vec2<vec2<T>> toVec(MatrixLayout flag = ROWS) const {
                switch(flag){
                    case ROWS:
                    return vec2<vec2<T>>({mat[0], mat[1]},
                                         {mat[2], mat[3]});
                    case COLUMNS:
                    return vec2<vec2<T>>({mat[0], mat[2]},
                                         {mat[1], mat[3]});
                    }
            };

            /**
              * @brief Retorna a determinante de uma matriz.
              * @return T Valor da determinante da matriz.
              */
            constexpr T det() const {
                return mat[0] * mat[3] - mat[1] * mat[2];
            }

            /**
              * @brief Calcula a matriz inversa de uma 2X2.
              * @details Utiliza a matriz adjunta dividida pelo determinante.
              * @return mat2<T> A matriz inversa. Retorna a identidade se não tiver inversa.
              */
            inline mat2 inverse() const {
                T determinant = det();
                
                if (std::abs(determinant) <= static_cast<T>(1e-8)) {
                    return mat2(); 
                }
                
                T invDet = static_cast<T>(1) / determinant;

                return mat2(
                     mat[3], -mat[1],
                     -mat[2], mat[0]
                ) * invDet;
            }

            /**
              * @brief Multiplica um escalar por uma matriz 2X2.
              * @param t Escalar do lado esquerdo da multiplicação.
              * @param m Matriz do lado direito da multiplicação.
              * @return mat2<T> A nova matriz resultante.
              */
            friend constexpr mat2 operator*(const T t, const mat2& m) {
                return m * t; 
                }
    };

#endif // SSUGARY_MATH_MATRIX_2X2_HPP