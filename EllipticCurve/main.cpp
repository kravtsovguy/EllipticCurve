//
//  main.cpp
//  EllipticCurve
//
//  Created by Matvey Kravtsov on 12.06.16.
//  Copyright © 2016 Matvey Kravtsov. All rights reserved.
//
#include <vector>
#include <iostream>
#define big long long
using namespace std;

big q = 12251;
big A= -3;
big B= 10;
big x = 35;
big y = 1230;

big trim(big a){
    big b = a % q;
    if(b<0) b+=q;
    return b;
}

big pow(big a, big n){
    if (n == 0)
        return 1;
    if (n % 2 == 1)
        return trim(pow (a, n-1) * a);
    else {
        big b = pow (a, n/2);
        return trim(b * b);
    }
}

big obr(big a)
{
    big b = pow(trim(a), q-2);
    return b;
}

struct Point
{
    big x,y;
    Point() // zero
    {
        this->x=0;
        this->y=0;
    }
    Point(big x, big y)
    {
        this->x=x;
        this->y=y;
    }
    
    bool operator==(const Point &b )
    {
        return this->x==b.x && this->y==b.y;
    }
    bool operator!=(const Point &b )
    {
        return !(*this == b);
    }
    
    Point operator+(const Point &b)
    {
        Point a = *this;
        Point r;
        big s = 0;
        if(a == Point())
        {
            return b;
        }
        if((Point)b == Point())
        {
            return a;
        }
        if(trim(a.y+b.y) == 0)
        {
            return Point();
        }
        if(a==b){
            s=trim(3*pow(a.x, 2)+A)*obr(2*a.y);
        }else{
            s=trim(b.y-a.y)*obr(b.x-a.x);
        }
        
        s = trim(s);
        
        r.x = pow(s, 2)-a.x-b.x;
        r.x = trim(r.x);
        
        r.y =-a.y+s*(a.x-r.x);
        r.y = trim(r.y);
        
        return r;
    }
    
    Point operator-(const Point &b)
    {
        Point a = *this;
        Point c = b;
        c.y = trim(-b.y);
        return a+c;
    }
    
    Point operator*(const big c)
    {
        Point a = *this;
        Point b = a;
        for(big i=1; i<abs(c); i++){
            b=b+a;
        }
        return b;
    }
    
    string toStr(){
        string s="";
        if(*this == Point()){
            s="O";
        }else{
            s="( "+to_string(x)+", "+to_string(y)+" )";
        }
        return s;
    }
    
    void print() {
        cout<<toStr()<<endl;
    }

};

Point p(x,y);

Point AliceGeneratePoint(big a){
    
    Point pA = p*a;
    cout<<"--Алиса--"<<endl;
    cout<<"Закрытый ключ Алисы: "<<a<<endl;
    cout<<"Открытый ключ Алисы: "; pA.print();
    return pA;
}

pair<Point, Point> Bob(big k, Point pM, Point pA)
{
    Point pB1 = p*k;
    Point pB2 = pM+pA*k;
    
    cout<<"--Боб--"<<endl;
    cout<<"Число Боба: "<<k<<endl;
    cout<<"Точка пространства Боба: "; pM.print();
    cout<<"Две сгенерированные точки: "<<"B1 = "<<pB1.toStr()<<" & "<<"B2 = "<<pB2.toStr()<<endl;
    return make_pair(pB1, pB2);
}

Point AliceResolvePoint(big a, pair<Point,Point> pB){
    
    Point pM = pB.second-pB.first*a;
    
    cout<<"--Алиса--"<<endl;
    cout<<"Закрытый ключ Алисы: "<<a<<endl;
    cout<<"Считаем: "<<pB.second.toStr()<<" - "<<"["<<to_string(a)<<"]*"<<pB.first.toStr()<<" = "<<pM.toStr()<<endl;
    cout<<"Расшифрованная точка: "; pM.print();
    
    return pM;
    
}

Point AliceGeneratePointZ(big a){
    
    Point pA = p*a;
    cout<<"--Алиса--"<<endl;
    cout<<"Закрытый ключ Алисы: "<<a<<endl;
    cout<<"Открытый ключ Алисы: "; pA.print();
    return pA;
}

