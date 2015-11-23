/* Copyright (c) 2015, Alister Chan. All rights reserved.
 * See the README file for license conditions and contact info.
 */
//Test statements for calculations
include `math.ls`;

func main: int8() {

    w: int8 = 128;
    x: int8 = 127;
    y: int8 = 2;
    z: int8;
    
    z = x + y;
    
    return z;
}