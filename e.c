
/* vi:set ts=2: */
/* tab size is 2 */

#include <stdio.h>
#include <math.h>

typedef double type;  

char c;

int argc, arg;
char **argv, *p;

void next()
{
	do
		{
			if(arg != argc && !*p) p = argv[++arg];
	
			if(arg != argc)	c = *p++; else c = 0;
		}
	while(c == ' ');
}

void syntax()
{
	int tc, c = -1, g = 1;
	char *t = argv[g];
	
	while(1)
		{
			if(t == p) tc = c;
			c++;
			
			if(g != argc && !*t) t = argv[++g];
	
			if(g != argc) putchar(*t++); else break;
		}
		
	putchar('\n');
	
	if(tc>16)
		{
			for(c=0;c<tc-16;c++) putchar(' ');
			printf("syntax error ---^\n");
		}
	else
		{
			for(c=0;c<tc;c++) putchar(' ');
			printf("^--- syntax error\n");
		}
		
	exit(1);
}

void unknown(char *s)
{
	printf("'%s': unknown function\n",s);
	exit(1);	
}


type E();
type term();

type constant()
{
	type r = 0;
	
	while(c >= '0' && c <= '9') 
		{ r = 10*r + (c-'0'); next(); }
	
	if(c == '.')
		{
			type p = 0.1;
			next();
			
			while(c >= '0' && c <= '9') 
				{ r += p * (c-'0'); p /= 10; next(); }
		}
	
	if(c == 'e' || c == 'E')
		{ 
			type m = 1;			

			next();
		  if(c == '-') { m = -m; next(); } else if(c == '+') next();

			r *= pow(10,m*term());
		}
		
	return r;
}


type function()
{
	char f[20], *p;
	type v;
	
	p = f;
	while(p-f < 19 && c >= 'a' && c <= 'z') { *p++ = c; next(); }
	
	*p = 0;
	
	if(!strcmp(f,"pi")) return M_PI;
	if(!strcmp(f,"e" )) return M_E;

	v = term();
	
	#define mathfunc(a,b) if(!strcmp(f,a)) return b;

	mathfunc("abs"   , fabs(v));
	mathfunc("fabs"  , fabs(v));
	mathfunc("floor" , floor(v));
	mathfunc("ceil"  , ceil(v));
	mathfunc("sqrt"  , sqrt(v));
	mathfunc("exp"   , exp(v));

	mathfunc("sin"   , sin(v));
	mathfunc("cos"   , cos(v));
	mathfunc("tan"   , tan(v));
	mathfunc("asin"  , asin(v));
	mathfunc("acos"  , acos(v));
	mathfunc("atan"  , atan(v));

	mathfunc("sinh"  , sinh(v));
	mathfunc("cosh"  , cosh(v));
	mathfunc("tanh"  , tanh(v));
	mathfunc("asinh" , asinh(v));
	mathfunc("acosh" , acosh(v));
	mathfunc("atanh" , atanh(v));

	mathfunc("ln"    , log(v));
	mathfunc("log"   , log(v)/log(2));
	
	unknown(f);
	return 0;
}

type term()
{
	/*type m = 1;*/
	const int m = 1;
		
	/*
	  if(c == '-') { m = -m; next(); }
	  else 
	  if(c == '+') next();
	*/
		
	if(c=='(' || c=='[')
		{
			type r;
			
			next();
			r = E();
			if(c != ')' && c !=']') syntax();
			
			next();
			return m * r;
		}
	
	else if((c >= '0' && c <= '9') || c == '.')
					return m * constant();
					
	else if(c >= 'a' && c <= 'z')
					return m * function();
}


static inline
type factorial(type v)
{
	type i, r = 1;
	
	for(i=2;i<=v;i++) r *= i;
	return r;
}

type H()
{
	type r = term();
	
	if(c == '!') { next(); r = factorial(r); }
	return r;
}

type G()
{
	type q, r = H();
	
	while(c == '^')
		{ next(); q = G(); r = pow(r,q); }
	
	return r;
}

type F()
{
	type r = G();

	while(1)
		{
			if(c=='*') { next(); r *= G(); }
			else
			if(c=='/') { next(); r /= G(); }
			else
			if(c=='%') { next(); r = fmod(r,G()); }
			else break;
		}
	
	return r;
}

type E()
{
	type r = F();

	while(1)
		{
			if(c=='+') { next(); r += F(); }
			else
			if(c=='-') { next(); r -= F(); }
			else break;
		}
	
	return r;
}

type S()
{
	type r = E();
	
	if(c != 0) syntax();
	return r;
}

void
format(type X)
{
	type i,f;
	int d;
	
	if(!finite(X)) { printf("%f", X); return ; }
	
	f = fabs(modf(X,&i));
	d = floor(log10(fabs(X)))+1;

	/* f is fractional part, i is integer part */ 
		
	/* printf("%.30f\n",(X)); */
	
	if(finite(f) && f!=0)
		{
			char *p, s[2000], t[2000];
			
			sprintf(t, "%%.%df", 15-d);
			sprintf(s,t,f);
			
			/*
			printf("(t=%s)",t);
			printf("(s=%s)",s);
			*/


			/* remove all zeros from s */
			p = s; 
			while(*p) p++; p--;
			while(p>s && *p=='0') *p-- = 0; 
			
			if(s[0] == '1') /* decimal part has been rounded */
				printf("%.0f",i+(X>=0?1:-1));
			else
				{			
					printf("%.0f",i);
					if(s[2] != 0) printf("%s",s+1);
				}
		}
	else
		printf("%.0f",i);

}

int main(int _argc, char **_argv)
{
	argc = _argc;
	argv = _argv;
	arg = 1;
	p = argv[arg];
	
	next();
	format(S());	
	printf("\n");
	
	return 0;
}
