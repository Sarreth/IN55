#include "Coord3D.h"

Coord3D::Coord3D()
{
        X = 0;
        Y = 0;
        Z = 0;
}

Coord3D::Coord3D(float x,float y,float z)
{
        X = x;
        Y = y;
        Z = z;
}

Coord3D::Coord3D(const Coord3D & v)
{
        X = v.X;
        Y = v.Y;
        Z = v.Z;
}

Coord3D::Coord3D(const Coord3D & from,const Coord3D & to)
{
        X = to.X - from.X;
        Y = to.Y - from.Y;
        Z = to.Z - from.Z;
}

Coord3D & Coord3D::operator= (const Coord3D & v)
{
        X = v.X;
        Y = v.Y;
        Z = v.Z;
        return *this;
}

Coord3D & Coord3D::operator+= (const Coord3D & v)
{
        X += v.X;
        Y += v.Y;
        Z += v.Z;
        return *this;
}

Coord3D Coord3D::operator+ (const Coord3D &v) const
{
        Coord3D t = *this;
        t += v;
        return t;
}

Coord3D & Coord3D::operator+ (const float a)
{
        X += a;
        Y += a;
        Z += a;
        return *this;
}

Coord3D & Coord3D::operator-= (const Coord3D & v)
{
        X -= v.X;
        Y -= v.Y;
        Z -= v.Z;
        return *this;
}

Coord3D Coord3D::operator- (const Coord3D & v) const
{
        Coord3D t = *this;
        t -= v;
        return t;
}

Coord3D & Coord3D::operator- (const float a)
{
        X -= a;
        Y -= a;
        Z -= a;
        return *this;
}


Coord3D & Coord3D::operator*= (const float a)
{
        X *= a;
        Y *= a;
        Z *= a;
        return *this;
}

Coord3D Coord3D::operator* (const float a)const
{
        Coord3D t = *this;
        t *= a;
        return t;
}

Coord3D operator* (const float a,const Coord3D & v)
{
        return Coord3D(v.X*a,v.Y*a,v.Z*a);
}

Coord3D & Coord3D::operator/= (const float a)
{
        X /= a;
        Y /= a;
        Z /= a;
        return *this;
}

Coord3D Coord3D::operator/ (const float a)const
{
        Coord3D t = *this;
        t /= a;
        return t;
}

Coord3D Coord3D::crossProduct(const Coord3D & vect)const //produit vectoriel
{
        Coord3D t;
        t.X = Y*vect.Z - Z*vect.Y;
        t.Y = Z*vect.X - X*vect.Z;
        t.Z = X*vect.Y - Y*vect.X;
        return t;
}

float Coord3D::distanceDroiteAuPoint(const Coord3D & pointDeLaDroite, const Coord3D & point)const //calcul la distance entre la droite (defini par un vecteur directeur et un pointDeLaDroite) et le point
{
        Coord3D prod = crossProduct(Coord3D(pointDeLaDroite, point));
        prod = prod-prod*2;

        return prod.length()/length();
}

float Coord3D::length()const
{
        return sqrt( X*X + Y*Y + Z*Z);
}

Coord3D & Coord3D::normalize()
{
        (*this) /= length();
        return (*this);
}
