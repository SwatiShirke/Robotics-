####finding area of convex polygon###
import math
import curio
import Tri_area
poly_verts=[(5.1,-1.64),
            (11.7,-0.22),
            (13.82,2),
            (12.52,3.56),
            (10.12,5.22),
            (7.48,4.64),
            (4.14,3.48)]
c=len(poly_verts)
d=c-2
poly_area=0
k=1

for j  in range(0,d):
    Triangle_verts=[poly_verts[0],poly_verts[k],poly_verts[k+1]]
    x_point=range(0,3)
    y_point=range(0,3)
    v2=range(0,3)
    v2_len=range(0,3)
    Triangle_area=range(0,d)
   
##extracting x,y from vector of points
    for i in range(0,3):
        x_point[i]=Triangle_verts[i][0]
        y_point[i]=Triangle_verts[i][1]
    
    for i in range(0,2):   
        v2[i]=curio.Vector2D(curio.Point2D(x_point[i],y_point[i]),curio.Point2D(x_point[i+1],y_point[i+1]))
        v2_len[i]=(v2[i].length)
    
    v2[i+1]=curio.Vector2D(curio.Point2D(x_point[0],y_point[0]),curio.Point2D(x_point[2],y_point[2]))
    v2_len[i+1]=v2[i+1].length
    print v2_len
    Triangle_area[j]=Tri_area.Tri_area(v2_len[0],v2_len[1],v2_len[2])
    poly_area+=Triangle_area[j]
    k=k+1

print poly_area
