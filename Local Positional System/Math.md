***LPS (Local Positional System)***

---

**Part 1 (2D):**

- The system uses cosine theorem to calculate the location of C point

A is the first reference point

B is the second reference point

C is the target point

x is the angle of A point

Cx: position of C point on the x-axis

Cy: position of C point on the y-axis

---

**The calculation formulas:**

[AC] = a

[AB] = b

[BC] = c

x = ArcCos($\frac{c²-a²-b²}{-2ab}$)

Cx = a*cos(x)

Cy = a*sin(x)

---

```Python
import math

a = 5
b = 3
c = 4

Ax, Ay = (0, 0)

x = math.acos((c**2-a**2-b**2)/(-2*a*b))

Cx = Ax+(a*math.cos(x))

Cy = Ay+(a*math.sin(x))

print(f"({Cx},{Cy})")

```
