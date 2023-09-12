/* REMEMBER:
 * - This is the only file that you need to edit!
 * - Declare variables only at the beginning of a function (as in C89).
 * - Do not include <stdio.h> header (it confuses dlc)
 * - Check correctness with ./btest
 * - Check operator constraints with ./dlc -e bits.c
 * - Run ./grade before you commit/push your solution
 */

/*
 * tmin - return minimum two's complement integer
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 1
 *   Rating: 1
 */

int tmin(void) {
  return 0x8 << 28;
}

/*
 * bitOr - x|y using only ~ and &
 * (tip: always remember De Morgan's laws!)
 *   Example: bitOr(6, 5) = 7
 *   Legal ops: ~ &
 *   Max ops: 4
 *   Rating: 1
 */
int bitOr(int x, int y) {
  return ~(~x&~y);
}

/*
 * negate - return -x
 * (tip: remember the definition of two's complement!)
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 2
 *   Rating: 1
 */
int negate(int x) {
  return ~x+1;
}

/*
 * isEqual - return 1 if x == y, and 0 otherwise
 * (tip: which bitwise op computes the difference?)
 *   Examples: isEqual(5,5) = 1, isEqual(4,5) = 0
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 2
 *   Rating: 2
 */
int isEqual(int x, int y) {
  return !(x^y);
}

/* 
 * absVal - absolute value of x
 *   Example: absVal(-1) = 1.
 *   You may assume -TMax <= x <= TMax
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 4
 */
int absVal(int x) {
  int y;
  y = x >> 31; //All 1 if neg, All 0 if pos
  return (y+x) ^ y; //if x was positive, doesn't change the result; if x was negative, add -1 and negate
}

/*
 * isLess - if x < y  then return 1, else return 0
 *   Example: isLess(4,5) = 1, isLess(5,4) = 0
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 18
 *   Rating: 3
 */
int isLess(int x, int y) {
  //int sign  = ((!(x>>31)) ^ (!(y>>31)));
  int xsign, ysign, z;
  xsign = (x>>31) & 1;
  ysign = (y>>31) & 1;
  z = ((~y+x+1)>>31) & 1; //computes the diff between x and y and see if its pos or neg
  return ((xsign) & (!ysign)) | (z & !(xsign ^ ysign)); //if ysign is 0 and xsign is 1 return 1 immediately; otherwise make sure x and y same sign and return z
}

/*
 * satAdd - adds two numbers but when positive overflow occurs, returns
 *          maximum possible value, and when negative overflow occurs,
 *          it returns minimum negative value.
 *   Examples: satAdd(0x40000000,0x40000000) = 0x7fffffff
 *             satAdd(0x80000000,0xffffffff) = 0x80000000
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 25
 *   Rating: 4
 */
int satAdd(int x, int y) {
  int sum, sumSign, xSign, ySign, inputSign, temp;
  sum = x + y;
  sumSign = (sum >> 31);
  xSign = (x >> 31);
  ySign = (y >> 31);
  inputSign = ((xSign ^ ySign)); //0 represents same sign
  inputSign = (inputSign ^ (xSign ^ (sumSign))) & (xSign ^ (sumSign)); //1 if sum and x are diff sign but x and y are same sign, so overflow
  temp = (~sumSign & (0x8 << 28)) | (sumSign & ((0x8 << 28) ^ (~0x0))); //if sumSign is 1(neg), then return max
  //return (~inputSign & sum);
  return (~inputSign & sum) | (inputSign & temp);
}

/*
 * dividePower2 - Compute x/(2^n), for 0 <= n <= 30
 *  Round toward zero
 *   Examples: dividePower2(15,1) = 7, dividePower2(-33,4) = -2
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 3
 */
int dividePower2(int x, int n) {
    int sign, y;
    sign = x >> 31;
    y = sign & ((1 << n) + ~0x0); //if neg, add 2^n - 1
    return (x+y) >> n;
}

/* 
 * replaceByte(x,n,c) - Replace byte n in x with c
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: replaceByte(0x12345678,1,0xab) = 0x1234ab78
 *   You can assume 0 <= n <= 3 and 0 <= c <= 255
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 3
 */
int replaceByte(int x, int n, int c) {
    int temp = 0xFF << (n << 3);
    temp = x & ~temp; //clear the bytes we need to replace
    return temp + (c << (n << 3));
}

/*
 * floatNegate - Return bit-level equivalent of expression -f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 7
 *   Rating: 2
 */
unsigned floatNegate(unsigned uf) {
  unsigned temp;
  temp = (uf >> 23) & 0xFF;
  if (temp == 0xFF) {
    if ((uf << 9) != 0x0) {
      return uf;
    }
  } 
  return uf ^ (0x1 << 31);
}

/*
 * floatIsEqual - Compute f == g for floating point arguments f and g.
 *   Both the arguments are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   If either argument is NaN, return 0.
 *   +0 and -0 are considered equal.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 19
 *   Rating: 3
 */
int floatIsEqual(unsigned uf, unsigned ug) {
  unsigned ufexpo, uffrac, ugexpo, ugfrac;
  ufexpo = (uf >> 23) & 0xFF;
  uffrac = (uf << 9);
  ugexpo = (ug >> 23) & 0xFF;
  ugfrac = (ug << 9);
  if (ufexpo == 0xFF) {
    if (uffrac != 0x0) {
      return 0;
    }
  }
  if (ugexpo == 0xFF) {
    if (ugfrac != 0x0) {
      return 0;
    }
  }
  if (uf == ug) {
    return 1;
  } 
  if (uffrac == 0x0) {
    if (ugfrac == 0x0) {
      if (ufexpo == 0x0) {
        if (ugfrac == 0x0) {
          if ((uf << 1) == (ug << 1)) {
            return 1;
          }
        }
      }
    }
  }
  return 0;
}

/*
 * floatFloat2Int - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 18
 *   Rating: 4
 */
int floatFloat2Int(unsigned uf) {
  unsigned ufexpo, uffrac, temp, sign;
  ufexpo = (uf >> 23) & 0xFF;
  temp = 0xFF << 24;
  sign = uf & (1 << 31);
  uffrac = (uf & ~temp) | (0x1 << 23);
  if (ufexpo < 127) {
    return 0;
  }
  else if (ufexpo == 127) {
    if (sign) {
      return -1;
    }
    else {
      return 1;
    }
  }
  else if (ufexpo <= 150) {
    unsigned tempp = (uffrac >> (23 - (ufexpo - 127)));
    if (sign) {
      return ~tempp+1;
    }
    else {
      return tempp;
    }
  }
  else {
    return 0x80000000u;
  }
}
