#include "PLL.h"
#include "keypad.h"
#include "LCD.h"
#include "math.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
float values[16];
char ops[16];

static int op_order (char op){
	if (op == 'x' || op == '/') return 1;
	if (op == '+' || op == '-') return 2;
	return 0;
}

static float equation_apply(float a, float b, char op){
    switch(op){
      case '+': return a + b;
			case '-': return a - b;
      case 'x': return a * b; 
      case '/': return a / b;
    }
    return 0;
}

static int reduce_string(float *values, int *val_end, char *ops, int *ops_end){
  // insert end values of the array to b, then a
  float b = values[(*val_end)--];
  float a = values[(*val_end)--];
	//Make op the highest order operator
  char op = ops[(*ops_end)--];
 // do the actual arithmetic
	if (op == '/' && b == 0.0f) return 0;
  float r = equation_apply(a, b, op);
  // push result back onto values stack for further calculation
  values[++(*val_end)] = r;
  return 1;
}

bool calculate_expression(const char *expr, float *result){
   int i=0; 
	 int val_length = -1, op_length = -1;
 	 while (expr[i] == ' ') i++; 
	if(!(isdigit((unsigned char)expr[i]) || expr[i]=='.' || (expr[i]=='-' && (i==0 || strchr("+-x/", expr[i-1]))))){
	return false;
}
while (expr[i] != '\0'){ //Checks if at end of the expression
		if (expr[i] == ' ') { i++; continue; } //Skip any spaces
		char *end;
		float v = strtof(&expr[i], &end); //Convert string to floating point
		if (end == &expr[i]){return false;}
		if (val_length >= 15){return false;}
		values[++val_length] = v; //Expand the value array to input the converted float.
		i = (int)(end-expr);
		
		if (expr[i] == '\0') break;    
		if (expr[i] == ' ') { i++; continue; } 
		
	if(strchr("x/-+", expr[i])){ //strchr locates first occurence of an op
		char op = expr[i++];
		while(op_length >= 0 && op_order(ops[op_length]) <= op_order(op)){
		if(!reduce_string(values, &val_length,  ops, &op_length)){return false;}
		} 
		if (op_length >= 15){return false;}
		ops[++op_length] = op;
		continue;
	}
	return false;
}

		while(op_length >= 0){
			if (val_length < 1){return false;}
			if(!reduce_string(values, &val_length, ops, &op_length)){return false;}
		}
		if (val_length !=0){return false;}
		*result = values[0];
		return true;
	}
