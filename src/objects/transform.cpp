#include "transform.hpp"
#include "common.hpp"


namespace pet {
    Transform::Transform() : mat() {}                   //< Construtor padrão (obs: a matriz já vem como identidade)
    Transform::Transform(const Mat3& mat) : mat(mat) {} //< Construtor parametrizado
    
    void Transform::rotate(double degrees, Point2 axis)
    {

        double radians = degrees * (M_PI/180.); //< Conversão do ângulo para radianos (utilize ele para as funções trigonométricas)

        Mat3 rotMat; //< Matriz de rotação 
    
        /*TODO: Definir a matriz rotMat*/

        Mat3 toOrigin = Mat3(); //< Matriz de translação do 'ponto' para o 'axis'   

        /*TODO: Definir a matriz toOrigin*/

        Mat3 fromOrigin = Mat3();//< Matriz de translação do 'axis' de volta para o 'ponto'

        /*TODO: Definir a matriz fromOrigin*/
        
        Mat3 pivot /* = TODO: Compor as matrizes. >CUIDADO COM A ORDEM<*/;  //< Composição entre as 3 transformações
        this->mat = mat * pivot;
    }

    // Essa função vai mover um ponto (x, y) para (x + delta.x, y + delta.y). 
    //
    // Caso queira um desafio, recomendo fazer o seguinte: 
    // 1. Receber como parâmetro apenas umum "Point2 reference".
    // 2. Fazer a matriz ir do ponto original para a "reference".
    // Parece a mesma coisa, mas é um pouco diferente, já que na função atual fazemos o ponto mover por um delta, mas a ideia é fazer o ponto mover PARA um lugar
    //
    // Ex: 
    // Point2 ponto_para_transformar; < ponto(x1, y1)
    // Point2 ponto_de_referencia;    < ponto (x2, y2)
    // A translate atual faz o ponto_para_transformar final ser (x1 + x2, y1 + y2)
    // A translate do desafio faz o ponto_para_transformar final ser (x2, y2)
    //
    void Transform::translate(Point2 delta)
    {

        Mat3 transMat = Mat3(); //< Matriz de translação até delta.
        
        /*TODO: Definir a matriz transMat*/

        this->mat = mat * transMat;
    }

    void Transform::scale(double deltax, double deltay, Point2 axis)
    {
        Mat3 scaleMat = Mat3();//< Adicionar o fator de 's' para a escala

        /*TODO: Definir a matriz scaleMat*/

        Mat3 toOrigin = Mat3(); //< Matriz de translação do 'ponto' para o 'axis'   

        /*TODO: Definir a matriz toOrigin*/

        Mat3 fromOrigin = Mat3();//< Matriz de translação do 'axis' de volta para o 'ponto'

        /*TODO: Definir a matriz fromOrigin*/        //< Composição entre as 3 transform

        Mat3 pivot /* = TODO: Compor as matrizes. >CUIDADO COM A ORDEM<*/;  //< Composição entre as 3 transformações
        this->mat = mat * pivot;
    }
    
    /// EXTRA
    void Transform::shear(double s, Point2 axis)
    {
        Mat3 shearMat = Mat3();//< Adicionar o fator de 's' para o cisalhamento

        /*TODO: Definir a matriz shearMat*/

        Mat3 toOrigin = Mat3(); //< Matriz de translação do 'ponto' para o 'axis'   

        /*TODO: Definir a matriz toOrigin*/

        Mat3 fromOrigin = Mat3();//< Matriz de translação do 'axis' de volta para o 'ponto'

        /*TODO: Definir a matriz fromOrigin*/
    
        Mat3 pivot /* = TODO: Compor as matrizes. >CUIDADO COM A ORDEM<*/;  //< Composição entre as 3 transformações

        this->mat = mat * pivot;
    }

    /// EXTRA
    void Transform::reflectX()
    {
        Mat3 reflectMat = Mat3();

        /*TODO: Adicionar o fator de reflexão pelo eixo x na matriz de reflexão*/

        this->mat = mat * reflectMat;
    }

    /// EXTRA
    void Transform::reflectY()
    {
        Mat3 reflectMat = Mat3();

        /*TODO: Adicionar o fator de reflexão pelo eixo Y na matriz de reflexão*/

        this->mat = mat * reflectMat;
    }

    /// EXTRA
    void Transform::reflect(Point2 axis)
    {
        Mat3 reflectMat = Mat3();

        /*TODO: Adicionar o fator de reflexão pelo eixo axis na matriz de reflexão*/
        /*TODO: Transladar para a origem*/
        /*TODO: Transladar para a posição original*/

        this->mat = mat * reflectMat;
    }

    Point2 Transform::operator*(const Point2& p) const
    {
        Point3 point; //< Ponto 'p' representado por coordenadas homogêneas
        
        /*TODO: Colocar o valor de 'point'*/
        /*TODO: Aplicar a matriz no ponto 'point'*/
        
        Point2 finalP; //< Ponto 'p' após a transformação
        /*TODO: Transformar o Ponto 'point' de volta para o tipo Point2*/
        return finalP;
    }

    Transform Transform::operator*(const Transform& t) const
    {
        return Transform(/*TODO: Matrizes 'mat' e 't.mat' compostas*/);
    }

}