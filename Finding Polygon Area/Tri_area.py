####findung area of a triangle####
import math
import curio
def Tri_area(a,b,c):
    s=float(a+b+c)/2
    area=(s*(s-a)*(s-b)*(s-c))**0.5
    return area


##intersection of two lines
def is_intersect(va,vb):
    p1=va[0][0]
    p2=va[0][1]
    p3=va[1][0]
    p4=va[1][1]
    s1=vb[0][0]
    s2=vb[0][1]
    s3=vb[1][0]
    s4=vb[1][1]
    v1=curio.Vector2D(curio.Point2D(p1,p2),curio.Point2D(p3,p4))
    v2=curio.Vector2D(curio.Point2D(s1,s2),curio.Point2D(s3,s4))
    if (v1,v2.intersect):
        d=1
    else:
        d=0
    return d

##for calculatig angle
def get_angle(va,vb):
    p1=va[0][0]
    p2=va[0][1]
    p3=va[1][0]
    p4=va[1][1]
    s1=vb[0][0]
    s2=vb[0][1]
    s3=vb[1][0]
    s4=vb[1][1]
    v1=curio.Vector2D(curio.Point2D(p1,p2),curio.Point2D(p3,p4))
    v2=curio.Vector2D(curio.Point2D(s1,s2),curio.Point2D(s3,s4))
    ###mul=[a*b for a,b in zip(v1,v2)]
    a=v1*v2
    theta=math.acos(a/(v1.length*v2.length))
    theta=theta*(180/3.14)
    return theta