pair<Point, pair<big, big>> BobZ(big k, big m1, big m2, Point pA)
{
    Point pB1 = p*k;
    Point pB2 = pA*k;
    Point pM(m1,m2);
    big c1 = pB2.x*m1;
    big c2 = pB2.y*m2;
    
    cout<<"--Боб--"<<endl;
    cout<<"Число Боба: "<<k<<endl;
    cout<<"Точка пространства Боба (m1,m2): "; pM.print();
    cout<<"Две сгенерированные точки: "<<"B1 = "<<pB1.toStr()<<" & "<<"B2 = "<<pB2.toStr()<<endl;
    cout<<"Два сгенерированных числа: "<<"с1 = "<<to_string(c1)<<" & "<<"с2 = "<<to_string(c2)<<endl;
    return make_pair(pB1, make_pair(c1, c2));
}

Point AliceResolvePointZ(big a, pair<Point, pair<big, big>> pB){
    
    Point pC = pB.first*a;
    Point pM(pB.second.first/pC.x,pB.second.second/pC.y);
    
    cout<<"--Алиса--"<<endl;
    cout<<"Закрытый ключ Алисы: "<<a<<endl;
    cout<<"Точка [a]B1: "<<pC.toStr()<<endl;
    cout<<"Считаем: "<<"m1 = " << pB.second.first<<"/"<<pC.x<<" & m2 = "<<pB.second.second<<"/"<<pC.y<<endl;
    cout<<"Расшифрованная точка: "; pM.print();
    
    return pM;
    
}


vector<Point> allPoints;
void findAllPoints() //метод работает долго, поэтому находим только первую точку
{
    for (big x=0; x<q; x++) {
        for (big y=0; y<q; y++) {
            if(pow(y,2) == trim(pow(x,3)+A*x+B)){
                allPoints.push_back(Point(x,y));
                return;
            }
        }
    }
    cout<<"Точек на кривой в заданном поле: "<<allPoints.size()<<endl;
}

void ElGamal(){
    cout<<"ElGamal"<<endl;
    big a = 3;
    big k = 4;
    Point pM = allPoints[0];
    Point pA = AliceGeneratePoint(a);
    cout<<endl;
    pair<Point, Point> pB = Bob(k, pM, pA);
    cout<<endl;
    Point pR = AliceResolvePoint(a, pB);
    cout<<endl;
    
    cout<<"Итог:"<<endl;
    cout<<"Расшифрованная точка: "<<pR.toStr()<<endl;
    cout<<"Точка M, заданная Бобом: "<<pM.toStr();
    cout<<endl;
    if(pR==pM)
    {
        cout<<pR.toStr()<<" == "<<pM.toStr()<<endl;
        cout<<"Точка Боба и расшифрованная точка Алисы совпадает!"<<endl;
    }

}

void Task1(){
    cout<<"Задача 1"<<endl;
    big a = 3;
    big k = 4;
    Point pM(10233, 4632);
    Point pA = AliceGeneratePointZ(a);
    cout<<endl;
    pair<Point, pair<big, big>> pB = BobZ(k, pM.x, pM.y, pA);
    cout<<endl;
    Point pR = AliceResolvePointZ(a, pB);
    cout<<endl;
    
    cout<<"Итог:"<<endl;
    cout<<"Расшифрованное сообщение: "<<pR.toStr()<<endl;
    cout<<"Сообщение (m1,m2), заданное Бобом: "<<pM.toStr();
    cout<<endl;
    if(pR==pM)
    {
        cout<<pR.toStr()<<" == "<<pM.toStr()<<endl;
        cout<<"Сообщение Боба и расшифрованное сообщение Алисы совпадает!"<<endl;
    }
}

int main(int argc, const char * argv[]) {
    
    cout<<"Кривая: "<<"y^2=x^3+("<<A<<")*x+("<<B<<")"<<endl;
    
    cout<<"Проверка на сингулярность: ";
    if(trim(4*A*A*A+27*B*B) != 0){
        cout<<"кривая пригодна (не сингулярная)";
    }else{
        cout<<"кривая не пригодна (сингулярная)";
    }
    cout<<endl;
    cout<<endl;
    
    cout<<"Генератор: ";
    p.print();
    
    Point r = p;
    big i=0;
    do{
        r=r+p;
        i++;
    }while (r != p);
    
    cout<<"Порядок генератора: "<<i<<endl;
    cout<<endl;
    cout<<"Первые 10 точек: "<<endl;
    r=p;
    for(int i=1; i<11; i++){
        cout<<i<<": ";
        r.print();
        r=r+p;
    }
    cout<<endl;
    
    findAllPoints();
    
    cout<<endl;
    
    ElGamal();
    
    cout<<endl;
    
    Task1();
    
    
    
    
    return 0;
}
