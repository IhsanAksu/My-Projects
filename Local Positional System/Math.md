***LPS (Local Positional System)***

*Part 1: 2D*

The system uses cosine theorem to calculating the location of C point

A is the first reference point
B is the second reference point
C is the target point

x is the angle of A point

[AC] = a
[AB] = b
[BC] = c

Cx: position of C point on the x-axis
Cy: position of C point on the y-axis

x = ArcCos((c²-a²-b²)/(-2*a*b))


```P
import math

x = math.acos((c²-a²-b²)/(-2*a*b))

Cx = 


```
