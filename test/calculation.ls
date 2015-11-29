/* Copyright (c) 2015, Alister Chan. All rights reserved.
 * See the README file for license conditions and contact info.
 */
//Test statements for calculations
include `math.ls`;

func main: int8() {

    q: char = '1';
    r: char = 1;
    
    s: bool = `false`;
    t: bool = true;
    
    u: int8 = `u123`;
    v: int8 = 'v';
    w: int8 = 128;
    x: int8 = 127;
    y: int8 = 2;
    z: int8;
    
    z = x + y;
    
    return z;
}