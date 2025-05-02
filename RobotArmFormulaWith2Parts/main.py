import math


#Position
cx = 7
cy = 7
ch = round(math.sqrt(cx**2+cy**2),5)

#Arm
h1 = 5
h2 = 5

#Angles
if cx != 0:
    theta  = round(math.acos(( (ch*ch) - (h1*h1) - (h2*h2))/(-2*h1*h2)),4)
    
    k = ch/math.sin(theta)

    a2 = math.asin((h2*math.sin(theta))/ch)
    a3 = (math.pi/2) - (math.atan(cy/cx))


    if cx < 0:
        alpha = -(math.pi/2) + (a2 + a3)
        beta  = -(math.pi)   + (theta)
    else:
        alpha = (math.pi/2) - (a2 + a3)
        beta  = (math.pi)   - (theta)

    print(round(math.degrees(alpha)),round(math.degrees(beta)))
    print(math.degrees(theta),math.degrees(a2),math.degrees(a3))
